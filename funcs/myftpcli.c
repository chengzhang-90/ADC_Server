#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "easysock.h"
#include "myftp.h"
#include "myftpcli.h"

int myerrno;

char *myftp_info( void )
{
   switch( myerrno )
   {
   case LOGIN_CORR:
      return "Login success!";
   case LOGIN_ERR:
      return "Login error!";
   case READ_OPER_SUCC:
      return "Read operator success!";
   case READ_ORG_SUCC:
      return "Read organ success!";
   case INVALID_CMD:
      return "Invalid command!";
   case OPEN_READ_ERR:
      return "Open file to read error!";
   case OPEN_WRITE_ERR:
      return "Open file to write error!";
   case RECV_ERR:
      return "Receive buffer error!";
   case TIMEOUT_ERR:
      return "Receive buffer timeout!";
   case LGET_FSIZE_ERR:
      return "Local get file size!";
   case RGET_FSIZE_ERR:
      return "Remote get file size!";
   case NOT_LOGIN:
      return "Not login, please login first!";
   case READY_RECV:
      return "Ready for receive!";
   case FILE_SIZE:
      return "Return file size";
   case CNT_SVR_ERR:
      return "Connect to myftp server error";
   case LOPEN_READ_ERR:
      return "Open local file to read error!";
   case LOPEN_WRITE_ERR:
      return "Open remote file to write error!";
   case PSEND_FSIZE_ERR:
      return "Put command send file size error!";
   default:
      return "unknown info code";
   }
}

int myftp_login( int server, char *pczOrgan, char *pczOperator, char *pczPass )
{
   char aczCmd[100], aczResp[4];

   return 0;

   /* 发送机构代码 **/
   sprintf( aczCmd, "org %s", pczOrgan );
#ifdef DEBUG
   printf( "%d in %s login send [%s]\n", __LINE__, __FILE__, aczCmd );
#endif
   if( tcp_raw_send( server, aczCmd, strlen(aczCmd) ) == -1 )
   {
      myerrno = SEND_LOGIN_ERR;
      return -1;
   }
   if( !tcp_wait_recv( server, 1000 ) )
   {
      myerrno = TIMEOUT_ERR;
      return -1;
   }
   memset( aczCmd, 0, sizeof(aczCmd) );
   if( tcp_raw_recv( server, aczCmd, sizeof(aczCmd) ) == -1 )
   {
      myerrno = RECV_ERR;
      return -1;
   }
#ifdef DEBUG
   printf( "%d in %s login recv [%s]\n", __LINE__, __FILE__, aczCmd );
#endif
   sprintf( aczResp, "%d", READ_ORG_SUCC );
   if( strncmp( aczResp, aczCmd, 3 ) != 0 )
   {
      myerrno = INVALID_CMD;
      return -1;
   }

   /* 发送操作员 */
   sprintf( aczCmd, "user %s", pczOperator );
#ifdef DEBUG
   printf( "%d in %s login send [%s]\n", __LINE__, __FILE__, aczCmd );
#endif
   if( tcp_raw_send( server, aczCmd, strlen(aczCmd) ) == -1 )
   {
      myerrno = SEND_LOGIN_ERR;
      return -1;
   }
   if( !tcp_wait_recv( server, 1000 ) )
   {
      myerrno = TIMEOUT_ERR;
      return -1;
   }
   memset( aczCmd, 0, sizeof(aczCmd) );
   if( tcp_raw_recv( server, aczCmd, sizeof(aczCmd) ) == -1 )
   {
      myerrno = RECV_ERR;
      return -1;
   }
#ifdef DEBUG
   printf( "%d in %s login recv [%s]\n", __LINE__, __FILE__, aczCmd );
#endif
   sprintf( aczResp, "%d", READ_OPER_SUCC );
   if( strncmp( aczResp, aczCmd, 3 ) != 0 )
   {
      myerrno = INVALID_CMD;
      return -1;
   }

   /* 发送密码 */
   sprintf( aczCmd, "pass %s", pczPass );
#ifdef DEBUG
   printf( "%d in %s login send [%s]\n", __LINE__, __FILE__, aczCmd );
#endif
   if( tcp_raw_send( server, aczCmd, strlen(aczCmd) ) == -1 )
   {
      myerrno = SEND_LOGIN_ERR;
      return -1;
   }
   if( !tcp_wait_recv( server, 1000 ) )
   {
      myerrno = TIMEOUT_ERR;
      return -1;
   }
   memset( aczCmd, 0, sizeof(aczCmd) );
   if( tcp_raw_recv( server, aczCmd, sizeof(aczCmd) ) == -1 )
   {
      myerrno = RECV_ERR;
      return -1;
   }
#ifdef DEBUG
   printf( "%d in %s login recv [%s]\n", __LINE__, __FILE__, aczCmd );
#endif
   sprintf( aczResp, "%d", LOGIN_CORR );
   if( strncmp( aczResp, aczCmd, 3 ) != 0 )
   {
      myerrno = LOGIN_ERR;
      return -1;
   }

   return 0;
}

