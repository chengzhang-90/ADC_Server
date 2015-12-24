/*****************************************************************
*
*    Copyright (C), 2010-2020, NewLand Computer Co., Ltd.
*    Version:  VER1.0
*    Description: 系统整体调用主控程序
*    History:
*
*    Function List:
*            recvHead : 根据CMP平台发送的数据头确认需要调用的函数名 
*            matchFunName: 根据服务名,匹配该服务的ID
*            sendDataSize: 发送到CMP平台报文长度-定义为10个字节的长度
*            sendData: 发送到CMP平台报文
*            recvDataSize: 接收CMP平台发送来的报文长度-定义为10个字节的长度
*            recvData: 接收CMP平台报文
*            run: 运行可执行程序
*            atExitFun:进程处理完成退出 
*            main:主控程序开始 
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
#include <unistd.h>
#include "adc_base.h"
#include "adc_handle.h"
#include "udo.h"
#include "log.h"
#include "agm_call.h"
#include "pub_base.h"
#include "tool_base.h"
#include "adc_global.h"

extern AgmFunManager stAgmFunManager;

AdpTxnPara stTxnPara;

apr_pool_t *pstGlobalPool;
apr_pool_t *pstLocalPool;
apr_socket_t *pstClient;

char *pcShmAddr = NULL; /*系统一个全局变量 */

/*
 *
 * Function: recvHead
 * Description: 根据CMP平台发送的数据头确认需要调用的函数名 
 * Input:
 *         pstClient - 通讯连接字 
 * Output:
 *         pczFunName - 函数名(64) 
 *         pczTranCode - 交易代码(20)
 *
 * Return:
 *         -1 - 通讯失败
 *         >=0  通讯超时时间
 * Other:
 *
 */
int recvHead( apr_socket_t *pstClient, char *pczFunName, char *pczTranCode )
{
   char aczRecvBuff[90];
   struct log4c_logInfo logInfoChnl;

   apr_size_t tLen = 89;

   if( apr_socket_recv( pstClient, aczRecvBuff, &tLen ) != APR_SUCCESS )
   {
  //    LOG4C(( LOG_FATAL, "接收函数名失败%s\n", strerror(errno) ));
  
 	 log4c_chgIntertag (__FILE__, __FUNCTION__, __LINE__);
  	log4c(LOG_FATAL, "接收函数名失败%s\n", strerror(errno));
      return -1;
   }
   if( tLen < 89 )
   {
      LOG4C(( LOG_FATAL, "接收函数名字符串长度不足\n" ));
      return -1;
   }

   tLen = sizeof (logInfoChnl);
   memset (&logInfoChnl, 0, tLen);
   if( ap_tcp_read (pstClient, (char *)&logInfoChnl, tLen ) != 0)
   {
      LOG4C(( LOG_FATAL, "接收渠道日志信息失败\n" ));
      return -1;
   }

   aczRecvBuff[89] = 0;
   LOG4C(( LOG_INFO, "接收报文头[%s],长度[%d]\n",aczRecvBuff,strlen(aczRecvBuff) ));
   /* 交易头由三部分组成: 函数名(64) + 交易代码(20) + 超时时间(5)*/
   memcpy( pczFunName, aczRecvBuff, 64 );
   memcpy( pczTranCode, &aczRecvBuff[64], 20 );

   return atoi(&aczRecvBuff[84]);
}

/*
 *
 * Function: matchFunName
 * Description: 根据服务名,匹配该服务的ID
 * Input:
 *         pczFunName - 服务名
 * Output:
 *
 * Return:
 *         -1 - 查找该服务ID出错
 *         >=0  该服务ID
 * Other:
 *
 */
int matchFunName( char *pczFunName )
{

   for( int i = 0; i < stAgmFunManager.m_Num; i++ )
   {
      if( strcmp( pczFunName, stAgmFunManager.m_Register[i].m_Name ) == 0 )
      {
         return i;
      }
   }

   return -1;
}

/*
 *
 * Function: sendDataSize
 * Description: 发送到CMP平台报文长度-定义为10个字节的长度
 * Input:
 *         pstClient - 通讯连接字 
 *         iDataLen -  要应答给CMP平台的数据长度
 * Output:
 *
 * Return:
 *         -1 - 通讯失败
 *         0  - SUCC
 * Other:
 *
 */
