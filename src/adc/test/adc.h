#include "udo.h"
#include "easysock.h"
#include "apr_network_io.h"
#include "apr_errno.h"
#include "apr_general.h"


#ifndef __APPLICATION_DEVELOPMENT_PLATFORM_H__
#define __APPLICATION_DEVELOPMENT_PLATFORM_H__

#define RECV_LEN 128

static const int NO_ERR                      = 0;
static const int CONNECT_BALANCE_SERVER_ERR  = 1;
static const int CONNECT_BUSINESS_SERVER_ERR = 2;
static const int TIME_OUT_BALANCE_SERVER     = 3;
static const int TIME_OUT_BUSINESS_SERVER    = 4;
static const int ADPCLI_NO_AVILAVLE_SERVER   = 5;
static const int ADPCLI_CONNECT_ERR          = 6;
static const int ADPCLI_SET_TIMEOUT_ERR      = 7;
static const int ADPCLI_SEND_ERR             = 8;
static const int ADPCLI_RECV_ERR             = 9;
static const int SERVICE_NAME_ERR            = 10;
static const int ADPCLI_DATA_LEN_OVERFLOW    = 11;
static const int ADPCLI_GEN_APR_POOL_ERR     = 12;
static const int ADPCLI_SHUTDOWN_SOCK_ERR    = 13;
static const int ADPCLI_GET_SOCKADDR_INFO_ERR= 14;
static const int ADPCLI_GEN_SOCKET_ERR       = 15;

static const int MAX_BUFF_SIZE = 131072;   //128K

/**
  * 
  */
class AdpClient
{
private:
   char m_HostIp[2][20];
   int  m_HostPort[2];
   int  m_Connect;
   int  m_Error;
   char msgbuf[80];

   int  m_Server;
   bool m_Balance;

   apr_pool_t *context;
   apr_sockaddr_t *local_sa, *remote_sa;
   apr_status_t stat;
   apr_socket_t *sock;
   char *local_ipaddr, *remote_ipaddr;
   apr_port_t local_port, remote_port;

   int sendDataSize( int iTotalSize );
   int recvDataSize( int iInitSize );

   int sendSvcHead( char *pczSvcHead );

public:
   /** 如果useBalance是true,则后面地址是负载均衡服务器地址 **/
   /** 否则后面地址是直接连接的交易服务器地址              **/
   AdpClient( char *hostIp, int hostPort, bool useBalance=false );
   ~AdpClient();

   /** 用于交易的接口 **/
   int  init( char *sysName );
   int  call( char *svc, char *trancode, UDP request, UDP response, int timeOut=-1 );

   /** 取错误代码 **/
   int getError( void ) { return m_Error; }

   /** 判断是否可用 **/
   bool isAvilable( void )
   {
      return m_Error == NO_ERR;
   }
};

extern "C" {
/** 客户端 **/
int ADP_APR_UDOTrans( char *epczHost, int eiPort, char *req,  char *res,
                      char *epcFun, char *epcTranCode, int eiTime );
}

#endif
