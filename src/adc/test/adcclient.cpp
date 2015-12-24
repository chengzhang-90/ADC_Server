#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/tcp.h>
#include "easysock.h"
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "adc.h"
#include "pub_base.h"
#include "log.h"

/*
 *
 *  ��������: ADC�ͻ��˵Ĺ��캯��
 *
 */
AdpClient::AdpClient( char *hostIp, int hostPort, bool useBalance )
{
   m_Error = NO_ERR;
   m_Balance = useBalance;
   m_Server = -1;

   if( useBalance == true )
   {
      strcpy( m_HostIp[0], hostIp );
      m_HostPort[0] = hostPort;
   }
   else
   {
      strcpy( m_HostIp[1], hostIp );
      m_HostPort[1] = hostPort;
   }
   /*
    *  ����Apache �ڴ��
    */
   if ( apr_pool_create(&context, NULL) != APR_SUCCESS ) 
   {
      fprintf( stderr,"create apache poll error[%s]\n", 
                       apr_strerror(stat, msgbuf, sizeof(msgbuf) ) );
      m_Error = ADPCLI_GEN_APR_POOL_ERR;
   }
   sock=NULL;
}

/*
 *
 *  ��������: ADC�ͻ��˵���������
 *
 */
AdpClient::~AdpClient( void )
{
   /*ֹͣ����SOCKET */
   if( sock!=NULL )
   {
      if (apr_socket_close(sock) != APR_SUCCESS)  
      {
         fprintf(stderr, "Could not close socket\n");
         m_Error =ADPCLI_SHUTDOWN_SOCK_ERR;
      }
   }
   apr_pool_destroy( context );
}

/*
 *
 *  ��������: ADC�ͻ��˵ĳ�ʼ������,��δʹ�ø��ؾ��������£�û���ô�
 *
 */
