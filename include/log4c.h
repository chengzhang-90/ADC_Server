#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#ifndef __LOG_4_C_LANGUAGE_H__
#define __LOG_4_C_LANGUAGE_H__

#undef LOG_FATAL
#define LOG_FATAL  0

#undef LOG_WARN
#define LOG_WARN   1

#undef LOG_INFO
#define LOG_INFO   2

#undef LOG_DEBUG
#define LOG_DEBUG  3

struct log4c_logInfo
{
    char log_iniFile[64];
    char logFileSuffix[64];
    char serial[16];
    char usertag[64];
};

#ifdef __cplusplus
extern "C" {
#endif


void log4c_open( void );

void log4c_close( void );

void log4c_init( char *config );

FILE *log4c_fp ();

void log4c_chgIntertag (const char * fname, const char * func, int lineno);

void log4c_chgUsertag (const char * serial, const char * usertag);

const char * log4c_logFileSuffix ();

void log4c_chgLogFileSuffix (const char *sufffix);

void log4c_logInfo (struct log4c_logInfo *log_info);

const char *log4c_logCurFileName ();

void log4c( int level, char *fformat, ... );

int log4c_IsLog (int level);

#ifdef __cplusplus
}
#endif

#endif
