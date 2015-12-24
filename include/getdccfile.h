#ifndef __GET_FILE_FROM_DCC_H__
#define __GET_FILE_FROM_DCC_H__

#ifdef __cplusplus
extern "C" {
#endif

int ADP_FileTrans( int iPort, char *epczAddr, char *epczSendBuf,
                   int eiSendLen, char *opczRecvBuf, int *opiRecvLen );

/**
 *�õ�DCC�ļ���**
  * ����ֵ: 0     �ɹ���������
  *         -1    ������
  *         ����  ʧ��
**/
int getDccFile( char *epczYesterday, char *epczClassNo,
                long elNewSerial, char *opczDccFile, char *org, char *oper, 
		char *epczTransCode, long lTrace);

/**
  * ��DCCȡ��ˮ��ϸ��Ϣ
  * ����ֵ: 0     �ɹ���������
  *         -1    ������
  *         ����  ʧ��
  */
int getDccDetail( char *epczYesterday, char *epczClassNo,
                  long elNewSerial, long elOldSerial, double *opdAmt );

#ifdef __cplusplus
}
#endif

#endif
