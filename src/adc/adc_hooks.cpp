/*****************************************************************
*
*    Copyright (C), 2010-2020, NewLand Computer Co., Ltd.
*    Version:  VER1.0
*    Description: 系统回调函数
*    History:
*
*    Function List:
*            onSysStart : 系统启动处理
*            onSysEnd: 系统退出时处理
*            onTxStart: 接收到CMP平台请求时处理
*            onTxEnd: 交易请求处理完毕后的处理回调
*
*    date:   2010-7-20
*    author: zhou hong di
*    created: zhou hong di
*
*    date:
*    author:
*    modify description
*
*****************************************************************/
#include "stdlib.h"
#include "adc_handle.h"
#include "log.h"
#include <ctype.h>
#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int DBConnect(void);
extern int DBDisconnect(void);

#ifdef __cplusplus
}
#endif
/*
 *
 * Function: onSysStart
 * Description: 系统启动处理
 * Return:
 * Other:
 */
int onSysStart( char *pczExecFile )
{
   return DBConnect();
   return 0;
}

/*
 *
 * Function: onSysEnd
 * Description: 系统退出时处理
 * Return:
 * Other:
 */
int onSysEnd( void )
{
   DBDisconnect();
   return 0;
}

/*
 *
 * Function: onTxStart
 * Description: 接收到CMP平台请求时处理
 * Return:
 * Other:
 */
int onTxStart( char *pczExecFile )
{
   if( TestDBConnect() != 0 )
   {
       DBDisconnect();
       usleep(500000);
       DBConnect();
   }
   /**交易开始时候重新连接***************
   *****************/
   return 0;
}

/*
 *
 * Function: onTxEnd
 * Description: 交易请求处理完毕后的处理回调
 * Return:
 * Other:
 */
int onTxEnd( void )
{
   //DBDisconnect();
   /*****交易结束断开数据库连接************
   *****************/
   return 0;
}
