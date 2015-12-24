#ifndef __EASY_SOCKET_H__
#define __EASY_SOCKET_H__

#ifndef __cplusplus
enum MYBOOL { false = 0, true };
typedef enum MYBOOL bool;
#endif

typedef enum _HeadType {
   NO_HEAD = 0,
   HEXBYTE_TWO_HAS_HEAD,
   HEXBYTE_TWO_NONE_HEAD,
   ASCII_FOUR_HAS_HEAD,
   ASCII_FOUR_NONE_HEAD,
   THREE_SYNC_HEXBYTE_TWO_NONE_HEAD,
   ASCII_SIX_NONE_HEAD,
   ASCII_EIGHT_NONE_HEAD
                      } HeadType;

#ifdef __cplusplus
extern "C" {
#endif

int tcp_bind( int port );

int tcp_connect(char *host, int port);

int tcp_accept( int sock, int waittime );

int tcp_send( int sock, char *buf, int len );

int tcp_recvbuf( int socket, char **buffer, int bufferlen, int timeout );

int tcp_sendbuf( int socket, char *pcBuffer, int len, int timeout );

int tcp_recv( int sock, char *buf, int maxlen );

int tcp_raw_send( int sock, char *buf, int len );
int tcp_raw_send_ebc( int sock, char *buf, int len );

int tcp_raw_recv( int sock, char *buf, int maxlen );
int tcp_raw_recv_ebc( int sock, char *buf, int maxlen, int *iOriRead );

int tcp_close( int sock );

bool tcp_wait_recv( int sock, int timeout );

bool tcp_wait_send( int sock, int timeout );

int  tcp_ctl_send( int sock, char *sendBuf, int sendLen,
                   int timeout, HeadType headType );

int  tcp_ctl_recv( int sock, char *recvBuf, int maxRecvLen,
                   int timeout, HeadType headType );

#ifdef __cplusplus
}
#endif

#endif
