/*****************************************************************
*
*    Copyright (C), 2010-2020, NewLand Computer Co., Ltd.
*    Version:  VER1.0
*    Description: 管理子应用的主程序
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
#include <string.h>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "adc_anymanager.h"
#include "apr_portable.h"
#include "log.h"
#include "adc_base.h"

static int iCmdFd;
/*
 *
 * Function: closeParent
 * Description: 
 * Input:
 * Output:
 * Return:
 * Other:
 *
 */
void closeParent( int iExcepFd1, int iExcepFd2 )
{
   int iFd, iFdSize;

   iFdSize = getdtablesize();
   for( iFd = 3; iFd < iFdSize; iFd++ )
   {
      if( iExcepFd1 != iFd && iExcepFd2 != iFd )
      {
         close( iFd );
      }
   }
}


/*
 *
 * Function: AnyManager
 * Description: 系统子进程管理构造函数
 * Input:
 *         pstSysName - 子进程名
 *         iIndex - 节点号
 *         iMaxChile - 运行子进程运行的最多数
 * Output:
 *
 * Return:
 * Other:
 *
 */
AnyManager::AnyManager( char *pczSysName, int iIndex, int iMaxChild )
{
   int iFd;

   strcpy( m_aczName, pczSysName );

   m_iIndex          = iIndex;

#ifndef __hpux
   if((iFd = open("/dev/tty",O_RDWR)) >= 0)
   {
      ioctl( iFd, TIOCNOTTY, NULL );
      close( iFd );
   }
#endif

   apr_initialize();
   apr_pool_create( &m_pstPool, NULL );
   apr_pool_create( &m_pstMaintanence, NULL );

   m_iGrpShmKey  = 0;
   m_iProcShmKey = 0;
   m_iCfgShmKey  = 0;

   m_iMinProcess = 0;
   m_iMaxProcess = 0;
   m_iMaxAllowProcess = iMaxChild;

   m_pstProcess = NULL;
   m_pstSystem  = NULL;

   m_pstListener = NULL;
}

/*
 *
 * Function: ~AnyManager
 * Description: 系统子进程管理析构函数
 * Input:
 * Output:
 * Return:
 * Other:
 *
 */
AnyManager::~AnyManager()
{
   if( m_pstListener != NULL )
   {
      apr_socket_close( m_pstListener );
   }

   if(  m_pstProcess != NULL ) 
   {
      shmdt( m_pstProcess );
   }
   if(  m_pstSystem != NULL ) 
   {
      shmdt( m_pstSystem );
   }

   /*释放所有分配的内存*/
   /*apr_pool_destroy( m_pstPool );*/
   /*apr_pool_destroy( m_pstMaintanence );*/

   apr_terminate();
}

/*
 *
 * Function: getValue
 * Description: 取XML格式文件的指定字段
 * Input:
 *         pstItem - 字段名
 * Output:
 *
 * Return:
 *         NULL - 空值
 *         char *  字段对应的内容
 * Other:
 *
 */
char *AnyManager::getValue( const char *pczItem )
{
   ItemConfig *pstConfig;

   pstConfig = m_stConfig.m_pstNext;
   while( pstConfig != NULL )
   {
      if( strcmp( pstConfig->m_pczKey, pczItem ) == 0 )
      {
         return pstConfig->m_pczVal;
      }
      pstConfig = pstConfig->m_pstNext;
   }

   return NULL;
}

/*
 *
 * Function: loadConfig
 * Description: 加载XML格式文件
 * Input:
 * Output:
 * Return:
 *         -1 - FAIL
 *         0  - SUCC
 * Other:
 *         配置信息保存在 $ADC_ROOT/etc 下以子系统为文件的 xml 文件中
 *
 */
