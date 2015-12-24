/*****************************************************************
*
*    Copyright (C), 2010-2020, NewLand Computer Co., Ltd.
*    Version:  VER1.0
*    Description: ϵͳ�����ӷ������Ӧ���Ӻ���
*    History:
*
*    Function List:
*          makeListener: ����APACH�ײ㺯����װ��������
*          makeClient : ����APACH�ײ㺯����װ�ͻ������Ӻ���
*          ap_tcp_write : ��������
*          ap_tcp_read : ��������
*          ap_basename : ȥ��Ŀ¼��ȡ�ļ�����
*          shm_get  :  ���ӹ����ڴ�
*          loadEnviorment : ���ػ����������ڴ�
*          fileExistAndExec : �ж��ļ��Ƿ�������ǿ�ִ�е�
*          getAxpSerial : ȡƽ̨��ˮ�ͽ���ƽ̨����
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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "adc_base.h"
#include "log.h"
#include "apr_hash.h"
#include "pub_base.h"

/*
 *
 * Function: makeListener
 * Description: ����APACH�ײ㺯����װ��������
 * Return:
 *         -1 - ͨѶʧ��
 *         0  - SUCC 
 * Other:
 *
 */
int makeListener( apr_socket_t **pstListener, apr_pool_t *pstPool,
                  int iPort, int iFamily, int iType )
{
   apr_status_t   tStatus;
   apr_sockaddr_t *pstAddr;

   tStatus = apr_socket_create( pstListener, iFamily, iType,
                                APR_PROTO_TCP, pstPool );
   if( tStatus != APR_SUCCESS )
   {
      LOG4C(( LOG_FATAL, "failed to create socket %s\n", strerror(errno) ));
      return -1;
   }

   tStatus = apr_socket_opt_set( *pstListener, APR_SO_REUSEADDR, 1 );
   if( tStatus != APR_SUCCESS && tStatus != APR_ENOTIMPL )
   {
      LOG4C(( LOG_FATAL, "failed to apr_socket_opt_set APR_SO_REUSEADDR %s\n",
              strerror(errno) ));
      return -1;
   }

   tStatus = apr_socket_opt_set( *pstListener, APR_SO_KEEPALIVE, 1 );
   if( tStatus != APR_SUCCESS && tStatus != APR_ENOTIMPL )
   {
      LOG4C(( LOG_FATAL, "failed to apr_socket_opt_set APR_SO_KEEPALIVE %s\n",
              strerror(errno) ));
      return -1;
   }

   /**
     * ���� socket ���仺������СΪ 2048
     */
   tStatus = apr_socket_opt_set( *pstListener, APR_SO_SNDBUF, 2048 );
   if( tStatus != APR_SUCCESS && tStatus != APR_ENOTIMPL )
   {
      LOG4C(( LOG_FATAL, "failed to apr_socket_opt_set APR_SO_SNDBUF %s\n",
              strerror(errno) ));
      return -1;
   }

   tStatus = apr_socket_opt_set( *pstListener, APR_TCP_NODELAY, 1);
   if( tStatus != APR_SUCCESS && tStatus != APR_ENOTIMPL )
   {
      LOG4C(( LOG_FATAL, "failed to apr_socket_opt_set APR_TCP_NODELAY %s\n",
              strerror(errno) ));
   }

   /**
     * ��ʾָ��SOCKETΪBLOCK��ʽ  
     **/
   tStatus = apr_socket_opt_set( *pstListener, APR_SO_NONBLOCK, 0);
   if( tStatus != APR_SUCCESS && tStatus != APR_ENOTIMPL )
   {
      LOG4C(( LOG_FATAL, "failed to apr_socket_opt_set APR_SO_NONBLOCK %s\n",
              strerror(errno) ));
   }

   apr_sockaddr_info_get( &pstAddr, NULL, iFamily, iPort, 0, pstPool );

   tStatus = apr_socket_bind( *pstListener, pstAddr );
   if( tStatus != APR_SUCCESS )
   {
      LOG4C(( LOG_FATAL, "failed to bind socket %s at %d\n",
              strerror(errno), iPort ));
      return -1;
   }

   /** the 511 from apache **/
   tStatus = apr_socket_listen( *pstListener, 511 );
   if( tStatus != APR_SUCCESS )
   {
      LOG4C(( LOG_FATAL, "failed to apr_socket_listen %s\n", strerror(errno) ));
      return -1;
   }

   return 0;
}

