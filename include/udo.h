#include <stdio.h>
#include <stdarg.h>

#ifndef __ADC_PAGE_BLOCK_FIELD_STORAGE_UDO_H__
#define __ADC_PAGE_BLOCK_FIELD_STORAGE_UDO_H__

/*
 * 扩充老版本UDO。当前版本
 * 新版本特征：
 * 1 支持UDO_AddField自动扩充内存
 * 2 UDO_Startup必须是第一个被调用的函数
 * 3 UDO_End支持清理所有未被释放的内存 
 */

typedef struct _UDOPage UDOPage;

/* 第一页中的iFieldNum表示整个UDO页链中的域个数, 以后的页表示该页是否使用 */

typedef union _Var
{
   char   cVal;
   short  shVal;
   int    iVal;
   long   lVal;
   float  fVal;
   double dVal;
   char   *pcVal;
} Var;

typedef struct _UDOValDesc UDOValDesc;

typedef struct _UDOFieldDesc UDOFieldDesc;

#define UDO_TOO_SMALL	           -1
#define UDO_NO_FIELD	               -2
#define UDO_INVALID_OCCUR           -3
#define UDO_OVER_RANGE              -4
#define UDO_MATCH_ERROR	           -5
#define UDO_TYPE_ERR                -6
#define UDO_FIELD_LEN_TOO_LONG      -7

#define STRTYPE		0
#define SHORTTYPE	1
#define INTTYPE		2
#define LONGTYPE        3
#define FLOATTYPE	4
#define DOUBLETYPE	5
#define BINTYPE         6
#define CHARTYPE	7
#define OTHERTYPE       100

#define UDT_STRING	STRTYPE
#define UDT_SHORT	SHORTTYPE
#define UDT_INT		INTTYPE
#define UDT_LONG 	LONGTYPE
#define UDT_DOUBLE	DOUBLETYPE
#define UDT_FLOAT	FLOATTYPE
#define UDT_CHAR		CHARTYPE
#define UDT_BIN	    BINTYPE
#define UDT_OTHER    OTHERTYPE

