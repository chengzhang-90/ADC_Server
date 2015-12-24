/*****************************************************************
*
*    Copyright (C), 2010-2020, NewLand Computer Co., Ltd.
*    Version:  VER1.0
*    Description: 接收启动、停止子服务命令请求
*    History:
*
*    Function List:
*
*    date:   2010-7-20
*    author: zhou hong di
*    created: zhou hong di
*
*    date:
*    author:
*    modify description
*
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/ioctl.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "adc_manager.h"
#include "log.h"
#include "adc_cmdsvr.h"
#include "adc_base.h"

/*
 *
 * Function: CommandServer
 * Description: 系统构造函数
 * Return:
 * Other:
 *
 */
CommandServer::CommandServer( void )
{
   char aczLogIni[128];
   int iFd;

   apr_initialize();
   apr_pool_create( &m_pstRoot, NULL );

   m_pstSystem = NULL;
   m_pstProcess = NULL;

   setupSignals();

#ifndef __hpux
   if((iFd = open("/dev/tty",O_RDWR)) >= 0)
   {
      ioctl( iFd, TIOCNOTTY, NULL );
      close( iFd );
   }
#endif

   sprintf( aczLogIni, "%s/etc/%s", getenv("ADC_ROOT"), "cmdsvrlog.ini" );
   LOG4C_INIT( aczLogIni ); 
   LOG4C_OPEN();
}

/*
 *
 * Function: ~CommandServer
 * Description: 系统析构函数
 * Return:
 * Other:
 *
 */
CommandServer::~CommandServer( void )
{
   cleanup();
   apr_pool_destroy( m_pstRoot );
   apr_terminate();
}

/*
 *
 * Function: cleanup
 * Description: 系统退出时清除工作
 * Return:
 * Other:
 *
 */
void CommandServer::cleanup( void )
{
   apr_socket_close( m_pstListen );
   LOG4C_CLOSE();
}

/*
 *
 * Function: loadConfig
 * Description: 加载系统配置文件cmdsvr.xml
 * Input: 
 *      pczXmlConfig - 配置文件
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
int CommandServer::loadConfig( char *pczXmlConfig )
{
   xmlDocPtr  doc;
   xmlNodePtr node;

   LIBXML_TEST_VERSION
   xmlKeepBlanksDefault(0);

   xmlInitParser();

   m_pstConfig = (SystemConfig*)apr_palloc( m_pstRoot, sizeof(SystemConfig) );

   doc = xmlParseFile( pczXmlConfig );
   if( doc == NULL )
   {
      printf( "xmlParseFile( \"%s\" ) error!!\n ", pczXmlConfig );
      xmlCleanupParser();
      return -1;
   }

   /* 取根节点 */
   node = xmlDocGetRootElement( doc );
   if( node == NULL )
   {
      printf( "空的 xml 配置文件!!\n" );
      xmlFreeDoc( doc );
      xmlCleanupParser();
      return -1;
   }

   node = node->xmlChildrenNode;

   /**
     * 开始解析 xml 文件
     */
   while( node != NULL )
   {
      if( node->type == XML_ELEMENT_NODE )
      {
         xmlNodePtr txtNode = node->xmlChildrenNode;
         if( xmlStrcmp( node->name, (const xmlChar*)"port" ) == 0 )
         {
            m_pstConfig->m_iListenPort = atoi((char*)txtNode->content);
         }
         else if( xmlStrcmp( node->name, (const xmlChar*)"grpshmkey" ) == 0 )
         {
            m_pstConfig->m_iGrpShmKey = atoi((char*)txtNode->content);
         }
         else if( xmlStrcmp( node->name, (const xmlChar*)"prcshmkey" ) == 0 )
         {
            m_pstConfig->m_iPrcShmKey = atoi((char*)txtNode->content);
         }
         else if( xmlStrcmp( node->name, (const xmlChar*)"maxsys" ) == 0 )
         {
            m_pstConfig->m_iMaxSys = atoi((char*)txtNode->content);
         }
         else if( xmlStrcmp( node->name, (const xmlChar*)"maxchild" ) == 0 )
         {
            m_pstConfig->m_iMaxChild = atoi((char*)txtNode->content);
         }
      }
      node = node->next;
   } 

   xmlFreeDoc( doc );
   xmlCleanupParser();

   return 0;
}