/*
 *
 * Function: makeClient
 * Description: ����APACH�ײ㺯����װ�ͻ������Ӻ���
 * Return:
 *         -1 - ͨѶʧ��
 *         0  - SUCC
 * Other:
 *
 */
int makeClient( apr_socket_t **pstClient, apr_pool_t *pstPool, char *pczServer,
                int iPort, int iFamily, int iType, int iTimeOut )
{
   apr_status_t   tStatus;
   apr_sockaddr_t *pstRemote;

   tStatus = apr_sockaddr_info_get( &pstRemote, pczServer, iFamily,
                                    iPort, 0, pstPool );
   if( tStatus != APR_SUCCESS )
   {
      LOG4C(( LOG_FATAL, "failed to sockaddr_info_get %s\n", strerror(errno) ));
      return -1;
   }

   tStatus = apr_socket_create( pstClient, iFamily, iType,
                                APR_PROTO_TCP, pstPool );
   if( tStatus != APR_SUCCESS )
   {
      LOG4C(( LOG_FATAL, "failed to create socket %s\n", strerror(errno) ));
      return -1;
   }

   /** ����ֻ�ȴ�5���� **/
   if( apr_socket_timeout_set( *pstClient, apr_time_from_sec(5) ) !=
       APR_SUCCESS )
   {
      LOG4C(( LOG_FATAL, "failed to set timeout %s\n", strerror(errno) ));
      return -1;
   }

   if( apr_socket_connect( *pstClient, pstRemote ) != APR_SUCCESS )
   {
      apr_socket_close( *pstClient );
      LOG4C(( LOG_FATAL, "failed to connect: %s\n", strerror(errno) ));
      return -1;
   }

   /**
     * ��ʾָ��SOCKETΪBLOCK��ʽ  
     **/
   tStatus = apr_socket_opt_set( *pstClient, APR_SO_NONBLOCK, 0);
   if( tStatus != APR_SUCCESS && tStatus != APR_ENOTIMPL )
   {
      LOG4C(( LOG_FATAL, "failed to apr_socket_opt_set APR_SO_NONBLOCK %s\n",
              strerror(errno) ));
   }

   if( apr_socket_timeout_set( *pstClient, apr_time_from_sec(iTimeOut) ) !=
       APR_SUCCESS )
   {
      LOG4C(( LOG_FATAL, "failed to set timeout %s\n", strerror(errno) ));
      return -1;
   }

   return 0;
}

/*
 *
 * Function: ap_tcp_write
 * Description: �������� 
 * Input:
 *         sock - ͨѶ������
 *         buf -  ���ݿռ� 
 *         len - ���ݳ���
 * Output:
 *
 * Return:
 *         -1 - ͨѶʧ��
 *         0  - SUCC
 * Other:
 *
 */
int ap_tcp_write ( apr_socket_t *sock, char *buf, int len )
{
        int  left;
        apr_size_t  nwrite;
        apr_status_t stat;
        
        left = len;
        while (left > 0)
        {
                nwrite = left;
                stat  = apr_socket_send (sock, buf, &nwrite);
                if (stat != APR_SUCCESS) 
                    return ( -1 );

                left -= nwrite;
                buf  += nwrite;
        }

        return ( 0 );
}

/*
 *
 * Function: ap_tcp_read
 * Description: ��������
 * Input:
 *         sock - ͨѶ������
 *         buf -  ���ݿռ�
 *         len - ���ݳ���
 * Output:
 *
 * Return:
 *         -1 - ͨѶʧ��
 *         0  - SUCC
 * Other:
 *
 */
