/*
**     函数功能:  本文件包括采用Apache底层函数实现的ADC客户端函数
**
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "easysock.h"
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "adc.h"
#include "pub_base.h"
#include "log.h"
#include "ap_base.h"

/*
 *
 *  函数功能: ADC客户端的构造函数
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
    *  产生Apache 通讯池
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
 *  函数功能: ADC客户端的析构函数
 *
 */
AdpClient::~AdpClient( void )
{
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
 *  函数功能: ADC客户端的初始化函数,在未使用负载均衡的情况下，没有用处
 *
 */
int AdpClient::init( char *sysName )
{
   apr_interval_time_t timeout = apr_time_from_sec(5);
   apr_size_t length;

   /** 如果不使用平衡负载则不初始化平衡负载服务器 **/
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
   stat = apr_socket_timeout_set( sock, timeout );
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

   /** 从平衡负载服务器返回地址以 "flag(1)ip port" 方式出现 **/
   /** flag 为 0 表示信息可用                               **/
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
 *  函数功能: 发送将要调用的服务头信息，包括名称、交易码、超时
 *            服务器端进行合法性校验 
 *            其中服务名称64位,交易码20位,超时5位 ,均为右对齐，左补空格
 *            
 */
int AdpClient::sendSvcHead( char *pczSvcHead )
{
   char aczRsp[5];
   apr_size_t iLength;
   struct log4c_logInfo logInfo;
 
   struct timeval stStart;
   struct timeval stEnd;
   double lStart, lEnd;

#ifdef QDEBUG
   memset (&stStart, 0, sizeof (stStart));
   memset (&stEnd, 0, sizeof (stEnd));

   gettimeofday (&stStart, NULL);
#endif

   iLength=strlen( pczSvcHead );

   if ( (stat = apr_socket_send(sock, pczSvcHead, &iLength) != APR_SUCCESS) ) 
   {
       fprintf(stderr, "Problem sending data: %s (%d)\n",
               apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
       apr_socket_close(sock);
       m_Error = ADPCLI_SEND_ERR;
       return -1;
   }

   iLength  = sizeof (logInfo);
   memset (&logInfo, 0, iLength);
   LOG4C_LOGINFO (&logInfo);

   if ( ap_tcp_write (sock, (char *)&logInfo, iLength) != 0) 
   {
       fprintf(stderr, "Problem sending data!\n");
       apr_socket_close(sock);
       m_Error = ADPCLI_SEND_ERR;
       return -1;
   }

#ifdef QDEBUG
   memset (&stEnd, 0, sizeof (stEnd));
   gettimeofday (&stEnd, NULL);
   lStart = stStart.tv_sec * 1000 + stStart.tv_usec / 1000;
   lEnd   = stEnd.tv_sec * 1000 + stEnd.tv_usec / 1000;

   printf ("CALLADC sendhead time [%.0lf] len[%d]\n", lEnd - lStart, iLength);
#endif

   iLength = RECV_LEN;
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
 *  函数功能: 发送通讯包的长度
 *            
 */
int AdpClient::sendDataSize( int iLen )
{
   /*为防止不同系统间数据类型表示不一致,长度转换为字符串再发送*/
   char aczLen[12], aczRsp[5];
   apr_size_t iLength;

   sprintf( aczLen, "%10d", iLen );

   iLength=strlen( aczLen );
   if ((stat = apr_socket_send(sock, aczLen, &iLength) != APR_SUCCESS)) 
   {
       apr_socket_close(sock);
       fprintf(stderr, "Problem sending data: %s (%d)\n",
               apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
       m_Error = ADPCLI_SEND_ERR;
       return -1;
   }

   iLength = RECV_LEN;

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
 *  函数功能: 接收通讯包的长度
 *            
 */
int AdpClient::recvDataSize( int iInitSize )
{
   char aczLen[13];
   int iLen;
   apr_size_t iLength;

   memset( aczLen, 0, sizeof(aczLen) );

   iLength=10;
   if ((stat = apr_socket_recv(sock, aczLen, &iLength)) != APR_SUCCESS) 
   {
      apr_socket_close(sock);
      fprintf(stderr, "Problem receiving data: %s (%d)\n",
                apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
      m_Error = ADPCLI_RECV_ERR;
      return -1;
   }
   /*响应为10位*/
   aczLen[ 10 ]='\0';
   iLen = atoi(aczLen);

   return iLen;
}

/*
 *
 *  函数功能: 按照服务名称，交易码 调用SERVER端
 *            
 */
int AdpClient::call( char *svcName, char *tranCode, UDP req, 
                     UDP res, int timeout )
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

   stat = apr_socket_connect(sock, remote_sa);
   if (stat != APR_SUCCESS) {
      apr_socket_close(sock);
      fprintf(stderr, "Could not connect: %s (%d)\n",
                apr_strerror(stat, msgbuf, sizeof(msgbuf)), stat);
      fflush(stderr);
      m_Error = ADPCLI_CONNECT_ERR;
      return -1;
   }
   apr_socket_addr_get(&remote_sa, APR_REMOTE, sock);

   apr_sockaddr_ip_get(&remote_ipaddr, remote_sa);

   apr_socket_addr_get(&local_sa, APR_LOCAL, sock);

   apr_sockaddr_ip_get(&local_ipaddr, local_sa);

   stat = apr_socket_timeout_set(sock, apr_time_from_sec(timeout));
   if (stat)
   {
      fprintf(stderr, "Problem setting timeout: %d\n", stat);
      m_Error = ADPCLI_SET_TIMEOUT_ERR;
      return -1;
   }

   iDataLen = UDO_ValueSize( req );
   if( iDataLen>sizeof( sendBuf ) );
   {
      /*超过初始分配空间后，重新分配,暂不考虑分配不成功的情况*/
      pSend=(char *)malloc( iDataLen+1 );
   }
   UDO_sprintf( req, pSend, &iDataLen );

   memset( aczSvcHead,0x00,sizeof( aczSvcHead ) );

   sprintf( aczSvcHead,"%64s%20s%5d",svcName,tranCode,timeout );

   aczSvcHead[ 89 ]='\0';

   if( sendSvcHead( aczSvcHead ) == -1 )
   {
      ret = -1;
      goto CALL_RET;
   }

   if( sendDataSize( iDataLen )== -1 )
   {
      ret = -1;
      goto CALL_RET;
   }
   iLength=iDataLen;
   iLen=0;


   /*
    *  考虑到一次发送大小限制的问题(开发环境为128KBytes)，采用循环发送的方式
    */
   do{
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


   /*接收响应长度*/
   if( (iDataLen = recvDataSize( sizeof(recvBuf) )) == -1 )
   {
      m_Error = ADPCLI_RECV_ERR;
      ret = -1;
      goto CALL_RET;
   }
   /*接收的包长度大于初始长度重新分配空间*/
   if( iDataLen>sizeof( recvBuf ) )
   {
      pRecv=(char *)malloc( iDataLen+1 );
   }
   iLength=iDataLen;
   iLen=0;
   do
   {
      iLength=iDataLen-iLen;
      if ((stat = apr_socket_recv(sock, recvBuf+iLen, &iLength)) != APR_SUCCESS) 
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

/*
 *
 * 通讯封装函数 采用APACHE底层函数实现
 *
 */
int ADP_APR_UDOTrans( char *epczHost, int eiPort, char *req,  char *res,
                      char *epcFun, char *epcTranCode, int eiTime )
{
   int ret;

   if (apr_initialize() != APR_SUCCESS) 
   {
        fprintf(stderr, "Something went wrong\n");
        return 9997;
   }
   atexit(apr_terminate);

   AdpClient client( epczHost, eiPort );

   ret = client.init( "system" );

   ret = client.call( epcFun, epcTranCode, req, res, eiTime );
   if( ret != 0 )
   {
      switch( client.getError( ) )
      {
      case 7:
      case 9:
         return 9999;
      default:
         return 9998;
      }
   }
   return 0;
}