int AdpClient::init( char *sysName )
{
   apr_interval_time_t timeout = apr_time_from_sec(2);
   apr_size_t length;

   /** �����ʹ��ƽ�⸺���򲻳�ʼ��ƽ�⸺�ط����� **/
   if( !m_Balance )
   {
      return 0;
   }

   stat=apr_sockaddr_info_get( &remote_sa, m_HostIp[0], 
                           APR_INET, m_HostPort[0], 0, context );
   if( stat!=APR_SUCCESS )
   {
      fprintf(stderr, "Address resolution failed for %s: %s\n",
                m_HostIp[0], apr_strerror(stat, msgbuf, sizeof(msgbuf)));
      m_Error = ADPCLI_GET_SOCKADDR_INFO_ERR;
      return -1;
   }

   stat = apr_socket_create( &sock, APR_INET, SOCK_STREAM, APR_PROTO_TCP,
                             context );
   if ( stat != APR_SUCCESS )
   {
      fprintf(stderr, "Couldn't create socket\n");
      m_Error = ADPCLI_GEN_SOCKET_ERR;
      return -1;
   }
   stat = apr_socket_timeout_set( sock, timeout*1000000 );
   if ( stat != APR_SUCCESS ) 
   {
      fprintf(stderr, "Problem setting timeout: %d\n", stat);
      m_Error = ADPCLI_SET_TIMEOUT_ERR;
      return -1;
   }

   stat = apr_socket_connect(sock, remote_sa);
   if (stat != APR_SUCCESS) 
   {
      fprintf(stderr, "Could not connect: %s (%d)\n",
              apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
      fflush(stderr);
      apr_socket_close(sock);
      m_Error = ADPCLI_CONNECT_ERR;
      return -1;
   }

   length = strlen( sysName );
   if ((stat = apr_socket_send(sock, sysName, &length) != APR_SUCCESS)) 
   {
      fprintf(stderr, "Problem sending data: %s (%d)\n",
          apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
      apr_socket_close(sock);
      m_Error = ADPCLI_SEND_ERR;
      return -1;
   }

   char recvBuf[32];
   int i;

   memset( recvBuf, 0, sizeof(recvBuf) );

   if ((stat = apr_socket_recv(sock, recvBuf, &length)) != APR_SUCCESS) 
   {
      fprintf(stderr, "Problem receiving data: %s (%d)\n",
                apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
      apr_socket_close(sock);
      m_Error = ADPCLI_RECV_ERR;
      return -1;
   }

   /** ��ƽ�⸺�ط��������ص�ַ�� "flag(1)ip port" ��ʽ���� **/
   /** flag Ϊ 0 ��ʾ��Ϣ����                               **/
   if( recvBuf[0] != '0' )
   {
      m_Error = ADPCLI_NO_AVILAVLE_SERVER;
      return -1;
   }
   i = 1;
   while( i < sizeof(recvBuf) )
   {
      if( recvBuf[i] == ' ' )
      {
         recvBuf[i] = 0;
         i++;
         break;
      }
      i++;
   }

   strcpy( m_HostIp[1], &recvBuf[1] );
   m_HostPort[1] = atoi(&recvBuf[i]);

   return 0;
}

/*
 *
 *  ��������: ���ͽ�Ҫ���õķ�������
 *            
 */
int AdpClient::sendSvcName( char *pczSvcName )
{
   char aczRsp[5];
   apr_size_t iLength;
   
   iLength=strlen( pczSvcName );

   if ( (stat = apr_socket_send(sock, pczSvcName, &iLength) != APR_SUCCESS) ) 
   {
       fprintf(stderr, "Problem sending data: %s (%d)\n",
               apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
       apr_socket_close(sock);
       m_Error = ADPCLI_SEND_ERR;
       return -1;
   }

   iLength = 4;

   memset( aczRsp, 0, sizeof(aczRsp) );

   if ((stat = apr_socket_recv(sock, aczRsp, &iLength)) != APR_SUCCESS) 
   {
       fprintf(stderr, "Problem receiving data: %s (%d)\n",
               apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
       apr_socket_close(sock);
       m_Error = ADPCLI_RECV_ERR;
       return -1;
   }
   if( memcmp( aczRsp, "0000", 4 ) != 0 )
   {
      return -1;
   }

   return 0;
}

/*
 *
 *  ��������: ����ͨѶ���ĳ���
 *            
 */
int AdpClient::sendDataSize( int iLen )
{
   /*Ϊ��ֹ��ͬϵͳ���������ͱ�ʾ��һ��,����ת��Ϊ�ַ����ٷ���*/
   char aczLen[12], aczRsp[5];
   apr_size_t iLength;

   sprintf( aczLen, "%d", iLen );

   iLength=strlen( aczLen );
   if ((stat = apr_socket_send(sock, aczLen, &iLength) != APR_SUCCESS)) 
   {
       apr_socket_close(sock);
       fprintf(stderr, "Problem sending data: %s (%d)\n",
               apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
       m_Error = ADPCLI_SEND_ERR;
       return -1;
   }

   iLength = 4;

   memset( aczRsp, 0, sizeof(aczRsp) );

   if ((stat = apr_socket_recv(sock, aczRsp, &iLength)) != APR_SUCCESS) 
   {
       apr_socket_close(sock);
       fprintf(stderr, "Problem receiving data: %s (%d)\n",
               apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
       m_Error = ADPCLI_RECV_ERR;
       return -1;
   }
   aczRsp[4] = 0;

   if( strncmp( aczRsp, "0000", 4 ) != 0 )
   {
      m_Error = ADPCLI_DATA_LEN_OVERFLOW;
      return -1;
   }

   return 0;
}

/*
 *
 *  ��������: ����ͨѶ���ĳ���
 *            
 */
int AdpClient::recvDataSize( int iInitSize )
{
   char aczLen[13];
   int i=0;
   apr_size_t iLength;

   memset( aczLen, 0, sizeof(aczLen) );

   iLength=11;
   if ((stat = apr_socket_recv(sock, aczLen, &iLength)) != APR_SUCCESS) 
   {
      apr_socket_close(sock);
      fprintf(stderr, "Problem receiving data: %s (%d)\n",
                apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
      m_Error = ADPCLI_RECV_ERR;
      return -1;
   }
   while( *(aczLen+i) != 0 )
   {
      if( *(aczLen+i) == '\n' )
      {
         *(aczLen+i) = 0;
         break;
      }
      i++;
   }
 
   return  atoi(aczLen);
}

/*
 *
 *  ��������: ���շ������ƣ������� ����SERVER��
 *            
 */
int AdpClient::call( char *svcName,  UDP req, 
                     UDP res, int iTimeOut )
{
   int iBufSize, iDataLen, ret=0;
   char *pSend, sendBuf[MAX_BUFF_SIZE+1];
   char *pRecv, recvBuf[MAX_BUFF_SIZE+1];
   char funIndex[5];

   char aczSvcHead[ 100 ];

   apr_size_t iLength,iLen;

   pSend = sendBuf;
   pRecv = recvBuf;
   m_Error = NO_ERR;
   iBufSize = UDO_ValueSize( req );

   stat=apr_sockaddr_info_get( &remote_sa, m_HostIp[1],
                           APR_INET, m_HostPort[1], 0, context );
   if( stat!=APR_SUCCESS )
   {
      fprintf(stdout, "Address resolution failed for %s: %s\n",
                m_HostIp[0], apr_strerror(stat, msgbuf, sizeof(msgbuf)));
      m_Error = ADPCLI_GET_SOCKADDR_INFO_ERR;
      return -1;
   }
   stat = apr_socket_create( &sock, APR_INET, SOCK_STREAM, APR_PROTO_TCP,
                             context );
   if ( stat != APR_SUCCESS )
   {
      fprintf(stderr, "Couldn't create socket\n");
      m_Error = ADPCLI_GEN_SOCKET_ERR;
      return -1;
   }
   stat = apr_socket_timeout_set(sock, apr_time_from_sec(iTimeOut) );
   if (stat)
   {
      fprintf(stderr, "Problem setting iTimeOut: %d\n", stat);
      m_Error = ADPCLI_SET_TIMEOUT_ERR;
      return -1;
   }

   stat = apr_socket_connect(sock, remote_sa);
   if (stat != APR_SUCCESS) {
      apr_socket_close(sock);
      fprintf(stderr, "Could not connect: %s (%d)\n",
                apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
      fflush(stderr);
      m_Error = ADPCLI_CONNECT_ERR;
      return -1;
   }

   iDataLen = UDO_ValueSize( req );
   if( iDataLen>sizeof( sendBuf ) );
   {
      /*������ʼ����ռ�����·���,�ݲ����Ƿ��䲻�ɹ������*/
      pSend=(char *)malloc( iDataLen+1 );
   }
   UDO_sprintf( req, pSend, &iDataLen );

   if( sendSvcName( svcName ) == -1 )
   {
      ret = -1;
      goto CALL_RET;
   }
   if( sendDataSize( iDataLen )== -1 )
   {
      ret = -1;
      goto CALL_RET;
   }

   iLen=0;
   /*
    *  ���ǵ�һ�η��ʹ�С���Ƶ�����(��������Ϊ128KBytes)������ѭ�����͵ķ�ʽ
    */
   do
   {
      iLength=iDataLen-iLen;
      if ((stat = apr_socket_send(sock, pSend+iLen, &iLength) != APR_SUCCESS))
      {
         apr_socket_close(sock);
         fprintf(stderr, "Problem sending data: %s (%d)\n",
                 apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
         m_Error = ADPCLI_SEND_ERR;
         ret = -1;
         goto CALL_RET;
      }
      iLen+=iLength;
   } while( iLen<iDataLen );

   /*������Ӧ����*/
   if( (iDataLen = recvDataSize( sizeof(recvBuf) )) == -1 )
   {
      m_Error = ADPCLI_RECV_ERR;
      ret = -1;
      goto CALL_RET;
   }
   /*���յİ����ȴ��ڳ�ʼ�������·���ռ�*/
   if( iDataLen>sizeof( recvBuf ) )
   {
      pRecv=(char *)malloc( iDataLen+1 );
   }

   iLength=iDataLen;
   iLen=0;
   do
   {
      iLength=iDataLen-iLen;
      stat = apr_socket_recv(sock, recvBuf+iLen, &iLength);
      if( stat != APR_SUCCESS )
      {
          fprintf(stderr, "Problem receiving data: %s (%d)\n",
                   apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
          apr_socket_close(sock);
          m_Error = ADPCLI_RECV_ERR;
          ret = -1;
          goto CALL_RET;
      }
      iLen+=iLength;
   } while( iLen<iDataLen );

   UDO_sscanf( recvBuf, iDataLen, res );

CALL_RET:
   if( pSend != sendBuf )
   {
      free( pSend );
   }
   if( pRecv != recvBuf )
   {
      free( pRecv );
   }

   return ret;
}
