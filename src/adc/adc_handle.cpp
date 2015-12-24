/*****************************************************************
*
*    Copyright (C), 2010-2020, NewLand Computer Co., Ltd.
*    Version:  VER1.0
*    Description: ϵͳ����������س���
*    History:
*
*    Function List:
*            recvHead : ����CMPƽ̨���͵�����ͷȷ����Ҫ���õĺ����� 
*            matchFunName: ���ݷ�����,ƥ��÷����ID
*            sendDataSize: ���͵�CMPƽ̨���ĳ���-����Ϊ10���ֽڵĳ���
*            sendData: ���͵�CMPƽ̨����
*            recvDataSize: ����CMPƽ̨�������ı��ĳ���-����Ϊ10���ֽڵĳ���
*            recvData: ����CMPƽ̨����
*            run: ���п�ִ�г���
*            atExitFun:���̴�������˳� 
*            main:���س���ʼ 
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

char *pcShmAddr = NULL; /*ϵͳһ��ȫ�ֱ��� */

/*
 *
 * Function: recvHead
 * Description: ����CMPƽ̨���͵�����ͷȷ����Ҫ���õĺ����� 
 * Input:
 *         pstClient - ͨѶ������ 
 * Output:
 *         pczFunName - ������(64) 
 *         pczTranCode - ���״���(20)
 *
 * Return:
 *         -1 - ͨѶʧ��
 *         >=0  ͨѶ��ʱʱ��
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
  //    LOG4C(( LOG_FATAL, "���պ�����ʧ��%s\n", strerror(errno) ));
  
 	 log4c_chgIntertag (__FILE__, __FUNCTION__, __LINE__);
  	log4c(LOG_FATAL, "���պ�����ʧ��%s\n", strerror(errno));
      return -1;
   }
   if( tLen < 89 )
   {
      LOG4C(( LOG_FATAL, "���պ������ַ������Ȳ���\n" ));
      return -1;
   }

   tLen = sizeof (logInfoChnl);
   memset (&logInfoChnl, 0, tLen);
   if( ap_tcp_read (pstClient, (char *)&logInfoChnl, tLen ) != 0)
   {
      LOG4C(( LOG_FATAL, "����������־��Ϣʧ��\n" ));
      return -1;
   }

   aczRecvBuff[89] = 0;
   LOG4C(( LOG_INFO, "���ձ���ͷ[%s],����[%d]\n",aczRecvBuff,strlen(aczRecvBuff) ));
   /* ����ͷ�����������: ������(64) + ���״���(20) + ��ʱʱ��(5)*/
   memcpy( pczFunName, aczRecvBuff, 64 );
   memcpy( pczTranCode, &aczRecvBuff[64], 20 );

   return atoi(&aczRecvBuff[84]);
}