#define SIZE_CHAR	sizeof(char)
#define SIZE_SHORT	sizeof(short)
#define SIZE_INT	sizeof(int)
#define SIZE_LONG	sizeof(long)
#define SIZE_FLOAT	sizeof(float)
#define SIZE_DOUBLE	sizeof(double)
#ifdef __cplusplus
extern "C" {
#endif
typedef char * UDP;

/*
**功能:
**输入:1
**     2
**输出:
**返回:
*/
void  UDO_Init( UDP epfAddr, int eiSize );

/*
**功能:分配UDP
**输入:1
**输出:
**返回:NULL-失败,其它-
*/
UDP UDO_Alloc( int eiSize );

/*
**功能:在UDP类型空间增加一个域值,如果域不存在,增加该域并保存值,
**输入:1 UDP
**     2
**     3
**     4
**     5
**输出:
**返回:0-成功,其它-
*/
int UDO_AddField( UDP epfAddr, int eiFieldId, int eiFieldType,
                 char * epValue, int eiValLen );

/*
**功能:
**输入:1 UDP
**     2
**     3
**输出:1
**     2
**返回:0-成功,其它-
*/
int UDO_GetField( UDP epfAddr, int eiFieldId, int eiFieldOccu,
                 char * opValue, int *opValLen );

/*
**功能:打印UDP
**输入:1 UDP
**输出:
**返回:
*/
void UDO_Print( UDP epfAddr );

/*
**功能:
**输入:1 UDP
**     2
**输出:
**返回: >0重复次数, <0
*/
int UDO_FieldOccur( UDP epfAddr, int eiFieldId );

/*
**功能:
**输入:1 UDP
**输出:
**返回: >=0
*/
int UDO_FieldCount( UDP epfAddr );

/*
**功能:取UDO
**输入:1 UDP
**输出:
**返回: >=0
*/
int UDO_PageSize( UDP epfAddr );

/*
**功能:取UDO
**输入:1 UDP
**输出:
**返回: >=0UDO
*/
int UDO_PageActualSize( UDP epfAddr );

/*
**功能:取UDO链中各页实际大小之和，可能会因为UDO_ChgField与各个值大小之和不等 
**输入:1 UDP
**输出:
**返回: >=0UDO
*/
int UDO_ValueSize( UDP epfAddr );

/*
**功能:取UDO链所有页大小之和 
**输入:1 UDP
**输出:
**返回: >=0
*/
int UDO_Size( UDP epfAddr );

/*
**功能:释放UDP
**输入:1 UDP
**输出:
**返回:
*/
void UDO_Free( UDP epfAddr );

/*
 * 功能: 释放所有UDO页动态申请的空间 
 */
void UDO_FreeAll();

/*
**功能:
**输入:1 FS
**     2
**输出:

**返回:0-不存在,1-
*/
int UDO_FieldExist( UDP epfAddr, int eiFieldId );

/*
**功能:
**输入:1 UDP
**     2
**输出:
**返回:>0,域类型，<0
*/
int UDO_FieldType( UDP epfAddr, int iFieldId );

/*
**功能:
**输入:1 UDP
**     2
**输出:1
**返回:0-成功，其他失败（超出UDP中域序号，编号从0
*/
int UDO_FieldId( UDP epfAddr, int eiFldNo, int *opiFldId );

/*
**功能:将一个UDP空间附加在另外一个UDP
**输入:1 目标UDP
**     2 源UDP
**输出:1 目标UDP
**返回:0-成功，其他失败（目标UDP
*/
int UDO_Append( UDP eopfTargetAddr, UDP epfSourceAddr );

/*
**功能:UDP
**输入:1 源UDP
**输出:1 目标UDP
**返回:0-成功,其他失败(目标空间太小)
*/
int UDO_Copy( UDP opfTargetAddr, UDP epfSourceAddr );

/*
**功能:
**输入:1 UDP
**     2
**     3
**输出:
**返回:>=0正确,域值长度,其他错误,
*/
int UDO_FieldLen( UDP epfAddr, int eiFieldId, int eiOccur );

/*
**功能:
**输入:1 UDP
**     2
**     3
**     4
**输出:1
**     2
**返回:0-成功,其它-
*/
int UDO_SafeGetField( UDP epfAddr, int eiFieldId, int eiFieldOccu,
                     int eiType, char * opValue, int *opiValLen );


/*
**功能:改变指定域指定重复次数的值,如果没有该域返回错,
**
**输入:1 UDP
**     2 域ID
**     3
**     4
**     5
**输出:
**返回:  0-成功,其他失败(空间大小不够或者无此域)
*/
int UDO_ChgField( UDP epfAddr, int eiFieldId, int eiFieldOccu,
                 char *epcValue, int eiFieldLen );

/*
**funtion:get the first field by the fieldid and return the next field address
**input:1 UDP address
**      2 field id
**output:1 field value
**       2 field len
**       3 second field address offset
**return:0-success,other-error,see also UDO_GetField
*/
int UDO_GetFirstField( UDP epfAddr, int eiFieldId, char *opcValue,
                      int *opiLen, int *opiSeconddOffset );

void UDO_FilePrint( UDP epfAddr, FILE *fp );

/*
**function:get the current field value and next field address
**input: 1 pfs address
**       2 current field offset
**output:1 current field value
**       2 current field len
**       3 next field address
**return:0-correct,other-error,see also UDO_GetField
*/
int UDO_GetCurrField( UDP epfAddr, int eiCurrFielOffset, char *opcValue,
                     int *opiValLen, int *opiNextFieldOffset );

void UDO_FPrint( char * epczFileName, UDP epfAddr );

/*
 * 功能: 释放所有UDO页动态申请的空间 
 */
void UDO_FreeAll();

/* @UDO_SeekFieldDesc
 * #查找指定域号的域描述符
 * %参数1 域描述符页地址指针                输入输出参数 
 * %参数2 域号                              输入参数 
 * %参数3 最后一个域描述符地址指针          输出参数 NULL--表示UDO页链为空 
 * #NULL--无指定域号的域 其他--指定域描述符指针
 * #只有当返回值为NULL时输出参数opstLastFldDesc才有意义 
 */
UDOFieldDesc *UDO_SeekFieldDesc( UDOPage **eopstPage, int eiFieldId, UDOFieldDesc **opstLastFldDesc );

UDP UDO_Merge( UDP *opfDest, UDP epfSrc );

/*
 * 将UDO转换为字符串形式
 */
int UDO_sprintf (UDP epfAddr, char * opcBuf, int *opiLen );

/* 从字符串中恢复UDO,请不要使用其他非UDO转换的字符串 */
int UDO_sscanf (char * epcBuf, int eiLen, UDP opfAddr);

int UDO_ACField( UDP epfSrc, int eiFldId, int eiType, char *epValue,
                 int eiLen );
int UDO_Prt2File( int eiLevel, char * epczFileName, UDP epfAddr, ... );

#define UDO_PAGE_SIZE 307200

#ifdef __cplusplus
};
#endif

#endif

