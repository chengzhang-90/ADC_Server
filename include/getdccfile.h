#ifndef __GET_FILE_FROM_DCC_H__
#define __GET_FILE_FROM_DCC_H__

#ifdef __cplusplus
extern "C" {
#endif

int ADP_FileTrans( int iPort, char *epczAddr, char *epczSendBuf,
                   int eiSendLen, char *opczRecvBuf, int *opiRecvLen );

/**
 *得到DCC文件名**
  * 返回值: 0     成功，有数据
  *         -1    无数据
  *         其他  失败
**/
int getDccFile( char *epczYesterday, char *epczClassNo,
                long elNewSerial, char *opczDccFile, char *org, char *oper, 
		char *epczTransCode, long lTrace);

/**
  * 从DCC取流水详细信息
  * 返回值: 0     成功，有数据
  *         -1    无数据
  *         其他  失败
  */
int getDccDetail( char *epczYesterday, char *epczClassNo,
                  long elNewSerial, long elOldSerial, double *opdAmt );

#ifdef __cplusplus
}
#endif

#endif