/*
 *
 * Function: matchFunName
 * Description: ���ݷ�����,ƥ��÷����ID
 * Input:
 *         pczFunName - ������
 * Output:
 *
 * Return:
 *         -1 - ���Ҹ÷���ID����
 *         >=0  �÷���ID
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
 * Description: ���͵�CMPƽ̨���ĳ���-����Ϊ10���ֽڵĳ���
 * Input:
 *         pstClient - ͨѶ������ 
 *         iDataLen -  ҪӦ���CMPƽ̨�����ݳ���
 * Output:
 *
 * Return:
 *         -1 - ͨѶʧ��
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
 * Description: ���͵�CMPƽ̨����
 * Input:
 *         pstClient - ͨѶ������ 
 *         pczSendBuf -Ҫ���͵����� 
 *         iDataLen -  ҪӦ���CMPƽ̨�����ݳ���
 * Output:
 *
 * Return:
 *         -1 - ͨѶʧ��
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
         LOG4C(( LOG_WARN, "��������ʧ��[%s]\n", strerror(errno) ));
         return -1;
      }
      LOG4C(( LOG_DEBUG, "���Ϳ鳤�� %d\n", tLen ));
      iSendedData += tLen;
   } while( iSendedData < iDataLen );

   return (int)tLen;
}

/*
 *
 * Function: recvDataSize
 * Description: ����CMPƽ̨�������ı��ĳ���-����Ϊ10���ֽڵĳ���
 * Input:
 *         pstClient - ͨѶ������ 
 * Output:
 *
 * Return:
 *         -1 - ͨѶʧ��
 *         >=0  ���ݳ���
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
      LOG4C(( LOG_WARN, "�������ݳ���ʧ��[%s]\n", strerror(errno) ));
      return -1;
   }
   tLen = 4;
   if( apr_socket_send( pstClient, "0000", &tLen ) != APR_SUCCESS )
   {
      LOG4C(( LOG_WARN, "�������ݳ���ȷ��ʧ��[%s]\n", strerror(errno) ));
      return -1;
   }

   return atoi( aczDataLen );
}

/*
 *
 * Function: recvData
 * Description: ����CMPƽ̨����
 * Input:
 *         pstClient - ͨѶ������ 
 *         iDataLen -  ���ݳ���
 * Output:
 *         pczRecvBuff -���յ����� 
 *
 * Return:
 *         -1 - ͨѶʧ��
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
         LOG4C(( LOG_WARN, "��������ʧ��[%s]�ѽ������� %d\n",
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
 * Description: ���п�ִ�г���
 * Input:
 *         pstProcess - ���й����� 
 *         pstListener - �ý��̼��������� 
 *         iMaxRunTimes - �ý�������������,-1�����˳� 
 *         iNodeId - �ý��̵Ľڵ�� 
 *         pczExecFile - �ý��̶�Ӧ�Ŀ�ִ���ļ� 
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

   /* ����Ϊ����ʽ�ȴ�*/
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
            LOG4C(( LOG_FATAL, "����10�� accept ���� %s\n", strerror(errno) ));
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
         LOG4C(( LOG_WARN, "���շ��������� %s\n", strerror(errno) ));
         continue;
      }
      PUB_StrTrim( aczFunName, ' ' );
      PUB_StrTrim( aczTranCode, ' ' );
      LOG4C(( LOG_DEBUG, "����ĺ�����(%s)���״���(%s)��ʱʱ��%d\n",
              aczFunName, aczTranCode, iTimeOut ));
      tLen = 4;
      LOG4C(( LOG_DEBUG, "1111111111\n"));
      if( (iFunId = matchFunName( aczFunName )) == -1 )
      {
         LOG4C(( LOG_WARN, "���ҷ�����[%s]���� %s\n",aczFunName,
                  strerror(errno) ));
         apr_socket_send( pstClient, "0001", &tLen );
         apr_socket_close( pstClient );
         continue;
      }
      if( apr_socket_send( pstClient, "0000", &tLen ) != APR_SUCCESS )
      {
         LOG4C(( LOG_WARN, "���ͺ�������Ӧ���� %s\n", strerror(errno) ));
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
         LOG4C(( LOG_WARN, "�������ݳ��ȳ��� %s\n", strerror(errno) ));
         goto TX_END_PRE;
      }

      LOG4C(( LOG_DEBUG, "���յ����ݳ���(%d)\n", iDataLen ));
      pczRecvBuff = aczRecvBuff;
      if( iDataLen > sizeof(aczRecvBuff) )
      {
         pczRecvBuff = (char*)apr_palloc( pstLocalPool, iDataLen+1 );
         memset( pczRecvBuff, 0, iDataLen+1 );
         LOG4C(( LOG_DEBUG, "ԭ���ݿռ䲻��,���·���\n" ));
      }

      if( recvData( pstClient, pczRecvBuff, iDataLen ) != iDataLen )
      {
         LOG4C(( LOG_WARN, "�������ݳ��ȳ��� %s\n", strerror(errno) ));
         goto TX_END_PRE;
      }

      if( onTxStart( pczExecFile ) == -1 )
      {
         LOG4C(( LOG_FATAL, "���׿�ʼ������ʧ��%s\n", strerror(errno) ));
         goto TX_END_PRE;
      }

      UDO_sscanf( pczRecvBuff, iDataLen, pstReq );

      LOG4C(( LOG_DEBUG, "���õĺ����±�%d\n", iFunId ));

      LOG4C_CLOSE();
      LOG4C_CHGSUFFIX (aczFunName);
      LOG4C_OPEN();

      LOG4C(( LOG_DEBUG, "-----------------��ʼ���г���[%s]---------------------",stAgmFunManager.m_Register[iFunId].m_Name));
      LOG4C(( LOG_DEBUG, "----------------------------------------------------------------" ));
      if( LOG4C_ISLOG (LOG_DEBUG) )
      {
         UDO_FilePrint( pstReq, LOG4C_FP() );
      }
      iRet = (*stAgmFunManager.m_Register[iFunId].m_Func)( pstReq, pstRsp );
      LOG4C(( LOG_DEBUG, "׼��Ӧ������"));
      if( LOG4C_ISLOG (LOG_DEBUG) )
      {
         UDO_FilePrint( pstRsp, LOG4C_FP() );
      }
      LOG4C(( LOG_DEBUG, "--------------------�������н���--------------------------------" ));
      LOG4C(( LOG_DEBUG, "----------------------------------------------------------------\n" ));

      /**
        * �ָ���־��ϢΪԭ״̬ 
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
         LOG4C(( LOG_WARN, "���ͷ������ݳ���ʧ��(%s)\n", strerror(errno) ));
         goto TX_END;
      }

      if( sendData( pstClient, pczSendBuff, iDataLen ) == -1 )
      {
         LOG4C(( LOG_WARN, "���ͷ�������ʧ��(%s)\n", strerror(errno) ));
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
 * Description:���̴�������˳� 
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
 * Description:��Ӧ�û��Զ����ź� 
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
 * Description:�����ź� 
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
 * Description: Ӧ������������������ʼ
 * ����:
 *      name socket procshmkey cfgshmkey maxchild index offset loginit maxruntimes nodeid
 *      name        -   ��ϵͳ����
 *      socket      -   �Ѿ��򿪵��ļ�������
 *      procshmkey  -   ���̹�����Ϣ�����ڴ� KEY
 *      cfgshmkey   -   ������Ϣ�����ڴ� KEY
 *      maxchild    -   ÿ��ϵͳ����ӽ�����
 *      index       -   ��ǰ��ϵͳ��ϵͳ�е�����
 *      offset      -   ��ǰ�ӽ�������ϵͳ�е�ƫ����
 *      loginit     -   ��־��ʼ���ļ�
 *      maxruntimes -   ��ǰ�ӽ���������д���
 *      nodeid      -   �ڵ���
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
      printf( "���ӽ��̹������ڴ�%dʧ��%s\n",
              iProcShmKey, strerror(errno) );
      return -1;
   }
   pstProcess += iMaxChild*iIndex + iOffset;

   if( iCfgShmKey > 0 )
   {
      pcShmAddr = (char*)shm_get( iCfgShmKey, 0 );
      if( pcShmAddr == NULL )
      {
         printf( "����������Ϣ�����ڴ�%dʧ��%s\n",
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
      printf( "�ָ�socket %dʧ��%s", iSocket, strerror(errno) );
      return -1;
   }


   run( pstProcess, pstListener, iMaxRunTimes, iNodeId, argv[0] );

   return 0;
}
