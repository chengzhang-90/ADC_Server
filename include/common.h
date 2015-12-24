#include <sys/time.h>
#include <unistd.h>
#include "udo.h"
#include "easysock.h"

#ifndef __COMMON_FUNCTION_H__
#define __COMMON_FUNCTION_H__

#ifdef __cplusplus
extern "C" {
#endif

int GetDateBefore(int eiDateNum ,char * epaczDate, char * opaczDate);
 
int transCMP( char *epczAddr, int eiPort, char *epczSendBuf, int eiSendLen, char *opczRecvBuf, int *opiRecvLen , HeadType enumHead);

int diffDate( char *epczFormat, char *epczBeginDate, char *epczEndDate );

int TestDBConnect();
int DBConnect();

int DBDisconnect();

int diffFloat( double f1, double f2 );

#ifdef __cplusplus
}
#endif

#endif
