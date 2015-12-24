#include <sys/time.h>
/*
#include <rpc/rpc.h>
*/
#ifndef __GLOBAL__
#define __GLOBAL__
#define CODEING

/*保存数据库连接的连接号*/
extern int iConnectId;

/*保留变量，用于特殊用途，目前用来标识证券类型*/
extern int iReserveVal;

/**保存配置的共享内存指针**/
extern char * pcShmAddr;

extern char gaczTranCode[10];

/****渠道上送流水号****/
extern char gaczAtmSerial[7];

/****渠道上送自助设备流水号****/
extern char gaczDevSerial[11];

/****代收交易第二帐号****/
extern char gaczSecAcct[29];

/* 设备编号 */
extern char gaczDevNo[5];

#define DBUSER  ""
#define PASSWD  ""

#define PKG_REQ    1
#define PKG_RESP   2
#define PKG_CONF   3


extern struct timeval stBegin, stCurr;
extern struct timeval stPkgBegin, stPkgEnd;
extern long   lTotalTime;
extern int    iSubSvcTime;

/*
** 用于设置Aix到SCO数据类型转换的标志
** 0-不需转换, 1-需要转换
** 该标志在打包拆包程序中读取，在适配器中设置，打包前置为1，拆包后再置为0
*/
extern int iScoFlag;

/* 用于对于需要转换为网络字节数据的进行控制
** 0-不需转换, 1-需要转换
** 该标志在打包拆包程序中读取，在适配器中设置，打包前置为1，拆包后再置为0 
** 浮点数据不支持
*/
extern int iNetTranFlag;
extern char  aczDwbhCheck[7];
#endif