int sendDataSize( apr_socket_t *pstClient, int iDataLen )
{
   char aczLen[11];
   apr_size_t tLen = 10;

   sprintf( aczLen,"%010d", iDataLen );
   return apr_socket_send( pstClient, aczLen, &tLen );
}

/*
 *
 * Function: sendData
 * Description: 发送到CMP平台报文
 * Input:
 *         pstClient - 通讯连接字 
 *         pczSendBuf -要发送的数据 
 *         iDataLen -  要应答给CMP平台的数据长度
 * Output:
 *
 * Return:
 *         -1 - 通讯失败
 *         0  - SUCC
 * Other:
 *
 */
int sendData( apr_socket_t *pstClient, char *pczSendBuff, int iDataLen )
{
   apr_size_t tLen = (apr_size_t)iDataLen;
   int iSendedData = 0;

   do
   {
      tLen = iDataLen - iSendedData;
      if( apr_socket_send( pstClient, pczSendBuff+iSendedData, &tLen )
          != APR_SUCCESS )
      {
         LOG4C(( LOG_WARN, "发送数据失败[%s]\n", strerror(errno) ));
         return -1;
      }
      LOG4C(( LOG_DEBUG, "发送块长度 %d\n", tLen ));
      iSendedData += tLen;
   } while( iSendedData < iDataLen );

   return (int)tLen;
}

/*
 *
 * Function: recvDataSize
 * Description: 接收CMP平台发送来的报文长度-定义为10个字节的长度
 * Input:
 *         pstClient - 通讯连接字 
 * Output:
 *
 * Return:
 *         -1 - 通讯失败
 *         >=0  数据长度
 * Other:
 *
 */
int recvDataSize( apr_socket_t *pstClient )
{
   char aczDataLen[11];
   apr_size_t tLen;

   memset( aczDataLen, 0, sizeof(aczDataLen) );
   tLen = sizeof(aczDataLen) - 1;
   if( apr_socket_recv( pstClient, aczDataLen, &tLen ) != APR_SUCCESS )
   {
      LOG4C(( LOG_WARN, "接收数据长度失败[%s]\n", strerror(errno) ));
      return -1;
   }
   tLen = 4;
   if( apr_socket_send( pstClient, "0000", &tLen ) != APR_SUCCESS )
   {
      LOG4C(( LOG_WARN, "返回数据长度确认失败[%s]\n", strerror(errno) ));
      return -1;
   }

   return atoi( aczDataLen );
}

/*
 *
 * Function: recvData
 * Description: 接收CMP平台报文
 * Input:
 *         pstClient - 通讯连接字 
 *         iDataLen -  数据长度
 * Output:
 *         pczRecvBuff -接收的数据 
 *
 * Return:
 *         -1 - 通讯失败
 *         0  - SUCC
 * Other:
 *
 */
int recvData( apr_socket_t *pstClient, char *pczRecvBuff, int iDataLen )
{
   apr_size_t tLen = (apr_size_t)iDataLen;
   int iRecvedLen = 0;

   do
   {
      tLen = iDataLen - iRecvedLen;
      if( apr_socket_recv( pstClient, pczRecvBuff+iRecvedLen, &tLen ) !=
          APR_SUCCESS )
      {
         printf( "data [%s]\n", pczRecvBuff );
         LOG4C(( LOG_WARN, "接收数据失败[%s]已接收数据 %d\n",
                 strerror(errno), iRecvedLen ));
         return -1;
      }
      iRecvedLen += tLen;
   } while( iRecvedLen < iDataLen );

   return (int)iDataLen;
}

extern char __logFileName[128];
extern FILE *__logFile;
extern char  __SYSTEM_LOG_FLAG__[100];
extern int   __can_log_file__;

/*
 *
 * Function: run
 * Description: 运行可执行程序
 * Input:
 *         pstProcess - 进行管理器 
 *         pstListener - 该进程监听链接字 
 *         iMaxRunTimes - 该进程运行最大次数,-1永不退出 
 *         iNodeId - 该进程的节点号 
 *         pczExecFile - 该进程对应的可执行文件 
 * Output:
 *
 * Return:

 * Other:
 *
 */