int myftp_put_file( int server, int iLocalFd, int iFileSize )
{
   char buf[2049], confirm[2];
   int iTotalSend = 0, iRead;
   int nMaxByte=256;

   do
   {
      iRead = (iFileSize-iTotalSend) > nMaxByte? nMaxByte: iFileSize-iTotalSend;
      if( read( iLocalFd, buf, iRead ) == -1 )
      {
#ifdef DEBUG
         printf( "%d in %s read file error: %s\n",
                 __LINE__, __FILE__, strerror(errno) );
#endif
         myerrno = LOPEN_READ_ERR;
         return -1;
      }
      if( tcp_raw_send( server, buf, iRead ) == -1 )
      {
#ifdef DEBUG
         printf( "%d in %s put buffer to server error: %s\n",
                 __LINE__, __FILE__, strerror(errno) );
#endif
         myerrno = PUT_ERROR;
         return -1;
      }
      if( tcp_raw_recv( server, confirm, 1 ) == -1 )
      {
#ifdef DEBUG
         printf( "%d in %s read confirm from server error: %s\n",
                 __LINE__, __FILE__, strerror(errno) );
#endif
         myerrno = PUT_ERROR;
         return -1;
      }
      iTotalSend += iRead;
#ifdef DEBUG
      printf( "%d in %s put buffer len %d\n",
              __LINE__, __FILE__, iRead );
#endif
   } while( iTotalSend < iFileSize );

#if 0
   if( tcp_wait_recv( server, 1000 ) )
   {
      memset( buf, 0, sizeof(buf) );
      if( tcp_raw_recv( server, buf, sizeof(buf)-1) == -1 )
      {
         return -1;
      }
      return 0;
   }
#endif

   return 0;
}

int myftp_get_file( int server, FILE *fp, int iFileSize )
{
   char buf[257], cOK='0';
   int  iTotalRead = 0, iRead, iCanRead;

   if( iFileSize == 0 )
   {
      if( tcp_raw_send( server, &cOK, 1 ) == -1 )
      {
#ifdef DEBUG
         printf( "%d in %s write confirm to remote server error: %s\n",
                 __LINE__, __FILE__, strerror(errno) );
         fflush(stdout);
#endif
         myerrno = GET_ERROR;
printf( "%d in %s : %s\n", __LINE__, __FILE__, strerror(errno) );
         return -1;
      }

      tcp_raw_send( server, "200Finished", 11 );
      return 0;
   }

   do
   {
      iCanRead = (iFileSize-iTotalRead) > 256? 256: iFileSize-iTotalRead;
#ifdef DEBUG
      printf( "%d in %s\n", __LINE__, __FILE__ );fflush(stdout);
#endif
      if( tcp_wait_recv( server, 1000 ) == false )
      {
#ifdef DEBUG
         printf( "%d in %s recv file from timeout, socket %d\n",
                 __LINE__, __FILE__, server );
         fflush(stdout);
#endif
         myerrno = GET_ERROR;
printf( "%d in %s\n", __LINE__, __FILE__ );
         return -1;
      }
      if( (iRead = tcp_raw_recv( server, buf, iCanRead )) == -1 )
      {
#ifdef DEBUG
         printf( "%d in %s recv file from server error: %s\n",
                 __LINE__, __FILE__, strerror(errno) );
         fflush(stdout);
#endif
         myerrno = GET_ERROR;
printf( "%d in %s : %s\n", __LINE__, __FILE__, strerror(errno) );
         return -1;
      }
#ifdef DEBUG
      printf( "%d in %s read len %d\n", __LINE__, __FILE__, iRead );
      fflush(stdout);
#endif
      fwrite( buf, iRead, 1, fp );
      if( ferror( fp ) )
      {
#ifdef DEBUG
         printf( "%d in %s write buffer to local file error: %s\n",
                 __LINE__, __FILE__, strerror(errno) );
         fflush(stdout);
#endif
         myerrno = GET_ERROR;
printf( "%d in %s\n", __LINE__, __FILE__ );
         return -1;
      }

      if( tcp_raw_send( server, &cOK, 1 ) == -1 )
      {
#ifdef DEBUG
         printf( "%d in %s write confirm to remote server error: %s\n",
                 __LINE__, __FILE__, strerror(errno) );
         fflush(stdout);
#endif
         myerrno = GET_ERROR;
printf( "%d in %s : %s\n", __LINE__, __FILE__, strerror(errno) );
         return -1;
      }
      iTotalRead += iRead;
#ifdef DEBUG
      printf( "%d in %s put buffer len %d\n",
              __LINE__, __FILE__, iRead );
      fflush(stdout);
#endif
   } while( iTotalRead < iFileSize );

   tcp_raw_send( server, "200Finished", 11 );

   return 0;
}