int AnyManager::loadConfig( void )
{
   char aczConfig[128];
   ItemConfig *pstConfig, *pstTail;

   sprintf( aczConfig, "%s/etc/%s.xml", getenv("ADC_ROOT"), m_aczName );
   m_stConfig.m_pstNext = NULL;

   xmlDocPtr  doc;
   xmlNodePtr node;

   LIBXML_TEST_VERSION
   xmlKeepBlanksDefault(0);

   xmlInitParser();

   LOG4C(( LOG_DEBUG, "子系统[%s]配置文件[%s]", aczConfig ));
   doc = xmlParseFile( aczConfig );
   if( doc == NULL )
   {
      LOG4C(( LOG_FATAL, "xmlParseFile( %s ) error!!\n ", aczConfig ));
      xmlCleanupParser();
      return -1;
   }

   /* 取根节点 */
   node = xmlDocGetRootElement( doc );
   if( node == NULL )
   {
      LOG4C(( LOG_FATAL, "failed to xmlDocGetRootElement %s\n ", aczConfig ));
      xmlFreeDoc( doc );
      xmlCleanupParser();
      return -1;
   }

   node = node->xmlChildrenNode;

   pstTail = &m_stConfig;
   pstTail->m_pstNext = NULL;

   while( node != NULL )
   {
      if( node->type == XML_ELEMENT_NODE )
      {
         xmlNodePtr txtNode = node->xmlChildrenNode;
         pstConfig = (ItemConfig*)apr_palloc( m_pstPool, sizeof(m_stConfig) );
         pstConfig->m_pczKey = (char*)apr_palloc( m_pstPool,
                                      xmlStrlen(node->name)+1 );
         pstConfig->m_pczVal = (char*)apr_palloc( m_pstPool,
                                      xmlStrlen(txtNode->content)+1 );
         strcpy( pstConfig->m_pczKey, (char*)node->name );
         strcpy( pstConfig->m_pczVal, (char*)txtNode->content );
         pstConfig->m_pstNext = NULL;
         pstTail->m_pstNext = pstConfig;
         pstTail = pstConfig;
      }
      node = node->next;
   }

   LOG4C_INIT( getValue("logini") );
   LOG4C_OPEN();

   int iRet = 0;
   if( getValue( (const char*)"port" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "port"));
      iRet = -1;
   }
   if( getValue( (const char*)"exec" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "exec"));
      iRet = -1;
   }
   if( getValue( (const char*)"logini" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "logini"));
      iRet = -1;
   }
   if( getValue( (const char*)"sysenv" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "sysenv"));
      iRet = -1;
   }
   if( getValue( (const char*)"shmkey" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "shmkey"));
      iRet = -1;
   }
   if( getValue( (const char*)"runtimes" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "runtimes"));
      iRet = -1;
   }
   if( getValue( (const char*)"minproc" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "minproc"));
      iRet = -1;
   }
   if( getValue( (const char*)"maxproc" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "maxproc"));
      iRet = -1;
   }
   if( getValue( (const char*)"nodeid" ) == NULL )
   {
      LOG4C(( LOG_FATAL, "缺少 %s 配置项\n", "nodeid"));
      iRet = -1;
   }

   xmlFreeDoc( doc );
   xmlCleanupParser();

   return iRet;
}


/*
 *
 * Function: childMain
 * Description:  exec 方式执行进程
 * Input: 
 *      iOffset - 第几个子服务程序
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::childMain( int iOffset )
{
   char aczSocket[10];
   char aczExecFile[1024];
   char aczProcShmKey[32];
   char aczCfgShmKey[32];
   char aczMaxChild[32];
   char aczIndex[32];
   char aczOffset[32];
   char aczLogIni[128];
   char aczMaxRunTimes[32];
   char aczNodeId[32];
   char *pczValue;
   apr_os_sock_t tSocket;

   /**
     * m_pstListener  == NULL 时表明实际处理进程为非socket通讯进程
     * 不用传递socket id到实际交易处理进程 
     */
   if (m_pstListener != NULL)
   {
       apr_os_sock_get( &tSocket, m_pstListener );
       sprintf( aczSocket, "%d", tSocket );
   }
   else
   {
       strcpy (aczSocket, "-1");
   }

   if( (pczValue = getValue("exec") ) == NULL )
   {
      printf( "exec 不存在\n" );fflush(stdout);
      return;
   }
   else
   {
      sprintf( aczExecFile, "%s", pczValue );
   }
   sprintf( aczProcShmKey, "%d", m_iProcShmKey );

   if( (pczValue = getValue("shmkey") ) == NULL )
   {
      printf( "shmkey不存在\n" );fflush(stdout);
      return;
   }
   else
   {
      sprintf( aczCfgShmKey, "%s", pczValue );
   }

   if( (pczValue = getValue("logini") ) == NULL )
   {
      printf( "exec 不存在\n" );fflush(stdout);
      return;
   }
   else
   {
      sprintf( aczLogIni, "%s", pczValue );
   }

   if( (pczValue = getValue("runtimes") ) == NULL )
   {
      printf( "runtimes 不存在\n" );fflush(stdout);
      return;
   }
   else
   {
      sprintf( aczMaxRunTimes, "%s", pczValue );
   }

   if( (pczValue = getValue("nodeid") ) == NULL )
   {
      printf( "nodeid 不存在\n" );fflush(stdout);
      return;
   }
   else
   {
      sprintf( aczNodeId, "%s", pczValue );
   }
   sprintf( aczMaxChild, "%d", m_iMaxAllowProcess );
   sprintf( aczIndex, "%d", m_iIndex );
   sprintf( aczOffset, "%d", iOffset );

