#include <sys/types.h>
#include <unistd.h>

#include "apr.h"
#include "apr_pools.h"
#include "apr_network_io.h"
#include "apr_shm.h"
#include "apr_signal.h"
#include "apr_hash.h"
#include "adc_base.h"
#include "udo.h"

#ifndef __APPLICATION_HANDLER_H__
#define __APPLICATION_HANDLER_H__

#ifdef __cplusplus
extern "C" {
#endif

int onSysStart( char * );  
int onSysEnd( void );     
int onTxStart( char * ); 
int onTxEnd( void );    
int preCheck( char *pczBuffer, int iLen );
int postCheck( UDP pUdo );
int recvBuffer( char **pczBuffer, apr_size_t *ptRecvLen );
int sendBuffer( char *pczBuffer, apr_size_t iSendLen );
int getTxCode( char *pczBuffer, char *pczTxCode );

#ifdef __cplusplus
}
#endif

#endif

