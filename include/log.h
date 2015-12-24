#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#ifndef __SIMPILE_LOG_H__
#define __SIMPILE_LOG_H__

#include "log4c.h"

#define LOG4C_INIT( config ) log4c_init( config )

#define LOG4C_OPEN( )        log4c_open( )

#define LOG4C_CLOSE( )       log4c_close( )

#define LOG4C_FP( )          log4c_fp( )

#define LOG4C_FILENAME()     log4c_logCurFileName ()

#define LOG4C_SUFFIX()       log4c_logFileSuffix ()

#define LOG4C_CHGSUFFIX(suffix) log4c_chgLogFileSuffix ((const char *)(suffix))

#define LOG4C_CHGUSERTAG(serial, usertag) log4c_chgUsertag((const char *)(serial), (const char *)(usertag))

#define LOG4C_LOGINFO(loginfo) log4c_logInfo ((loginfo))

#define LOG4C_ISLOG(level) log4c_IsLog ((level))

#define LOG4C( info )\
{\
      log4c_chgIntertag (__FILE__, __FUNCTION__, __LINE__);\
      log4c info ;\
}

#endif