/***
   printf("---------------%s----------\n",aczExecFile );
   printf("---------------%s----------\n",m_aczName );
   printf("---------------%s----------\n",aczSocket );
   printf("---------------%s----------\n",aczProcShmKey );
   printf("---------------%s----------\n",aczCfgShmKey );
   printf("---------------%s----------\n",aczMaxChild );
   printf("---------------%s----------\n",aczIndex );
   printf("---------------%s----------\n",aczOffset);
   printf("---------------%s----------\n",aczLogIni);
   printf("---------------%s----------\n",aczMaxRunTimes );
   printf("---------------%s----------\n",aczNodeId);
***/

   LOG4C(( LOG_DEBUG, "参数[%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s]\n",
           aczExecFile, m_aczName, aczSocket, aczProcShmKey, aczCfgShmKey,
           aczMaxChild, aczIndex, aczOffset, aczLogIni, aczMaxRunTimes,
           aczNodeId ));

   if( execlp( aczExecFile, aczExecFile, m_aczName, aczSocket, aczProcShmKey,
               aczCfgShmKey, aczMaxChild, aczIndex, aczOffset, aczLogIni,
               aczMaxRunTimes, aczNodeId, NULL ) == -1 )
   {
      printf( "子系统[%s]运行出错[%s]\n", aczExecFile, strerror(errno) );
   }
   else
   {
      printf( "子系统[%s]退出\n", aczExecFile );
   }
}

/*
 *
 * Function: makeChild
 * Description:  创建子服务程序
 * Input: 
 *      iSlot - 第几个子服务程序
 * Output:
 * Return:
 * Other:
 *
 */
int AnyManager::makeChild( int iSlot )
{
   ProcessRegister *pstRegister;
   pid_t tPid;

   pstRegister = m_pstProcess + iSlot;
   /*由父进程创建子进程*/
   pstRegister->m_bActive = true;
   pstRegister->m_iRunTimes = 0;

   do
   {
      tPid = fork();
      if( tPid == -1 )
      {
         apr_sleep( 1000000 );
      }
   } while( tPid == -1 );

   if( tPid > 0 )
   {
      pstRegister->m_tPid = tPid;
      return 0;
   }

   LOG4C(( LOG_INFO, "系统名称 %s \n", m_aczName ));
   childMain( iSlot );

   apr_sleep( 1000000 );
   exit( 0 );

   return 0;
}