/*
 *
 * Function: setupSignals
 * Description: 屏蔽系统其他信号
 * Input: 
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
int CommandServer::setupSignals( void )
{
   apr_signal( SIGTERM, SIG_IGN );
   apr_signal( SIGINT,  SIG_IGN );
   apr_signal( SIGPIPE, SIG_IGN );
   apr_signal( SIGHUP,  SIG_IGN );
   apr_signal( SIGTTOU, SIG_IGN ); 
   apr_signal( SIGTTIN, SIG_IGN ); 
   apr_signal( SIGTSTP, SIG_IGN ); 

   for( int fd =3, fdtablesize = getdtablesize(); fd < fdtablesize; fd++ )
   {
      close(fd);
   }

   return 0;
}

/*
 *
 * Function: initShareMemory
 * Description: 系统管理共享内存分配
 * Input: 
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
int CommandServer::initShareMemory( void )
{
   int iSize, iMaxSys, iMaxChildPerSys, iShmId;
   apr_status_t tStatus;

   iMaxSys = m_pstConfig->m_iMaxSys;
   iMaxChildPerSys = m_pstConfig->m_iMaxChild;
   if( iMaxSys > 128 )
   {
      iMaxSys = 128;
   }
   if( iMaxSys < 16 )
   {
      iMaxSys = 16;
   }
   if( iMaxChildPerSys > 256 )
   {
      iMaxChildPerSys = 256;
   }
   if( iMaxChildPerSys < 2 )
   {
      iMaxChildPerSys = 2;
   }

   iSize = sizeof( ProcessRegister ) * iMaxSys * iMaxChildPerSys;
   LOG4C(( LOG_DEBUG, "计算出的进程管理共享内存大小 %d\n", iSize ));
   m_pstProcess = (ProcessRegister*)shm_get( m_pstConfig->m_iPrcShmKey, iSize );
   if( m_pstProcess == NULL )
   {
      LOG4C(( LOG_DEBUG, "创建共享内存(KEY=%d)失败", m_pstConfig->m_iPrcShmKey ));
      return -1;
   }

   iSize = sizeof(SystemRegister) * m_pstConfig->m_iMaxSys;
   LOG4C(( LOG_DEBUG, "计算出的系统管理共享内存大小 %d\n", iSize ));
   m_pstSystem = (SystemRegister*)shm_get( m_pstConfig->m_iGrpShmKey, iSize );
   if( m_pstSystem == NULL )
   {
      LOG4C(( LOG_DEBUG, "创建共享内存(KEY=%d)失败", m_pstConfig->m_iGrpShmKey ));
      return -1;
   }

   return 0;
}

/*
 *
 * Function: getEmptyIndex
 * Description: 查找空系统管理器下标
 * Input: 
 * OutPut:
 *      
 * Return:
 *        -1 - 查找失败
 *        >=0 下标
 * Other:
 *
 */
int CommandServer::getEmptyIndex( void )
{
   for( int i = 0; i < m_pstConfig->m_iMaxSys; i++ )
   {
      if( !m_pstSystem[i].m_bActive )
      {
         return i;
      }
   }

   return -1;
}

/*
 *
 * Function: getRunningIndex
 * Description: 取子服务系统下标
 * Input: 
 * OutPut:
 *      
 * Return:
 *        -1 - 查找失败
 *        >=0 下标
 * Other:
 *
 */
int CommandServer::getRunningIndex( char *pczName )
{
   for( int i = 0; i < m_pstConfig->m_iMaxSys; i++ )
   {
      if( (strcmp( m_pstSystem[i].m_aczName, pczName ) == 0) &&
          m_pstSystem[i].m_bActive )
      {
         return i;
      }
   }

   return -1;
}

