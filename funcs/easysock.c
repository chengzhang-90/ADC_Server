#include <sys/types.h>
#if defined( SCO_SV )
#include <sys/netinet/in.h>
#endif
#include <sys/socket.h>

#ifndef __hpux
#include <sys/select.h>
#endif

#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#include "easysock.h"
#include "log.h"

int tcp_bind( int port )
{
   const int on = 1;
   int sock;
   struct sockaddr_in sockname;

   memset ((char *)&sockname,0,sizeof(sockname));

   if( (sock=socket(AF_INET,SOCK_STREAM,0)) == -1 )
   {
      return -1;
   }
#if 0
   setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
#endif

   sockname.sin_addr.s_addr=INADDR_ANY;
   sockname.sin_family=AF_INET;
   sockname.sin_port=htons(port);

   if ( (bind(sock,(struct sockaddr *)&sockname,sizeof(sockname))) == -1 )
   {
      close (sock);
      return -1;
   }
   listen (sock,5);

   return sock;
}

int tcp_connect(char *host, int port)
{
   int sock, flags;
   struct sockaddr_in rsock;
   struct hostent * hostinfo;
   struct in_addr * addp;
   struct linger stLinger = { 1, 0 };

   memset ((char *)&rsock,0,sizeof(rsock));

   if ( (hostinfo=gethostbyname(host)) == NULL )
   {
      return -1;
   }
   
   sock=socket(AF_INET,SOCK_STREAM,0);
   if ( sock == -1 )
   {
      return -1;
   }
   
   addp=(struct in_addr *)*(hostinfo->h_addr_list);
   rsock.sin_addr=*addp;
   rsock.sin_family=AF_INET;
   rsock.sin_port=htons(port);

   if( connect(sock,(struct sockaddr *)(&rsock),sizeof(rsock)) == -1 )
   {
      close (sock);
      return -1;
   }

   flags = setsockopt( sock, SOL_SOCKET, SO_LINGER, &stLinger,
                       sizeof( struct linger ) );
   if( flags == -1 )
   {
      close (sock);
      return -1;
   }

   return sock;
}

int tcp_close( int sock )
{
   return close( sock );
}

int tcp_accept( int sock, int waittime )
{
   struct sockaddr sa;

#ifdef AF_CCITT
   int    len = sizeof(sa);
#else
   size_t len = sizeof(sa);
#endif

   int newsock;
   int one = 1;
   int on = 1;
   int flags;
#if 0
   if( tcp_wait_recv( sock, waittime ) == 0 )
   {
      return -1;
   }
#endif

   do
   {
      newsock = accept( sock, &sa, &len );
   } while( newsock == -1 );

   return newsock;
}

int tcp_send( int sock, char *buf, int len )
{
   char sendbuf[512+5];
   int pos = 0, sendlen;

   do
   {
      sendlen = len - pos;
      if( sendlen > 512 )
      {
         sendlen = 512;
         sendbuf[0] = '1';
      }
      else
      {
         sendbuf[0] = '0';
      }
      sprintf( &sendbuf[1], "%03d", sendlen );
      memcpy( &sendbuf[4], buf+pos, sendlen );
      if( write( sock, sendbuf, sendlen + 4 ) == -1 )
      {
         return -1;
      }
      pos += sendlen;
   } while( pos < len );

   return pos;
}

int tcp_raw_send( int sock, char *buf, int len )
{
   return write( sock, buf, len );
}

int tcp_recv( int sock, char *buf, int maxlen )
{
   int rch, rlen;
   char hinfo[5];
   int pos = 0;
   bool hasNext = false;

   hinfo[4] = 0;
   do
   {
      if( read( sock, hinfo, 4 ) == 4 )
      {
         hasNext = (hinfo[0]=='1')? true: false;
         rch = atoi( &hinfo[1] );
         rch = ((maxlen-pos)>rch)? rch: maxlen-pos;
         rlen = 0;
         do
         {
            rch -= rlen;
            rlen = read( sock, buf+pos, rch );
            if( rlen == -1 )
            {
               return -1;
            }
            pos += rlen;
         } while( (rch-rlen)>0 && rlen>0 );
      }
      else
      {
         hasNext = false;
         pos = -1;
      }
   } while( hasNext );
   
   return pos;
}

int tcp_raw_recv( int sock, char *buf, int maxlen )
{
   int rch;
   do
   {
      rch = read( sock, buf, maxlen );
   } while( rch == -1 && errno == EINTR );

   return rch;
}


bool tcp_wait_recv( int sock, int timeout )
{
   fd_set fds;
   struct timeval to, *pto;
   int rto;

   FD_ZERO( &fds );
   FD_SET( sock, &fds );

   if( timeout != -1 )
   {
      to.tv_sec = timeout/1000;
      to.tv_usec = timeout%1000;
      pto = &to;
   }
   else
   {
      pto = NULL;
   }

   rto = select( sock+1, &fds, NULL, NULL, pto );
   if( rto == 0 || rto == -1 )
   {
      return false;
   }

   return true;
}

