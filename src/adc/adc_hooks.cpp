/*****************************************************************
*
*    Copyright (C), 2010-2020, NewLand Computer Co., Ltd.
*    Version:  VER1.0
*    Description: ϵͳ�ص�����
*    History:
*
*    Function List:
*            onSysStart : ϵͳ��������
*            onSysEnd: ϵͳ�˳�ʱ����
*            onTxStart: ���յ�CMPƽ̨����ʱ����
*            onTxEnd: ������������Ϻ�Ĵ���ص�
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
 * Description: ϵͳ��������
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
 * Description: ϵͳ�˳�ʱ����
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
 * Description: ���յ�CMPƽ̨����ʱ����
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
   /**���׿�ʼʱ����������***************
   *****************/
   return 0;
}

/*
 *
 * Function: onTxEnd
 * Description: ������������Ϻ�Ĵ���ص�
 * Return:
 * Other:
 */
int onTxEnd( void )
{
   //DBDisconnect();
   /*****���׽����Ͽ����ݿ�����************
   *****************/
   return 0;
}
