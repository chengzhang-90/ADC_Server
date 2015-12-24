/*
**  file: pub_base.h
*/
#include <sys/time.h>
#include <unistd.h>

#ifndef __PUB_BASE__
#define __PUB_BASE__

#ifdef __cplusplus
extern "C" {
#endif

/* 去字符串左边指定字符，pczSrc是以'\0'结束的字符串 */
void PUB_StrLeftTrim( char * epczStr, char ecCh );

/* 去字符串右边指定字符，pczSrc是以'\0'结束的字符串 */
void PUB_StrRightTrim( char * epczStr, char ecCh );

/* 去字符串左右两边指定字符，pczSrc是以'\0'结束的字符串 */
void PUB_StrTrim( char * epczStr, char ecCh );

/*
** 功能：字符串全部变大写
** 输入参数：1 输入待操作的字符串
** 输出参数：1 输出大写字符串
** 返回值：    无
*/
void PUB_StrUpper( char *eopczStr );

/*
** 功能：字符串全部变小写
** 输入参数：1 输入待操作的字符串
** 输出参数：1 输出小写字符串
** 返回值：    无
*/
void PUB_StrLower( char *eopczStr );

void PUB_UpSort( int eiNum, int *piArray );

long PUB_CalcTimeVal( struct timeval stTimeBegin, struct timeval stTimeEnd );

/**
 * 功能: 字符串按分隔符分解成字符串矢量
 * 参数: char * epczStr  - 待分解字符串
 *char   chSep    - 分隔符
 *char **oppczRet - 字符串指针数组,指向各矢量字符串
 *u_int  max_flds - 最大分解矢量数,为0完全分解整个字符串
 * 返回: 矢量个数
 * 备注: 注意,此函数会将原字符串所有或max_flds个分隔符替换成字符'\0'
**/
int PUB_ParseStr(char *epczStr, char chSep, char *oppczRet[],
                 unsigned int max_flds);

#ifdef __cplusplus
}
#endif

#endif

/*
** end of file: pub_base.h
*/