/*
 *
 * Function: prepare
 * Description:  检测该进程配置信息,确保可以运行
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
int AnyManager::prepare( int iGrpShmKey, int iProcShmKey )
{
   m_iGrpShmKey = iGrpShmKey;
   m_iProcShmKey = iProcShmKey;
   m_iCfgShmKey = atoi(getValue("shmkey"));

   setupSignal();

   LOG4C(( LOG_WARN, "执行命令[%s]\n", "" ));
   loadEnviorment( getValue("sysenv") );

   /*如果有 syscmd, 则先执行*/
   char *pczCmd = getValue( "syscmd" );
   if( pczCmd != NULL )
   {
      LOG4C(( LOG_WARN, "执行命令[%s]\n", pczCmd ));
      system( pczCmd );
   }

   char *pczFile = getValue( "exec" );
   /*检查可执行程序是否存在并且可执行*/
   if( !fileExistAndExec( pczFile ) )
   {
      LOG4C(( LOG_WARN, "配置的程序[%s]不存在或者不可执行\n", pczFile ));
      return -1;
   }

   char *pczPort = getValue( "port" );
   LOG4C(( LOG_DEBUG, "侦听端口 [%s]\n", pczPort ));

   /**
     * port  < 0 表示实际的处理进程为非socket通讯方式
     * 不用创建侦听端口
     */
   if (atoi (pczPort) > 0)
   {
       /*创建命令接收器*/
       int iRet = makeListener( &m_pstListener, m_pstPool, atoi(pczPort),
                                APR_INET, SOCK_STREAM );
       if( iRet == -1 )
       {
          LOG4C(( LOG_WARN, "在端口 %s 创建命令接收 socket 出错 [%s]\n",
                  pczPort, strerror(errno) ));
          return -1;
       }
   }
   else
   {
       m_pstListener = NULL;
   }

   m_pstProcess = (ProcessRegister*)shm_get( m_iProcShmKey, 0 );
   if( m_pstProcess == NULL)
   {
      LOG4C(( LOG_WARN, "连接进程管理共享内存(key=%d)失败[%s]\n",
              m_iProcShmKey, strerror(errno) ));
      return -1;
   }
   m_pstProcess += m_iMaxAllowProcess*m_iIndex;

   m_pstSystem = (SystemRegister*)shm_get( m_iGrpShmKey, 0 );
   if( m_pstProcess == NULL)
   {
      LOG4C(( LOG_WARN, "连接系统管理共享内存(key=%d)失败[%s]\n",
              m_iGrpShmKey, strerror(errno) ));
      return -1;
   }
   m_pstSystem += m_iIndex;

   m_pstSystem->m_tManagerPid = getpid();
   m_pstSystem->m_bActive = true;
   strcpy( m_pstSystem->m_aczName, m_aczName );
   m_pstSystem->m_iPort =  atoi(pczPort);
   m_pstSystem->m_iMinProcess = m_iMinProcess;
   m_pstSystem->m_iMaxProcess = m_iMaxProcess;

   if( m_iCfgShmKey == 0 )
   {
      return 0;
   }

   /*检测配置共享内存是否存在*/
   void *pCfgShm = shm_get( m_iCfgShmKey, 0 );
   if( m_pstProcess == NULL)
   {
      LOG4C(( LOG_WARN, "连接配置共享内存(key=%d)失败[%s]\n",
              m_iCfgShmKey, strerror(errno) ));
      return -1;
   }
   shmdt( pCfgShm );

   return 0;
}

/*
 *
 * Function: creatMoreChildren
 * Description:  检查进程是否都处于忙的状态 
 *               创建新进程总是从低位开始
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
bool AnyManager::creatMoreChildren( void )
{
   int iBusyProcess = 0, iCreateNum = 0, iIdx;
   float fBusyPercent;

   /*上半部用于检查是否要新增进程*/
   for( iIdx = 0; iIdx < m_iActiveProcess; iIdx++ )
   {
      if( m_pstProcess[iIdx].m_bDoTxn )
      {
         iBusyProcess++;
      }
   }

   fBusyPercent = iBusyProcess/m_iActiveProcess;
   if( fBusyPercent > 0.9 )
   {
      if( m_iActiveProcess < m_iMaxProcess )
      {
         iCreateNum = m_iActiveProcess/5;
      }
      if( iCreateNum + m_iActiveProcess >= m_iMaxProcess )
      {
         iCreateNum = m_iMaxProcess - m_iActiveProcess;
      }
      else if( iCreateNum == 0 )
      {
         iCreateNum = 1;
      }
   }


   /*创建更多的进程*/
   for( iIdx = 0; iIdx < iCreateNum; iIdx++ )
   {
      makeChild( iIdx + m_iActiveProcess);
   }

   m_iActiveProcess += iCreateNum;
   m_pstSystem->m_iActiveProcess = m_iActiveProcess;

   return iCreateNum>0;
}