int myftp_get( char *svrip, int svrport, char *pczOrgan, char *pczOperator,
               char *pczPass, char *rfile, char *lfile)
{
   int server, iFileSize, ret = 0;
   FILE *fp;
   char aczCmd[200], aczResp[100];

   myerrno = 0;

   server = tcp_connect( svrip, svrport );
   if( server == -1 )
   {
      myerrno = CNT_SVR_ERR;
      return E_CONNECT;
   }

   /* 登录 */
   if( myftp_login( server, pczOrgan, pczOperator, pczPass ) == -1 )
   {
#ifdef DEBUG
      printf( "%d in %s login error %s\n", __LINE__, __FILE__, myftp_info() );
#endif
      myerrno = LOGIN_ERR;
      tcp_close( server );
      return E_LOGIN;
   }

#ifdef DEBUG
   printf( "%d in %s open local file %s\n", __LINE__, __FILE__, lfile );
#endif
   /* 打开本地文件 */
   fp = fopen( lfile, "w" );
   if( fp == NULL )
   {
#ifdef DEBUG
      printf( "%d in %s create local file %s error: %s\n",
              __LINE__, __FILE__, lfile, strerror(errno) );
#endif
      myerrno = LOPEN_WRITE_ERR;
      tcp_raw_send( server, "quit", 4 );
      tcp_close( server );
      return E_OPENFILE;
   }

   sprintf( aczCmd, "get %s", rfile );
#ifdef DEBUG
   printf( "%d in %s send command %s\n", __LINE__, __FILE__, aczCmd );
#endif
   if( tcp_raw_send( server, aczCmd, strlen(aczCmd) ) == -1 )
   {
      myerrno = GET_ERROR;
#ifdef DEBUG
   printf( "%d in %s send command error: %s\n",
           __LINE__, __FILE__, strerror(errno) );
#endif
      tcp_raw_send( server, "quit", 4 );
      tcp_close( server );
      fclose( fp );
      return E_GET;
   }

#ifdef DEBUG
   printf( "%d in %s wait response\n", __LINE__, __FILE__ );
#endif
   if( !tcp_wait_recv( server, 1000 ) )
   {
#ifdef DEBUG
      printf( "%d in %s wait response timeout\n", __LINE__, __FILE__ );
#endif
      myerrno = GET_ERROR;
      tcp_close( server );
      fclose( fp );
      return E_GET;
   }
   memset( aczCmd, 0, sizeof(aczCmd) );
   if( tcp_raw_recv( server, aczCmd, sizeof(aczCmd) ) == -1 )
   {
#ifdef DEBUG
      printf( "%d in %s read response error: %s\n",
              __LINE__, __FILE__, strerror(errno) );
#endif
      myerrno = GET_ERROR;
      tcp_close( server );
      fclose( fp );
      return E_GET;
   }

#ifdef DEBUG
   printf( "%d in %s response %s\n", __LINE__, __FILE__, aczCmd );
#endif
   sprintf( aczResp, "%d", FILE_SIZE );
   if( strncmp( aczCmd, aczResp, 3 ) != 0 )
   {
      aczCmd[3] = 0;
      myerrno = atoi( aczCmd );
      tcp_close( server );
      fclose( fp );
      return E_GET;
   }

   iFileSize = atoi(&aczCmd[3]);
#ifdef DEBUG
   printf( "%d in %s file size %d\n", __LINE__, __FILE__, iFileSize );
#endif

   if( tcp_raw_send( server, "corr", 4 ) == -1 )
   {
      myerrno = GET_ERROR;
      tcp_close( server );
      fclose( fp );
      return E_GET;
   }
#ifdef DEBUG
   printf( "%d in %s begin to put file\n", __LINE__, __FILE__ );
#endif
   ret = myftp_get_file( server, fp, iFileSize );

   fclose( fp );
   tcp_close( server );

   return ret;
}