/*
 *
 * Function: saveBusiManager
 * Description:  在指定下标保存子系统信息
 * Input: 
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
void CommandServer::saveBusiManager( int iIndex, char *pczName, pid_t tPid )
{
   LOG4C(( LOG_DEBUG, "保存子系统位置 %d 名子 [%s]\n", iIndex, pczName ));
   m_pstSystem[iIndex].m_bActive = true;
   strcpy( m_pstSystem[iIndex].m_aczName, pczName );
}


/*
 *
 * Function: runAnyManager
 * Description: 启动子系统服务命令 
 * Input: 
 *      pczName - 子系统服务名
 *      iIndex  - 子系统服务名下标
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
pid_t CommandServer::runAnyManager( char *pczName, int iIndex )
{
   char aczRspFd[10], aczGrpShmKey[12], aczPrcShmKey[12], aczIndex[12];
   char aczMaxChild[12], aczExecFile[]="nlmanager";
   int  aiFd[2];

   if( pipe( aiFd ) == -1 )
   {
      LOG4C(( LOG_WARN, "创建命令管道失败 [%s]\n", strerror(errno) ));
      return -1;
   }

   sprintf( aczRspFd, "%d", aiFd[1] );
   sprintf( aczGrpShmKey, "%d", m_pstConfig->m_iGrpShmKey );
   sprintf( aczPrcShmKey, "%d", m_pstConfig->m_iPrcShmKey );
   sprintf( aczIndex, "%d", iIndex );
   sprintf( aczMaxChild, "%d", m_pstConfig->m_iMaxChild );

   LOG4C(( LOG_INFO, "调用子系统命令[%s][%s][%s][%s][%s][%s][%s]\n",
           aczExecFile, pczName, aczRspFd, aczGrpShmKey,
           aczPrcShmKey, aczIndex, aczMaxChild ));

   pid_t tPid = fork();

   if( tPid > 0 )
   {
      char aczInfo[1000];

      memset( aczInfo, 0, sizeof(aczInfo) );
      close( aiFd[1] );
      read( aiFd[0], aczInfo, 1000 );
      close( aiFd[0] );

      LOG4C(( LOG_DEBUG, "子进程返回信息[%s]\n", aczInfo ));
      if( aczInfo[0] != '0' )
      {
         kill( tPid, 9 );  /*不正常就杀掉*/
         return -1;
      }
      else
      {
         return tPid;
      }
   }
   else if( tPid == -1 )
   {
      LOG4C(( LOG_FATAL, "创建子进程资源不足\n" ));
      return -1;
   }

   LOG4C_CLOSE();

   apr_socket_close( m_pstListen );
   close( aiFd[0] );
   if( execlp( aczExecFile, aczExecFile, pczName, aczRspFd, aczGrpShmKey,
               aczPrcShmKey, aczIndex, aczMaxChild, NULL ) == -1 )
   {
      printf( "运行程序失败[%s]\n", strerror(errno) );
   }

   exit(0);

   return 0;
}

/*
 *
 * Function: start
 * Description:  启动子系统服务名
 * Input: 
 *      pstClient - 通讯接口字
 *      pczName  - 子系统服务名
 * OutPut:
 *      
 * Return:
 *      -1 -  FAIL
 *       0 -  SUCC
 * Other:
 *
 */
int CommandServer::start( apr_socket_t *pstClient, char *pczName )
{
   int iEmptyIdx = 0, iShmOffset;
   pid_t tChild;
   char aczRsp[100];
   apr_size_t tLen;

   if( getRunningIndex( pczName ) != -1 )
   {
      LOG4C(( LOG_WARN, "%s 子系统已经在运行\n", pczName ));
      sprintf( aczRsp, "<    %-11.11s>子系统已经在运行", pczName );
      tLen = strlen( aczRsp );
      apr_socket_send( pstClient, aczRsp, &tLen );
      return -1;
   }

   if( (iEmptyIdx = getEmptyIndex()) == -1 )
   {
      LOG4C(( LOG_WARN, "无足够的管理器运行 %s 子系统\n", pczName ));
      sprintf( aczRsp, "无足够的管理器运行 %s 子系统", pczName );
      tLen = strlen( aczRsp );
      apr_socket_send( pstClient, aczRsp, &tLen );
      return -1;
   }

   iShmOffset = iEmptyIdx*m_pstConfig->m_iMaxChild;

   if( !fileExistAndExec( "nlmanager" ) )
   {
      LOG4C(( LOG_WARN, "nlmanager 不存在, 请检查系统\n" ));
      sprintf( aczRsp, "系统错误, 请检查" );
      tLen = strlen( aczRsp );
      apr_socket_send( pstClient, aczRsp, &tLen );
      return -1;
   }

   if( (tChild = runAnyManager( pczName, iEmptyIdx )) == -1 )
   {
      LOG4C(( LOG_WARN, "运行 nlmanager 失败, 请检查系统\n" ));
      sprintf( aczRsp, "运行 nlmanager 失败, 请检查系统" );
      tLen = strlen( aczRsp );
      apr_socket_send( pstClient, aczRsp, &tLen );
      return -1;
   }

   LOG4C(( LOG_INFO, "%s 子系统已经开始运行\n", pczName ));
   sprintf( aczRsp, "<    %-11.11s>子系统启动成功|Start Success", pczName );
   tLen = strlen( aczRsp );
   apr_socket_send( pstClient, aczRsp, &tLen );

   saveBusiManager( iEmptyIdx, pczName, tChild );
   LOG4C(( LOG_INFO, "%s 子系统已经开始运行, 等待下一次运行\n", pczName ));

   return 0;
}

