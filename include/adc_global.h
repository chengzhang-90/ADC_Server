#ifndef __APPLICATION_GLOBAL_H__
#define __APPLICATION_GLOBAL_H__

#include "apr_pools.h"

/**
  * 全局变量管理
  * 系统并不真正定义一个C语言全局变量, 而是定义一个符号
  */

#define BEGIN_INTERACT   3
#define IN_INTERACT      2
#define END_INTERACT     1
#define NOT_INTERACT     0
#ifdef __cplusplus
extern "C" { 
#endif

extern apr_pool_t *gpstGlobalVariablePool;

void globalInit( void );

int addInt( char *pczName, int iValue );
int getInt( char *pczName, int *piValue );

int addString( char *pczName, char *pczValue );
int getString( char *pczName, char *pczValue );

int addShort( char *pczName, short shValue );
int getShort( char *pczName, short int *pshValue );

int addLong( char *pczName, long lValue );
int getLong( char *pczName, long *plValue );

int addFloat( char *pczName, float fValue );
int getFloat( char *pczName, float *pfValue );

int addDouble( char *pczName, double dValue );
int getDouble( char *pczName, double *pdValue );

int addBinary( char *pczName, void *pValue, int iLen );
int getBinary( char *pczName, void *pValue, int *piLen );

int getLength( char *pczName );

#ifdef __cplusplus
}
#endif

#endif