/*
 *
 * Function: decreaseIdleChildren
 * Description:  减少空闲进程
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::decreaseIdleChildren( void )
{
   int     iIdx, iToStop;
   double  iIdle = 0;

   for( iIdx = 0; iIdx < m_iActiveProcess; iIdx++ )
   {
      if( !m_pstProcess[iIdx].m_bDoTxn )
      {
         iIdle++;
      }
   }

   /*如果空闲比例低于 20%, 则不减少*/
   if( iIdle/m_iActiveProcess < 0.2 )
   {
      return;
   }

   iToStop = (int)(iIdle/5);
   if( iToStop == 0 )
   {
      iToStop = 1;
   }

   if( m_iActiveProcess - iToStop < m_iMinProcess )
   {
      iToStop = m_iActiveProcess - m_iMinProcess;
   }

   int iTrueStop = 0;
   for( iIdx = 0; iIdx < iToStop; iIdx++ )
   {
      if( !m_pstProcess[m_iActiveProcess-iIdx-1].m_bDoTxn )
      {
         m_pstProcess[m_iActiveProcess-iIdx-1].m_bActive = false;
         if( m_pstProcess[m_iActiveProcess-iIdx-1].m_tPid > 0 )
         {
            kill( m_pstProcess[m_iActiveProcess-iIdx-1].m_tPid, 9 );
            m_pstProcess[m_iActiveProcess-iIdx-1].m_bDoTxn = false;
            m_pstProcess[m_iActiveProcess-iIdx-1].m_tPid = 0;
            m_pstProcess[m_iActiveProcess-iIdx-1].m_iRunTimes = 0;
         }
         iTrueStop++;
      }
   }

   m_iActiveProcess -= iTrueStop;

   m_pstSystem->m_iActiveProcess = m_iActiveProcess;
}

/*
 *
 * Function: checkExitedChildren
 * Description:  检查不正常退出的进程并恢复
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::checkExitedChildren( bool bStoping )
{
   int iExitCode, iIdx;
   apr_exit_why_e tWhy;
   apr_status_t tStatus;
   apr_proc_t   stProcess;
   bool abRebuild[1024];

   for( iIdx = 0; iIdx < m_iActiveProcess; iIdx++ )
   {
      abRebuild[iIdx] = false;
   }

   /*处理是否有退出的进程*/
   do
   {
      tStatus = apr_proc_wait_all_procs( &stProcess, &iExitCode, &tWhy,
                                         APR_NOWAIT, m_pstMaintanence );
      if( stProcess.pid > 0 && !APR_STATUS_IS_EINTR(tStatus) )
      {
         /*设置进程状态为恢复*/
         for( iIdx = 0; iIdx < m_iActiveProcess; iIdx++ )
         {
            if( m_pstProcess[iIdx].m_tPid == stProcess.pid &&
                m_pstProcess[iIdx].m_bActive )
            {
               LOG4C(( LOG_DEBUG, "退出进程 pid %d at %d\n",
                       stProcess.pid, iIdx ));
               if( !bStoping )
               {
                  abRebuild[iIdx] = true;
               }
               else
               {
                  m_pstSystem->m_iActiveProcess--;
               }
            }
         }
      }
      else
      {
         break;
      }
   } while( true );

   if( bStoping )
   {
      return;
   }

   /*将所有待恢复的进程创建新的子进程*/
   for( iIdx = 0; iIdx < m_iActiveProcess; iIdx++ )
   {
      if( abRebuild[iIdx] )
      {
         LOG4C(( LOG_INFO, "在 %d 重建子进程\n", iIdx ));
         makeChild( iIdx );
      }
   }
}