bool tcp_wait_send( int sock, int timeout )
{
   fd_set fds;
   struct timeval to, *pto;
   int rto;

   FD_ZERO( &fds );
   FD_SET( sock, &fds );

   if( timeout != -1 )
   {
      to.tv_sec = timeout/1000;
      to.tv_usec = timeout%1000;
      pto = &to;
   }
   else
   {
      pto = NULL;
   }

   rto = select( sock+1, NULL, &fds, NULL, pto );
   if( rto == 0 || rto == -1 )
   {
      return false;
   }

   return true;
}


#if 0

/**
  * 带传送控制头信息的发送函数
  * 支持的头控制类型参见 easysock.h
  * 返回值大于等于0表示成功, -1 表示失败, 可用 errno直接得到错误
  */
int  tcp_ctl_send( int sock, char *sendBuf, int sendLen,
                   int timeout, HeadType headType )
{
   unsigned char aczHeadType[9];
   int  iHeadLen = 0;

   memset( aczHeadType, 0, sizeof(aczHeadType) );


   

   /* 根据头类型产生合适的头信息 */
   switch( headType )
   {
   case NO_HEAD:                   /** 不加任何控制头信息             **/
      break;
   case HEXBYTE_TWO_HAS_HEAD:      /** 两个字节的控制信息(发送长度+2) **/
      if( sendLen < 0 || sendLen > 256*256 - 3 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      aczHeadType[0] = (sendLen+2)/256;
      aczHeadType[1] = (sendLen+2)%256;
      iHeadLen = 2;
      break;
   case HEXBYTE_TWO_NONE_HEAD:
      if( sendLen < 0 || sendLen > 256*256 - 1 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      aczHeadType[0] = sendLen/256;
      aczHeadType[1] = sendLen%256;
      iHeadLen = 2;
      break;
   case ASCII_FOUR_HAS_HEAD:
      if( sendLen < 0 || sendLen > 9999 - 4 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      sprintf( aczHeadType, "%04d", sendLen+4 );
      iHeadLen = 4;
      break;
   case ASCII_FOUR_NONE_HEAD:
      if( sendLen < 0 || sendLen > 9999 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      sprintf( aczHeadType, "%04d", sendLen );
      iHeadLen = 4;
      break;
   case THREE_SYNC_HEXBYTE_TWO_NONE_HEAD:
      errno = ENOSYS;
      return -1;
      break;
   case ASCII_EIGHT_NONE_HEAD:
     if( sendLen < 0 || sendLen > 9999 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      sprintf( aczHeadType, "%08d", sendLen );
      iHeadLen = 8;
      break;
   default:
      errno = EINVAL;
      return -1;
   }

 

   /**
     * 发送长度控制信息
     */
   if( iHeadLen > 0 )
   {
      if( tcp_wait_send( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }
      if( tcp_raw_send( sock, aczHeadType, iHeadLen ) == -1 )
      {
         return -1;
      }
    
    LOG4C(( LOG_INFO, "发送[%s][%d]", aczHeadType, iHeadLen));
   }
   /**
     * 发送实际报文
     */
   if( tcp_wait_send( sock, timeout ) == false )
   {
      errno = ETIMEDOUT;
      return -1;
   }
  
  LOG4C(( LOG_INFO, "发送[%s][%d]", sendBuf, sendLen));
   if( tcp_raw_send( sock, sendBuf, sendLen ) == -1 )
   {
      return -1;
   }

   return sendLen;
}

#endif

/**
  * 带传送控制头信息的发送函数
  * 支持的头控制类型参见 easysock.h
  * 返回值大于等于0表示成功, -1 表示失败, 可用 errno直接得到错误
  */
int  tcp_ctl_send( int sock, char *sendBuf, int sendLen,
                   int timeout, HeadType headType )
{
   unsigned char aczHeadType[9];
   int  iHeadLen = 0;
   char szBuffer[9999];

   memset( aczHeadType, 0, sizeof(aczHeadType) );
   memset(szBuffer, 0, sizeof(szBuffer));

   

   /* 根据头类型产生合适的头信息 */
   switch( headType )
   {
   case NO_HEAD:                   /** 不加任何控制头信息             **/
      break;
   case HEXBYTE_TWO_HAS_HEAD:      /** 两个字节的控制信息(发送长度+2) **/
      if( sendLen < 0 || sendLen > 256*256 - 3 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      aczHeadType[0] = (sendLen+2)/256;
      aczHeadType[1] = (sendLen+2)%256;
      iHeadLen = 2;
      break;
   case HEXBYTE_TWO_NONE_HEAD:
      if( sendLen < 0 || sendLen > 256*256 - 1 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      aczHeadType[0] = sendLen/256;
      aczHeadType[1] = sendLen%256;
      iHeadLen = 2;
      break;
   case ASCII_FOUR_HAS_HEAD:
      if( sendLen < 0 || sendLen > 9999 - 4 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      sprintf( aczHeadType, "%04d", sendLen+4 );
      iHeadLen = 4;
      break;
   case ASCII_FOUR_NONE_HEAD:
      if( sendLen < 0 || sendLen > 9999 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      sprintf( aczHeadType, "%04d", sendLen );
      iHeadLen = 4;
      break;
   case THREE_SYNC_HEXBYTE_TWO_NONE_HEAD:
      errno = ENOSYS;
      return -1;
      break;
   case ASCII_EIGHT_NONE_HEAD:
     if( sendLen < 0 || sendLen > 9999 )
      {
         errno = EMSGSIZE;
         return -1;
      }
      sprintf( aczHeadType, "%08d", sendLen );
      iHeadLen = 8;
      break;
   default:
      errno = EINVAL;
      return -1;
   }


   memcpy(szBuffer, aczHeadType, iHeadLen);
   memcpy(szBuffer+iHeadLen, sendBuf, sendLen);


   /**
     * 发送实际报文
     */
   if( tcp_wait_send( sock, timeout ) == false )
   {
      errno = ETIMEDOUT;
      return -1;
   }
  
  LOG4C(( LOG_INFO, "发送[%s][%d]", szBuffer, sendLen+iHeadLen));
   if( tcp_raw_send( sock, szBuffer, sendLen+iHeadLen) == -1 )
   {
      return -1;
   }

   return sendLen;
}


int  tcp_ctl_recv( int sock, char *recvBuf, int maxRecvLen,
                   int timeout, HeadType headType )
{
   unsigned char aczHeadType[9];
   int  recvLen, pos, readLen;

   memset( aczHeadType, 0, sizeof(aczHeadType) );

   /* 根据头类型产生合适的头信息 */
   switch( headType )
   {
   case NO_HEAD:                   /** 无任何控制头信息,只读一次      **/
      if( tcp_wait_recv( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }
      return tcp_raw_recv( sock, recvBuf, maxRecvLen );
      break;
   case HEXBYTE_TWO_HAS_HEAD:      /** 两个字节的控制信息(发送长度+2) **/
      if( tcp_wait_recv( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }
      if( tcp_raw_recv( sock, aczHeadType, 2 ) == -1 )
      {
         return -1;
      }
      recvLen = aczHeadType[0]*256 + aczHeadType[1] - 2;
      break;
   case HEXBYTE_TWO_NONE_HEAD:
      if( tcp_wait_recv( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }
      if( tcp_raw_recv( sock, aczHeadType, 2 ) == -1 )
      {
         return -1;
      }
      recvLen = aczHeadType[0]*256 + aczHeadType[1];
      break;
   case ASCII_FOUR_HAS_HEAD:
      if( tcp_wait_recv( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }
      if( tcp_raw_recv( sock, aczHeadType, 4 ) == -1 )
      {
         return -1;
      }
      recvLen = atoi(aczHeadType) - 4;
      break;
   case ASCII_FOUR_NONE_HEAD:
      if( tcp_wait_recv( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }
      if( tcp_raw_recv( sock, aczHeadType, 4 ) == -1 )
      {
         return -1;
      }
      recvLen = atoi(aczHeadType);
      break;
   case THREE_SYNC_HEXBYTE_TWO_NONE_HEAD:
      errno = ENOSYS;
      return -1;
      break;
   case ASCII_EIGHT_NONE_HEAD:

    
    LOG4C(( LOG_INFO, "ASCII_EIGHT_NONE_HEAD" ));
      if( tcp_wait_recv( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }
      if( tcp_raw_recv( sock, aczHeadType, 8 ) == -1 )
      {
         return -1;
      }
      
      LOG4C(( LOG_INFO, "接收[%s]11111", aczHeadType ));
      recvLen = atoi(aczHeadType);
      break;
   default:
      errno = EINVAL;
      return -1;
   }
   
   LOG4C(( LOG_INFO, "接收[%s]", aczHeadType ));


   /**
     * 如果应收长度大于缓冲区最大长度, 返回错误代码
     */
   if( recvLen > maxRecvLen )
   {
      errno = EMSGSIZE;
      return -1;
   }

   pos = 0;

   do
   {
      if( tcp_wait_recv( sock, timeout ) == false )
      {
         errno = ETIMEDOUT;
         return -1;
      }

      if( (readLen = tcp_raw_recv( sock, recvBuf+pos, recvLen-pos )) == -1 )
      {
         return -1;
      }

      pos += readLen;
   } while( pos < recvLen );
   LOG4C(( LOG_INFO, "接收[%s][%d]", recvBuf, pos));

   return recvLen;
}