int ap_tcp_read ( apr_socket_t *sock, char *buf, int len )
{
        int  left;
        apr_size_t nread;
        apr_status_t stat;

        left = len;
        while ( left > 0 )
        {
                nread = left;
                stat = apr_socket_recv ( sock, buf, &nread);

                if ( stat != APR_SUCCESS ) return( -1 );

                left -= nread;
                buf += nread;
        }

        return 0;
}

/*
 *
 * Function: ap_basename
 * Description: ȥ��Ŀ¼��ȡ�ļ�����
 * Input:
 *         str - �����ļ�+Ŀ¼
 * Output:
 *
 * Return:
 *         �ļ����� 
 * Other:
 *
 */
char *ap_basename(char *str)
{
        int i;
        for(i=strlen(str)-1; i>=0; i--)
                if (str[i]=='/') break;
        return &str[i+1];
}

/*
 *
 * Function: shm_get
 * Description:  ���ӹ����ڴ�
 * Input:
 *         iShmKey - �����ڴ�KEY
 *         iSize  - �����ڴ��С
 * Output:
 *
 * Return:
 *         �����ڴ�ָ��
 * Other:
 *
 */
void * shm_get( int iShmKey, int iSize )
{
   bool bCreateNew = false;
   int iShmId;
   void *pShmAddr;

   iShmId = shmget( iShmKey, 0, 0600 );
   if( iShmId == -1 )
   {
      if( iSize == 0 )
      {
         LOG4C(( LOG_FATAL, "failed to shmget %s\n", strerror(errno) ));
         return NULL;
      }

      /*�����С����0,������Ҫ�������ڴ�*/
#ifdef SHM_HUGETLB
      iShmId = shmget( iShmKey, iSize,
                       0700|IPC_CREAT|IPC_EXCL );
#else
      iShmId = shmget( iShmKey, iSize,
                       0700|IPC_CREAT|IPC_EXCL );
#endif
      bCreateNew = true;
   }

   if( iShmId == -1 )
   {
      LOG4C(( LOG_FATAL, "failed to shmget %s\n", strerror(errno) ));
      return NULL;
   }

   pShmAddr = (ProcessRegister*)shmat( iShmId, 0, 0 );
   if( (long)pShmAddr == -1 )
   {
      LOG4C(( LOG_FATAL, "failed to shmat %s\n", strerror(errno) ));
      return NULL;
   }
   
   if( bCreateNew )
   {
      memset( pShmAddr, 0, iSize );
   }

   return pShmAddr;
}


/*
 *
 * Function: fileExistAndExec
 * Description: �ж��ļ��Ƿ�������ǿ�ִ�е� 
 * Input:
 *         pczFile - �ļ�����
 * Output:
 *
 * Return:
 *        true - �� 
 *        false - �� 
 * Other:
 *
 */
bool fileExistAndExec( char *pczFile )
{
   char aczPath[8193], *p1, *p2, aczFile[128];
   bool bFound = false, bExecute = false;
   struct stat stStat;

   /**
     * ����ִ���ļ������� PATH ָ���Ļ����������Ƿ����
     */
   memset( aczPath, 0, sizeof(aczPath) );
   if( getenv( "PATH" ) != NULL )
   {
      strcpy( aczPath, getenv( "PATH" ) );
   }

   p1 = aczPath;
   do
   {
      p2 = strchr( p1, ':' );
      if( p2 != NULL )
      {
         *p2 = 0;
         sprintf( aczFile, "%s/%s", p1, pczFile );
         if( lstat( aczFile, &stStat ) == 0 )
         {
            /** �Ѿ��ҵ��ļ�, ���ǻ���֪���Ƿ��ִ�� **/
            bFound = true;
            if( stStat.st_mode&S_IXUSR != 0 )
            {
               bExecute = true;
               break;
            }
         }
         p1 = p2+1;
      }
      else
      {
         sprintf( aczFile, "%s/%s", p1, pczFile );
         if( lstat( aczFile, &stStat ) == 0 )
         {
            /** �Ѿ��ҵ��ļ�, ���ǻ���֪���Ƿ��ִ�� **/
            bFound = true;
            if( stStat.st_mode&S_IXUSR != 0 )
            {
               bExecute = true;
               break;
            }
         }
      }
   } while( p2 != NULL );

   if( bFound == false )
   {
      return false;
   }

   if( bExecute == false )
   {
      return false;
   }

   return true;
}