/*
 *
 * Function: stop
 * Description:  停止子系统服务名
 * Input: 
 *      pstClient - 通讯接口字
 *      pczName  - 子系统服务名
 * OutPut:
 *      
 * Return:
 *      -1 -  FAIL
 *       0 -  SUCC
 * Other:
 *
 */
int CommandServer::stop( apr_socket_t *pstClient, char *pczName )
{
   char aczInfo[256];
   int iIdx, iPort;
   apr_socket_t *pstCmdClient;
   apr_size_t tLen;

   memset( aczInfo, 0, sizeof(aczInfo) );
   if( (iIdx = getRunningIndex( pczName )) == -1 )
   {
      LOG4C(( LOG_WARN, "指定的子系统%s不存在", pczName ));
      strcpy( aczInfo, "指定的子系统不存在" );
      tLen = strlen( aczInfo );
      apr_socket_send( pstClient, aczInfo, &tLen );
      return -1;
   }

   LOG4C(( LOG_DEBUG, "要停的子系统[%s]在%d位置 pid = %d\n",
           pczName, iIdx, m_pstSystem[iIdx].m_tManagerPid ));

   if( m_pstSystem[iIdx].m_tManagerPid > 0 )
   {
      if( kill( m_pstSystem[iIdx].m_tManagerPid, 0 ) == 0 )
      {
         kill( m_pstSystem[iIdx].m_tManagerPid, SIGUSR1 );
      }
      else
      {
         LOG4C(( LOG_WARN, "检测进程出错%s\n", strerror(errno) ));
      }
   }
   m_pstSystem[iIdx].m_bActive = false;

   sprintf( aczInfo, "<    %-11.11s>子系统停止成功|Stop Success", pczName );
   tLen = strlen( aczInfo );
   apr_socket_send( pstClient, aczInfo, &tLen );

   return 0;
}

/*
 *
 * Function: report
 * Description:  取活动的子系统服务下标
 * Input: 
 *      pstClient - 通讯接口字
 *      pczName  - 子系统服务名
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
int CommandServer::report( apr_socket_t *pstClient, char *pczName )
{
   int iCount;
   char aczReport[256];
   

   if( strcmp( pczName, "all" ) == 0 )
   {
      return 0;
   }

   for( int i = 0; i < m_pstConfig->m_iMaxSys; i++ )
   {
      if( (strcmp( m_pstSystem[i].m_aczName, pczName ) == 0) &&
          m_pstSystem[i].m_bActive )
      {
         return i;
      }
   }
   
   return 0;
}

/*
 *
 * Function: doCommand
 * Description:  解析命令并执行
 * Input: 
 *      pstClient - 通讯接口字
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
int CommandServer::doCommand( apr_socket_t *pstClient )
{
   char aczCommand[64], aczName[64];

   /**
     * 解析命令, 执行命令
     */
   sscanf( m_aczCommand, "%s %s", aczCommand, aczName );
   LOG4C(( LOG_INFO, "%s %s\n", aczCommand, aczName ));

   if( strcmp( aczCommand, "start" ) == 0 )
   {
      start( pstClient, aczName );
   }
   else if( strcmp( aczCommand, "stop" ) == 0 )
   {
      stop( pstClient, aczName );
   }
   else if( strcmp( aczCommand, "restart" ) == 0 )
   {
      stop( pstClient, aczName );
      start( pstClient, aczName );
   }
   else if( strcmp( aczCommand, "report" ) == 0 )
   {
      report( pstClient, aczName );
   }
   else
   {
      char aczResponse[64];
      apr_size_t tSendLen;

      LOG4C(( LOG_WARN, "Invalid Command [%s]", aczCommand ));
      sprintf( aczResponse, "1Invalid Command %s", aczCommand );
      tSendLen = strlen(aczResponse);
      apr_socket_send( pstClient, aczResponse, &tSendLen );
   }

   return 0;
}