/**
  * 将本地文件存放到远端服务器
  */
int myftp_put( char *svrip, int svrport, char *pczOrgan, char *pczOperator,
               char *pczPass, char *lfile, char *rfile )
{
   int server, localFd, ret = 0;
   struct stat stStat;
   char aczCmd[200], aczResp[100];

   myerrno = 0;

   server = tcp_connect( svrip, svrport );
   if( server == -1 )
   {
      myerrno = CNT_SVR_ERR;
      return E_CONNECT;
   }
   /* 检查本地文件是否存在 */
   if( lstat( lfile, &stStat ) == -1 )
   {
      myerrno = LGET_FSIZE_ERR;
      return E_PUT;
   }

   /* 登录 */
   if( myftp_login( server, pczOrgan, pczOperator, pczPass ) == -1 )
   {
#ifdef DEBUG
      printf( "%d in %s login error %s\n", __LINE__, __FILE__, myftp_info() );
#endif
      myerrno = LOGIN_ERR;
      tcp_close( server );
      return E_LOGIN;
   }


#ifdef DEBUG
   printf( "%d in %s open local file %s\n", __LINE__, __FILE__, lfile );
#endif
   /* 打开本地文件 */
   localFd = open( lfile, 0 );
   if( localFd == -1 )
   {
#ifdef DEBUG
      printf( "%d in %s open local file %s error %s\n",
              __LINE__, __FILE__, lfile, strerror(errno) );
#endif
      myerrno = PUT_ERROR;
      tcp_raw_send( server, "quit", 4 );
      tcp_close( server );
      return E_OPENFILE;
   }

   sprintf( aczCmd, "put %s %d", rfile, stStat.st_size );
#ifdef DEBUG
   printf( "%d in %s send command %s\n", __LINE__, __FILE__, aczCmd );
#endif


   if( tcp_raw_send( server, aczCmd, strlen(aczCmd) ) == -1 )
   {
      myerrno = PUT_ERROR;
      tcp_raw_send( server, "quit", 4 );
      tcp_close( server );
      close( localFd );
      return E_PUT;
   }

#ifdef DEBUG
   printf( "%d in %s wait response\n", __LINE__, __FILE__ );
#endif
   if( !tcp_wait_recv( server, 1000 ) )
   {
#ifdef DEBUG
      printf( "%d in %s wait response timeout\n", __LINE__, __FILE__ );
#endif
      myerrno = PUT_ERROR;
      tcp_close( server );
      close( localFd );
      return E_PUT;
   }
   memset( aczCmd, 0, sizeof(aczCmd) );
   if( tcp_raw_recv( server, aczCmd, sizeof(aczCmd) ) == -1 )
   {
#ifdef DEBUG
      printf( "%d in %s read response error: %s\n",
              __LINE__, __FILE__, strerror(errno) );
#endif
      myerrno = PUT_ERROR;
      tcp_close( server );
      close( localFd );
      return E_PUT;
   }

#ifdef DEBUG
   printf( "%d in %s response %s\n", __LINE__, __FILE__, aczCmd );
#endif
   sprintf( aczResp, "%d", READY_RECV );
   if( strncmp( aczCmd, aczResp, 3 ) != 0 )
   {
      myerrno = PUT_ERROR;
      tcp_close( server );
      close( localFd );
      return E_PUT;
   }

#ifdef DEBUG
   printf( "%d in %s begin to put file\n", __LINE__, __FILE__ );
#endif
   ret = myftp_put_file( server, localFd, stStat.st_size );

   close( localFd );
   tcp_close( server );

   return ret;
}