void run( ProcessRegister *pstProcess, apr_socket_t *pstListener,
          int iMaxRunTimes, int iNodeId, char *pczExecFile )
{
   char aczFunName[65], aczLogFile[128], aczTranCode[21];
   char aczRecvBuff[20480], *pczRecvBuff;
   char aczSendBuff[20480], *pczSendBuff;
   int  iRecvLen, iSendLen, iRet;
   int iErrorCount = 0, iDataLen, iFunId, iTimeOut;
   unsigned int iAllowRunTimes, iRunTimes = 0;

   apr_size_t tLen;
   UDP pstReq, pstRsp;

   FILE *systemLogFile;

   stTxnPara.m_iNodeId = iNodeId;

   /* 设置为阻塞式等待*/
   apr_socket_timeout_set( pstListener, -1 );
   if( iMaxRunTimes <= 0 )
   {
      iAllowRunTimes = (unsigned int)-1;
   }
   else
   {
      iAllowRunTimes = iMaxRunTimes;
   }

   while( pstProcess->m_bActive && iRunTimes < iAllowRunTimes )
   {
      apr_pool_clear( pstLocalPool );
      pstProcess->m_bDoTxn = false;
      if( apr_socket_accept( &pstClient, pstListener, pstLocalPool ) !=
          APR_SUCCESS )
      {
         iErrorCount++;
         if( iErrorCount == 10 )
         {
            LOG4C(( LOG_FATAL, "连续10次 accept 出错 %s\n", strerror(errno) ));
            return;
         }
         continue;
      }

      LOG4C_CLOSE();
      LOG4C_OPEN();

      iErrorCount = 0;

      pstProcess->m_tBegin = apr_time_sec(apr_time_now());
      pstProcess->m_bDoTxn = true;

      memset( aczFunName, 0, sizeof(aczFunName) );
      memset( aczTranCode, 0, sizeof(aczTranCode) );
      if( (iTimeOut = recvHead( pstClient, aczFunName, aczTranCode )) == -1 )
      {
         LOG4C(( LOG_WARN, "接收服务名出错 %s\n", strerror(errno) ));
         continue;
      }
      PUB_StrTrim( aczFunName, ' ' );
      PUB_StrTrim( aczTranCode, ' ' );
      LOG4C(( LOG_DEBUG, "请求的函数名(%s)交易代码(%s)超时时间%d\n",
              aczFunName, aczTranCode, iTimeOut ));
      tLen = 4;
      LOG4C(( LOG_DEBUG, "1111111111\n"));
      if( (iFunId = matchFunName( aczFunName )) == -1 )
      {
         LOG4C(( LOG_WARN, "查找服务名[%s]出错 %s\n",aczFunName,
                  strerror(errno) ));
         apr_socket_send( pstClient, "0001", &tLen );
         apr_socket_close( pstClient );
         continue;
      }
      if( apr_socket_send( pstClient, "0000", &tLen ) != APR_SUCCESS )
      {
         LOG4C(( LOG_WARN, "发送函数名响应出错 %s\n", strerror(errno) ));
         apr_socket_close( pstClient );
         continue;
      }

      pstReq = UDO_Alloc( 20480 );
      pstRsp = UDO_Alloc( 20480 );

      strcpy( stTxnPara.m_aczTranCode, aczTranCode );
      stTxnPara.m_iTimeOut = iTimeOut;
      stTxnPara.m_iChangeReverseMode = REVERSE_NO_CHANGE;
      memset (stTxnPara.m_aczErrorMessage, 0, sizeof (stTxnPara.m_aczErrorMessage));

      if( (iDataLen = recvDataSize( pstClient )) == -1 )
      {
         LOG4C(( LOG_WARN, "接收数据长度出错 %s\n", strerror(errno) ));
         goto TX_END_PRE;
      }

      LOG4C(( LOG_DEBUG, "接收的数据长度(%d)\n", iDataLen ));
      pczRecvBuff = aczRecvBuff;
      if( iDataLen > sizeof(aczRecvBuff) )
      {
         pczRecvBuff = (char*)apr_palloc( pstLocalPool, iDataLen+1 );
         memset( pczRecvBuff, 0, iDataLen+1 );
         LOG4C(( LOG_DEBUG, "原数据空间不够,重新分配\n" ));
      }

      if( recvData( pstClient, pczRecvBuff, iDataLen ) != iDataLen )
      {
         LOG4C(( LOG_WARN, "接收数据长度出错 %s\n", strerror(errno) ));
         goto TX_END_PRE;
      }

      if( onTxStart( pczExecFile ) == -1 )
      {
         LOG4C(( LOG_FATAL, "交易开始处理函数失败%s\n", strerror(errno) ));
         goto TX_END_PRE;
      }

      UDO_sscanf( pczRecvBuff, iDataLen, pstReq );

      LOG4C(( LOG_DEBUG, "调用的函数下标%d\n", iFunId ));

      LOG4C_CLOSE();
      LOG4C_CHGSUFFIX (aczFunName);
      LOG4C_OPEN();

      LOG4C(( LOG_DEBUG, "-----------------开始运行程序[%s]---------------------",stAgmFunManager.m_Register[iFunId].m_Name));
      LOG4C(( LOG_DEBUG, "----------------------------------------------------------------" ));
      if( LOG4C_ISLOG (LOG_DEBUG) )
      {
         UDO_FilePrint( pstReq, LOG4C_FP() );
      }
      iRet = (*stAgmFunManager.m_Register[iFunId].m_Func)( pstReq, pstRsp );
      LOG4C(( LOG_DEBUG, "准备应对数据"));
      if( LOG4C_ISLOG (LOG_DEBUG) )
      {
         UDO_FilePrint( pstRsp, LOG4C_FP() );
      }
      LOG4C(( LOG_DEBUG, "--------------------程序运行结束--------------------------------" ));
      LOG4C(( LOG_DEBUG, "----------------------------------------------------------------\n" ));

      /**
        * 恢复日志信息为原状态 
        **/
      LOG4C_CLOSE();
      LOG4C_CHGSUFFIX ("");
      LOG4C_OPEN();

      LOG4C((LOG_DEBUG, "END   call function %s\n", aczFunName ));

      UDO_AddField( pstRsp, -100, UDT_INT, (char*)&iRet, 0 );
      UDO_AddField( pstRsp, -101, UDT_STRING, stTxnPara.m_aczErrorMessage,
                    strlen(stTxnPara.m_aczErrorMessage) );
      UDO_AddField( pstRsp, -102, UDT_INT,
                    (char*)&stTxnPara.m_iChangeReverseMode, 0 );
      iDataLen = UDO_ValueSize( pstRsp );

      pczSendBuff = aczSendBuff;
      if( iDataLen > sizeof(aczSendBuff) )
      {
         pczSendBuff = (char*)apr_palloc( pstLocalPool, iDataLen );
      }
      UDO_sprintf( pstRsp, pczSendBuff, &iDataLen );

      if( sendDataSize( pstClient, iDataLen ) == -1 )
      {
         LOG4C(( LOG_WARN, "发送返回数据长度失败(%s)\n", strerror(errno) ));
         goto TX_END;
      }

      if( sendData( pstClient, pczSendBuff, iDataLen ) == -1 )
      {
         LOG4C(( LOG_WARN, "发送返回数据失败(%s)\n", strerror(errno) ));
      }

      iRunTimes++;

TX_END:
      onTxEnd();

TX_END_PRE:
      apr_socket_close( pstClient );

      pstProcess->m_iRunTimes = iRunTimes;

      UDO_Free( pstReq );
      UDO_Free( pstRsp );
   }
}