/*
 *
 * Function: killTimeoutChildren
 * Description:   检查超时的进程, 终止后再重启
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::killTimeoutChildren( void )
{
   bool abRebuild[256];
   char *pczValue;
   int iIdx;
   apr_time_t tTimeOut;
   apr_time_t tNow;

   pczValue = getValue( (const char*)"timeout" );
   if( pczValue == NULL )
   {
      return;
   }
   tTimeOut = atoi(pczValue);
   if( tTimeOut <= 0 )
   {
      return;
   }

   tNow = apr_time_sec (apr_time_now());

   for( iIdx = 0; iIdx < m_iActiveProcess; iIdx++ )
   {
      if( m_pstProcess[iIdx].m_bDoTxn &&
          (m_pstProcess[iIdx].m_tPid > 0) &&
          ((tNow - m_pstProcess[iIdx].m_tBegin) > tTimeOut) )
      {
         LOG4C(( LOG_WARN, "进程 %d 超时, 强行中止 %ld %ld %ld\n",
               m_pstProcess[iIdx].m_tPid, tNow, m_pstProcess[iIdx].m_tBegin, tTimeOut ));
         kill( m_pstProcess[iIdx].m_tPid, 9 );
         abRebuild[iIdx] = true;
      }
      else
      {
         abRebuild[iIdx] = false;
      }
   }

   for( iIdx = 0; iIdx < m_iActiveProcess; iIdx++ )
   {
      if( abRebuild[iIdx] )
      {
         makeChild( iIdx );
      }
   }
}

/*
 *
 * Function: doStopCmd
 * Description:  处理停止子服务命令 
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::doStopCmd( void )
{
   char *pczValue;
   int iTimeOut;
   int iStopedCount = 0;
   apr_time_t tNow;

   pczValue = getValue( (const char*)"timeout" );
   if( pczValue == NULL )
   {
      iTimeOut = 30;
   }
   iTimeOut = atoi(pczValue);
   if( iTimeOut <= 0 )
   {
      iTimeOut = 30;
   }
   iTimeOut *= 1000000;

   tNow = apr_time_now();
   for( int i = 0; i < m_iActiveProcess; i++ )
   {
      if( m_pstProcess[i].m_bActive )
      {
         if( !m_pstProcess[i].m_bDoTxn )
         {
            kill( m_pstProcess[i].m_tPid, 9 );
            m_pstSystem->m_iActiveProcess--;
         }
         else
         {
            if( (tNow - m_pstProcess[i].m_tBegin) > iTimeOut )
            {
               kill( m_pstProcess[i].m_tPid, 9 );
               m_pstSystem->m_iActiveProcess--;
            }
         }
         m_pstProcess[i].m_bActive = false;
      }
   }
}

/*
 *
 * Function: doLoadCfgCmd
 * Description:  处理加载服务配置命令 
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::doLoadCfgCmd( void )
{
   for( int i = 0; i < m_iActiveProcess; i++ )
   {
      m_pstProcess[i].m_tCommand = LOAD_CONFIG;
   }
}

static int giCommand = 0;
const static int STOP_CMD = 0x01;
const static int RELOAD_CMD = 0x10;

/*
 *
 * Function: doCommand
 * Description:  处理命令 
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::doCommand( void )
{
   /*支持 stop loadcfg*/
   if( giCommand&STOP_CMD )
   {
      doStopCmd( );
   }
   else if( giCommand&RELOAD_CMD )
   {
      doLoadCfgCmd( );
   }
}

/*
 *
 * Function: sig_usr1
 * Description:  处理命令用户自定义信号1 用于停机信号
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void sig_usr1( int iSignal )
{
   giCommand |= STOP_CMD;
   signal( SIGUSR1, sig_usr1 );
}

/*
 *
 * Function: sig_usr2
 * Description:  处理命令用户自定义信号2 用于重新连接配置共享内存
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void sig_usr2( int iSignal )
{
   giCommand |= RELOAD_CMD;
   signal( SIGUSR2, sig_usr2 );
}

/*
 *
 * Function: maintenance
 * Description:  接收系统管理器命令 
 *               如果没有命令时每秒检查一次系统
 *               决定是否创建新进程或者减少进程
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::maintenance( void )
{
   bool bActive = true;
   bool bStoping = false;

   m_pstSystem->m_iActiveProcess = m_iActiveProcess;
   do
   {
      apr_pool_clear( m_pstMaintanence );

      /*如果有命令则处理*/
      if( giCommand&STOP_CMD )
      {
         bStoping = true;
         giCommand = 0;
      }
      else if( giCommand&RELOAD_CMD )
      {
         doLoadCfgCmd( );
      }

      if( bStoping )
      {
         doStopCmd( );
      }

      /*LOG4C(( LOG_INFO, "开始超时检查\n" ));*/
      if( !bStoping )
      {
         killTimeoutChildren( );

         /*LOG4C(( LOG_INFO, "开始检查是否需要更多进程\n" ));*/
         if( !creatMoreChildren( ) )
         {
            /*LOG4C(( LOG_INFO, "开始检查是否需要减少进程\n" ));*/
            decreaseIdleChildren();
         }
      }

      /*LOG4C(( LOG_INFO, "开始检查是否有退出进程\n" ));*/
      checkExitedChildren( bStoping );

      if( bStoping && m_pstSystem->m_iActiveProcess == 0 )
      {
         m_pstSystem->m_bActive = false;
         break;
      }
      /*每两次轮巡之间间隔 0.5 秒*/
      apr_sleep( 500000 );
   } while( bActive );
}