/*
 *
 * Function: loadEnviorment
 * Description: ���ػ����������ڴ� 
 * Input:
 *         moduleName - ���ص�ģ������ 
 * Output:
 *
 * Return:
 * Other:
 *
 */
void loadEnviorment( char *moduleName )
{
   char aczCompleteFileName[128];
   char aczLine[1024], *p;
   FILE *fp;

   sprintf( aczCompleteFileName, "%s/etc/%s.env",
            getenv( "ADC_ROOT" ), moduleName );
   fp = fopen( aczCompleteFileName, "r" );
   if( fp == NULL )
   {
      return;
   }

   /**
     * ���洦��������
     */
   memset( aczLine, 0, sizeof(aczLine) );
   while( fgets( aczLine, sizeof(aczLine), fp ) != NULL )
   {
      PUB_StrTrim( aczLine, '\n' );
      PUB_StrTrim( aczLine, ' ' );
      if( aczLine[0] == '#' )
      {
         continue;
      }

      if( (p = strchr( aczLine, '=' )) == NULL )
      {
         continue;
      }

      if( strlen( aczLine ) < 3 )
      {
         continue;
      }

#ifndef __hpux
      *p = 0;
      if( setenv( aczLine, p+1, 1 ) != 0 )
      {
         LOG4C(( LOG_WARN, "setenv: %s" , strerror(errno) ));
      }
#else
     p = (char *)malloc (strlen (aczLine) + 1);
     strcpy (p, aczLine); 
     
         LOG4C(( LOG_WARN, "putenv: %s" , p ));
      if( putenv( p ) != 0 )
      {
         LOG4C(( LOG_WARN, "putenv: %s" , strerror(errno) ));
      }
#endif
   }

   fclose( fp );
}

/*
 *
 * Function: getAxpSerial
 * Description: ȡƽ̨��ˮ�ͽ���ƽ̨����
 * Input:
 * Output:
 *         pczSerial - ƽ̨��ˮ
 *         pczTxDate - ƽ̨����
 * Return:
 * Other:
 *         ������ʹ��ǰ���뱣֤�л������� SER_ADDR �� SER_PORT
 *
 */
int getAxpSerial( char *pczSerial, char *pczTxDate, apr_pool_t *pstPool )
{
   apr_socket_t *pstSock;
   apr_status_t tStat;
   apr_sockaddr_t *pstRemote;

   char *pczServer;
   char aczRecv[17];
   int  iPort;
   apr_size_t tLen = 16;

   pczServer = getenv( "SER_ADDR" );
   iPort = atoi( getenv( "SER_PORT" ) );

   tStat = apr_sockaddr_info_get( &pstRemote, pczServer, APR_INET,
                                  iPort, 0, pstPool );
   if( tStat != APR_SUCCESS )
   {
      return -1;
   }

   tStat = apr_socket_create( &pstSock, APR_INET, SOCK_STREAM, APR_PROTO_TCP,
                              pstPool );
   if ( tStat != APR_SUCCESS )
   {
      return -1;
   }

   tStat = apr_socket_timeout_set( pstSock, 2000000 );
   if ( tStat != APR_SUCCESS )
   {
      apr_socket_close( pstSock );
      return -1;
   }

   tStat = apr_socket_connect( pstSock, pstRemote );
   if ( tStat != APR_SUCCESS )
   {
      apr_socket_close( pstSock );
      return -1;
   }

   tStat = apr_socket_recv( pstSock, aczRecv, &tLen);
   apr_socket_close( pstSock );
   if ( tStat != APR_SUCCESS || tLen != 16 )
   {
      return -1;
   }

   memcpy( pczSerial, aczRecv, 8 );
   memcpy( pczTxDate, &aczRecv[8], 8 );
   pczSerial[8] = 0;
   pczTxDate[8] = 0;

   return 0;
}

