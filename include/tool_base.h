#include <stdarg.h>
#include <string.h>
#ifndef _TOOL_TRACE_H_
#define _TOOL_TRACE_H_

#ifndef SYS_HAED_ERR
#define SYS_HAED_ERR   1
#endif

#ifndef SYS_LOGIC_ERR
#define SYS_LOGIC_ERR  2
#endif

#ifndef KEY_DEBUG
#define KEY_DEBUG      3
#endif

#ifndef GENL_DEBUG
#define GENL_DEBUG     4
#endif

/*
**  错误代码定义。
*/
#define TOOL_FILE_OPEN_ERROR         -501 
#define TOOL_FILE_SEEK_ERROR         -502
#define TOOL_RENAME_ERROR            -503


/*
**  常量定义
*/
#define TOOL_DATE_AND_TIME_LEN       30
#define TOOL_COMMON_MSG_LEN          2048
#define TOOL_MAX_MSG_LEN	     4096
#define TOOL_LINE_LENGTH	     80
#define TOOL_FILE_NAME_LEN           80

#define TOOL_FILE_SIZE               2000000

#define TOOL_SHMKEYFILE  "Shm.key"
#define TOOL_ERR_SHMKEYFILE   "ErrShm.key"
#define TOOL_ERR_MSGKEYFILE   "ErrMsg.key"
#define TOOL_SEMKEYFILE  "Sem.key"

#define TOOL_MULT_SEMKEYFILE  "MultSem.key"
#define TOOL_MAX_SUB_MESG  300

#define TOOL_KEYID  8
#define TOOL_PATH_LEN   80
#define TOOL_SQNO_LEN   12
#define TOOL_NODE_GAP   10

#define TOOL_SYS_TRACE_LEVEL      0
#define TOOL_DISPLAY_TRACE_LEVEL  1
#define TOOL_DATABASE_TRACE_LEVEL 2
#define TOOL_TRAN_TRACE_LEVEL     3

#define TOOL_LOG_LEVEL0    TOOL_SYS_TRACE_LEVEL
#define TOOL_LOG_LEVEL1    TOOL_DISPLAY_TRACE_LEVEL
#define TOOL_LOG_LEVEL2    TOOL_DATABASE_TRACE_LEVEL
#define TOOL_LOG_LEVEL3    TOOL_TRAN_TRACE_LEVEL

#ifdef DISPLAY
#undef DISPLAY
#endif
#define DISPLAY 1

#ifdef TRACE
#undef TRACE
#endif

/*全局变量,保存可执行文件名字,为TOOL_Trace提供按可执行文件记日志的功能*/

typedef struct _TOOL_LogInfo
{
   int iPid;                    /* 当前调用进程的PID                */
   int iCanWrite;               /* 判断当前指定的日志文件是否能打开 */
   int iLogLevel;               /* 环境变量TRACE_LEVEL指定的指定的值*/
   char aczFileName[256];       /* TOOL_Log使用的文件名             */
   char aczStanId[64];          /* 流水号                           */
} TOOL_LogInfo;

#define TRACE 1

#ifdef __cplusplus
extern "C" {
#endif

typedef void ( *SigFunc )( int );

/*
**  保存错误代码的全局变量定义
*/
extern int iTOOL_ErrorNO;

extern char *pczExecFileName;

/* 
**  外部函数说明
*/
void TOOL_LogInit( char *epczSerial, char *epczBussInfo );

void TOOL_Log( int eiTraceLevel, char *epcMessage, ... );

void TOOL_Trace( int eiTraceLevel, 
                 char *epcFileName, char *epcMessage, ... );

void TOOL_Dump( int eiTraceLevel,
                char *epcFileName,
                unsigned char *epbyDumpedData,
                int eiDumpedDataSize );

void TOOL_Debug( char *epcFileName, char *epcMessage, ... );

void TOOL_DebugDump( char *epcFileName,
                     unsigned char *epbyDumpedData,
                     int eiDumpedDataSize );

void TOOL_GetCurrTime( char *epcDateAndTime );

void TOOL_GetTimeString( char *epcTime );


void TOOL_BinaryDump( int eiFileDes,
                      unsigned char *epbyMessage,
                      int eiDumpedDataSize );

void TOOL_Message( int eiTraceLevel,
                   char *epcSender, char *epcMessage );


/*
**  去除字符串前后空格 
*/
char *TOOL_StripSpace( char *);

/*
** 产生供消息队列、信号灯、共享内存是用的 KEY 值
*/
/*extern key_t TOOL_Ftok ( char * );*/

/*
**   取每一个交易请求消息传递用的消息标识
*/
int TOOL_GetMsgType( int , long *);

/*
**  复杂信号管理
*/
SigFunc TOOL_Signal( int , SigFunc );


/*
**   日志函数
*/
void TOOL_Printf(va_list, ...);

/*
**    设置忽略信号集
*/
int TOOL_IgnSignal ( void );

/*
** 取精确时间
*/
char *TOOL_TimeTell( char * );

/*
**  取时间到秒 hh:mm:ss
*/
void TOOL_GetTime ( char * );

/*
**  取时间到毫秒 hh:mm:ss:ms
*/
void TOOL_GetMicroTime ( char * );

#ifdef __cplusplus
}
#endif


#endif