/*
 *
 * Function: atExitFun
 * Description:进程处理完成退出 
 * Return:
 * Other:
 */
void atExitFun( void )
{
   onSysEnd();
   apr_pool_destroy( pstGlobalPool );
   apr_terminate();

   LOG4C_CLOSE();
}

/*
 *
 * Function: sig_usr1
 * Description:响应用户自定义信号 
 * Return:
 * Other:
 */
void sig_usr1( int iSignal )
{
   exit(0);
}

/*
 *
 * Function: setupSignal
 * Description:屏蔽信号 
 * Return:
 * Other:
 */
void setupSignal( void )
{
   apr_signal( SIGTERM, SIG_IGN );
   apr_signal( SIGINT,  SIG_IGN );
   apr_signal( SIGPIPE, SIG_IGN );
   apr_signal( SIGHUP,  SIG_IGN );
   apr_signal( SIGCHLD, SIG_IGN );
   apr_signal( SIGTTOU, SIG_IGN );
   apr_signal( SIGTTIN, SIG_IGN );
   apr_signal( SIGTSTP, SIG_IGN );
}

/*
 *
 * Function: main
 * Description: 应用数据中心主函数开始
 * 参数:
 *      name socket procshmkey cfgshmkey maxchild index offset loginit maxruntimes nodeid
 *      name        -   子系统名字
 *      socket      -   已经打开的文件描述符
 *      procshmkey  -   进程管理信息共享内存 KEY
 *      cfgshmkey   -   配置信息共享内存 KEY
 *      maxchild    -   每个系统最大子进程数
 *      index       -   当前子系统在系统中的索引
 *      offset      -   当前子进程在子系统中的偏移量
 *      loginit     -   日志初始化文件
 *      maxruntimes -   当前子进程最大运行次数
 *      nodeid      -   节点编号
 * Return:
 * Other:
 */