/*
 *
 * Function: checkExitedSystem
 * Description:  检查退出的子系统
 * Input: 
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
void CommandServer::checkExitedSystem( void )
{
   int iExitCode, iIdx;
   apr_exit_why_e tWhy;
   apr_status_t tStatus;
   apr_proc_t   stProcess;

   do
   {
      tStatus = apr_proc_wait_all_procs( &stProcess, &iExitCode, &tWhy,
                                         APR_NOWAIT, m_pstLocal );
      if( stProcess.pid > 0 && !APR_STATUS_IS_EINTR(tStatus) )
      {
         /*设置该子系统为可用*/
         for( iIdx = 0; iIdx < m_pstConfig->m_iMaxSys; iIdx++ )
         {
            if( m_pstSystem[iIdx].m_tManagerPid == stProcess.pid )
            {
               m_pstSystem[iIdx].m_tManagerPid = 0;
               m_pstSystem[iIdx].m_bActive = false;
               break;
            }
         }
         if( iIdx == m_pstConfig->m_iMaxSys )
         {
            LOG4C(( LOG_WARN, "请尽快检查系统，有个进程%d不知道怎么来的\n",
                    stProcess.pid ));
         }
      }
      else
      {
         return;
      }
   } while( true );
}

/*
 *
 * Function: maintenance
 * Description:  接收命令或者每隔指定时间维护系统信息
 * Input: 
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
void CommandServer::maintenance( void )
{
   apr_status_t tStatus;
   apr_socket_t *pstCommand;
   apr_interval_time_t tTimeOut = 5000000; /*设置5秒的超时时间*/
   apr_size_t   tRecvLen;

   apr_pool_create( &m_pstLocal, m_pstRoot );
   apr_socket_timeout_set( m_pstListen, 1000000 );

   while( true )
   {
      apr_pool_clear( m_pstLocal );
      memset( m_aczCommand, 0, sizeof(m_aczCommand) );
      checkExitedSystem( );
      tStatus = apr_socket_accept( &pstCommand, m_pstListen, m_pstLocal );
      if( tStatus != APR_SUCCESS )
      {
         apr_sleep( 1000000 );
         continue;
      }

      apr_socket_timeout_set( pstCommand, tTimeOut );
      tRecvLen = 2047;
      tStatus = apr_socket_recv( pstCommand, m_aczCommand, &tRecvLen );
      if( tStatus != APR_SUCCESS )
      {
         LOG4C(( LOG_WARN, "从客户端接收命令失败 %s\n", strerror(errno) ));
         apr_socket_close( pstCommand );
         continue;
      }

      doCommand( pstCommand );

      apr_socket_close( pstCommand );
   }
}

/*
 *
 * Function: run
 * Description:  
 * Input: 
 * OutPut:
 *      
 * Return:
 * Other:
 *
 */
int CommandServer::run( void )
{
   int iSize;

   if( initShareMemory() == -1 )
   {
      return -1;
   }

   int iRet = makeListener( &m_pstListen, m_pstRoot, m_pstConfig->m_iListenPort,
                            APR_INET, SOCK_STREAM );
   if( iRet == -1 )
   {
      return -1;
   }

   maintenance();

   return 0;
}

/*
 *   
 * Function: main 
 * Description: 应用数据中心主函数开始
 * 参数:
 *      xmlconfig - 管理程序对应的配置文件 
 * Return:
 * Other:
 */
int main( int argc, char *argv[] )
{
   if( argc != 2 )
   {
      printf( "useage : %s config_xml \n", argv[0] );
      return -1;
   }

   if( fork() > 0 )
   {
      return 0;
   }

   CommandServer commandServer;

   if( commandServer.loadConfig( argv[1] ) == -1 )
   {
      printf( "load config [%s] error\n", argv[1] );
      return -1;
   }

   commandServer.run();

   return 0;
}
