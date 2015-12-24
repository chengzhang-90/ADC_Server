#ifndef _ASM_OPERDEF_H_
#define _ASM_OPERDEF_H_

typedef struct _operate_code
{
   char m_sId[5];           /* 操作码ID */
   int  (*pfnFun)( char *opczBuf, int *opiBufLen );
   int  m_nInType;          /* 输入类型0-定长 1-变长 */
   int  m_nInAB;            /* 输入字段类型 0-ASCII 1-BCD */
   int  m_nInLen;           /* 输入字段长度 */
   int  m_nInLenType;       /* 输入字段长度字段类型 */
   int  m_nInLenLen;        /* 输入字段长度字段长度 */
   int  m_nOutType;         /* 输出类型0-定长 1-变长 */
   int  m_nOutAB;           /* 输出字段类型 0-ASCII 1-BCD */
   int  m_nOutLen;          /* 输出字段长度 */
   int  m_nOutLenType;      /* 输出字段长度字段类型 */
   int  m_nOutLenLen;       /* 输出字段长度字段长度 */
} OperateCode;

EXEC SQL BEGIN DECLARE SECTION;
typedef struct _flow_info
{
   char       m_sProcode[11];
   char       m_sOperCode[3];
   char       m_sOperHintInd[3];
   char       m_sCryptType[3];
   char       m_sOperData[101];
   int        m_nSerialNo;
   char       m_sOperType[2];
   char       m_sStatus[2];
   char       m_sDac[17];
}FlowInfo;
typedef struct _flow_info_ind
{
   short      m_nProcode;
   short      m_nOperCode;
   short      m_nOperHintInd;
   short      m_nCryptType;
   short      m_nOperData;
   short      m_nSerialNo;
   short      m_nOperType;
   short      m_nStatus;
   short      m_nDac;
}FlowInfoInd;
EXEC SQL END DECLARE SECTION;

#endif