int main( int argc, char *argv[] )
{
   char aczName[64], aczLogIni[128];
   int iSocket, iProcShmKey, iCfgShmKey, iMaxChild;
   int iIndex, iOffset, iMaxRunTimes, iNodeId;
   ProcessRegister *pstProcess;
   apr_socket_t *pstListener;
   apr_os_sock_info_t stSockInfo;
   
#ifdef AF_CCITT
  int tSockLen;
#else
  size_t tSockLen;
#endif

   if( argc != 11 )
   {
      printf( "input %d para: ", argc );
      for( int i = 0; i < argc; i++ )
      {
         printf( "[%s]", argv[i] );
      }
      printf( "\n" );
      printf( "Usage: %s name socket prcshmkey cfgshmkey maxchild index "
              "offset loginit maxruntims nodeid\n", argv[0] );fflush(stdout);
      return -1;
   }


   strcpy( aczName, argv[1] );
   iSocket = atoi( argv[2] );
   iProcShmKey = atoi( argv[3] );
   iCfgShmKey  = atoi( argv[4] );
   iMaxChild   = atoi( argv[5] );
   iIndex      = atoi( argv[6] );
   iOffset     = atoi( argv[7] );
   strcpy( aczLogIni, argv[8] );
   iMaxRunTimes = atoi( argv[9] );
   iNodeId      = atoi( argv[10] );

   LOG4C_INIT( aczLogIni );
   LOG4C_OPEN();

   apr_initialize();
   atexit( atExitFun );

   apr_pool_create( &pstGlobalPool, NULL );
   apr_pool_create( &gpstGlobalVariablePool, pstGlobalPool );
   apr_pool_create( &pstLocalPool, NULL );

   setupSignal();

   pstProcess = (ProcessRegister*)shm_get( iProcShmKey, 0 );
   if( pstProcess == NULL )
   {
      printf( "连接进程管理共享内存%d失败%s\n",
              iProcShmKey, strerror(errno) );
      return -1;
   }
   pstProcess += iMaxChild*iIndex + iOffset;

   if( iCfgShmKey > 0 )
   {
      pcShmAddr = (char*)shm_get( iCfgShmKey, 0 );
      if( pcShmAddr == NULL )
      {
         printf( "连接配置信息共享内存%d失败%s\n",
                 iCfgShmKey, strerror(errno) );
         return -1;
      }
   }


   /*signal( SIGUSR1, sig_usr1 );*/

   /*if( onSysStart( argv[0] ) < 0 )*/
   /* zhd 20100907 */
   if( onSysStart( argv[0] ) != 0 )
   {
      printf( "onSysStart error\n" );
      return -1;
   }

   stSockInfo.os_sock = &iSocket;
   tSockLen = sizeof(struct sockaddr);
   stSockInfo.local = (sockaddr *)apr_palloc( pstGlobalPool, tSockLen );
   getsockname( iSocket, stSockInfo.local, (socklen_t *)&tSockLen );
   stSockInfo.remote  = (sockaddr *)apr_palloc( pstGlobalPool, tSockLen );
   getsockname( iSocket, stSockInfo.remote, (socklen_t *)&tSockLen );
   stSockInfo.family  = APR_INET;
   stSockInfo.type    = SOCK_STREAM;

   if( apr_os_sock_make( &pstListener, &stSockInfo, pstGlobalPool ) != APR_SUCCESS )
   {
      printf( "恢复socket %d失败%s", iSocket, strerror(errno) );
      return -1;
   }


   run( pstProcess, pstListener, iMaxRunTimes, iNodeId, argv[0] );

   return 0;
}