/*
 *
 * Function: run
 * Description:  创建足够多的子进程后进入维护模式
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
int AnyManager::run( void )
{
   char *pczValue;
   pid_t tPid;

   pczValue = getValue( "minproc" );
   m_iMinProcess = atoi(pczValue);
   pczValue = getValue( "maxproc" );
   m_iMaxProcess = atoi(pczValue);

   if( m_iMaxProcess > m_iMaxAllowProcess )
   {
      m_iMaxProcess = m_iMaxAllowProcess;
   }

   if( m_iMinProcess > m_iMaxProcess )
   {
      m_iMinProcess = m_iMaxProcess;
   }

   if( m_iMinProcess < 2 )
   {
      /*m_iMinProcess = 2;*/
   }

   LOG4C(( LOG_INFO, "系统 %s 最小进程数 %d 最大进程数 %d\n",
           m_aczName, m_iMinProcess, m_iMaxProcess ));

   /*以下是子系统管理进程创建业务进程*/
   for( int i = 0; i < m_iMinProcess; i++ )
   {
      makeChild( i );
   }

   m_iActiveProcess = m_iMinProcess;
   m_pstSystem->m_iActiveProcess = m_iMinProcess;

   maintenance();

   return 0;
}

/*
 *
 * Function: setupSignal
 * Description:  屏蔽系统处理信号
 * Input: 
 * Output:
 * Return:
 * Other:
 *
 */
void AnyManager::setupSignal( void )
{
   apr_signal( SIGTERM, SIG_IGN );
   apr_signal( SIGINT,  SIG_IGN );
   apr_signal( SIGPIPE, SIG_IGN );
   apr_signal( SIGHUP,  SIG_IGN );
   apr_signal( SIGTTOU, SIG_IGN );
   apr_signal( SIGTTIN, SIG_IGN );
   apr_signal( SIGTSTP, SIG_IGN );
}

/*
 *
 * Function: main
 * Description: 应用数据中心主函数开始
 * 参数:
 *      name        -   子系统名字
 *      runrlt      -   运行次数,-1表示永不退出
 *      procshmkey  -   进程管理信息共享内存 KEY
 *      cfgshmkey   -   配置信息共享内存 KEY
 *      maxchild    -   每个系统最大子进程数
 *      index      -   节点编号
 * Return:
 * Other:
 */

int main( int argc, char *argv[] )
{
   char aczName[128];
   int  iRunRlt, iGrpShmKey, iProcShmKey, iIndex, iMaxChild;

   if( argc != 7 )
   {
      printf( "Usage: %s name fd grpshmkey procshmkey index maxchild\n",
              argv[0] );
      return -1;
   }

   strcpy( aczName, argv[1] );
   iRunRlt = atoi( argv[2] );
   iGrpShmKey = atoi( argv[3] );
   iProcShmKey = atoi( argv[4] );
   iIndex = atoi( argv[5] );
   iMaxChild = atoi( argv[6] );

   iCmdFd = iRunRlt;

   signal( SIGUSR1, sig_usr1 );
   signal( SIGUSR2, sig_usr2 );

   AnyManager anyManager( aczName, iIndex, iMaxChild );
   /*printf("after AnyManager construct\n");*/
   if( anyManager.loadConfig() == -1 )
   {
      write( iRunRlt, "1配置失败", 9 );
      close( iRunRlt );
      return -1;
   }
   /*printf("after AnyManager.loadConfig \n");*/

   if( anyManager.prepare( iGrpShmKey, iProcShmKey ) == -1 )
   {
      write( iRunRlt, "1准备运行失败", 13 );
      close( iRunRlt );
      return -1;
   }

   write( iRunRlt, "0开始运行", 9 );
   if( iRunRlt > 2 )
   {
      close( iRunRlt );
   }

   anyManager.run();

   return 0;
}

