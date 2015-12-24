#ifndef __MY_FTP_CLIENT_H__
#define __MY_FTP_CLIENT_H__

#include "myftp.h"

#ifdef __cplusplus
extern "C" {
#endif

static const int E_CONNECT  = -1;
static const int E_PUT      = -2;
static const int E_GET      = -3;
static const int E_OPENFILE = -4;
static const int E_LOGIN    = -5;

int myftp_put( char *svrip, int svrport, char *pczOrgan, char *pczOperator,
               char *pczPass, char *lfile, char *rfile );

int myftp_get( char *svrip, int svrport, char *pczOrgan, char *pczOperator,
               char *pczPass, char *rfile, char *lfile );

#ifdef __cplusplus
}
#endif

extern int myerrno;

#endif
