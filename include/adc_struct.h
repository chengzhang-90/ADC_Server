
#ifndef __ADC_STRUCT_H__
#define __ADC_STRUCT_H__

EXEC SQL BEGIN DECLARE SECTION;

typedef struct _SysCtl
{
   char m_BankCode[4];     /* ���д��� */
   int m_NodeId;           /* �м�ҵ��ڵ�� */
   char m_WorkDate[9];     /* �������� */
   char m_LastWorkDate[9]; /* �ϴι������� */
   int m_CurSerial;        /* �м�ҵ��ƽ̨��ˮ�� */
   char m_RunningFlag[2];  /* �м�ҵ��ϵͳ״̬ */
   char m_State[2];        /* ״̬ */
   char m_TxUnit[10];      /* ���׻����� */
   char m_TxKind[3];       /* �����ն����� */
   double m_Teller;        /* ���׹�Ա�� */
   char m_TermMark[9];     /* �����ն˺� */
   char m_TermName[61];    /* �����ն������������� */
   char m_ChkTeller[13];   /* ���˹�Ա�� */
   char m_AutTeller[13];   /* ��Ȩ��Ա�� */
} SysCtl;

typedef struct _CentCustCtl
{
   int m_VisaId;        /* ǩԼid */
   char m_WorkDate[9];  /* �������� */
   char m_ChkNo[4];     /* ������� */
   char m_PeriodNo[9];  /* �ں� */
   char m_TimesNo[4];   /* �κ� */
   char m_Notes[61];    /* ˵�� */
   char m_LastChkNo[4]; /* �ϴ�������� */
   char m_ChkFlag[2];   /* ���ʱ�ʶ */
} CentCustCtl;

typedef struct _CmdResult
{
   int m_CommandId;      /* ����ID */
   int m_SysId;          /* ������ */
   char m_StartDate[9];  /* ����ִ������ */
   char m_StartTime[7];  /* ��ʼʱ�� */
   char m_EndTime[7];    /* ���ʱ�� */
   char m_EndDate[9];    /* ����������� */
   int m_CmdStatus;      /* ����ִ�н�� */
   char m_Notes[301];    /* ��ע */
   char m_CmdInfo[201];  /* ��һ����������Ϣ */
   char m_Operator[41];  /* ����Ա */
   char m_AgTranDate[9]; /* ����ִ������ */
} CmdResult;

typedef struct _FinDetail
{
  int m_AgSerial;           /* �м�ҵ��ϵͳ������ˮ�� */
  char m_AgTranDate[9];     /* �м�ҵ���������� */
  char m_ClassNo[13];       /* ���˷����� */
  int m_ProdCode;           /* ��Ʒ���� */
  char m_AreaCode[8];       /* �������� */
  int m_ChanCode;           /* �������� */
  char m_AgTxCode[10];      /* �м�ҵ������ */
  char m_SysDate[9];        /* �м�ҵ��ϵͳ���� */
  char m_ChanDate[9];       /* �������� */
  char m_TxTime[7];         /* ����ʱ�� */
  char m_ChTxCode[10];      /* ���������� */
  char m_ChSerial[11];      /* ����������ˮ�� */
  char m_HostTranCode[9];   /* ���������� */
  char m_HoSerial[20];      /* ������ˮ�� */
  char m_ExTranCode[21];    /* ����ϵͳ������ */
  char m_ExSerial[39];      /* ����ϵͳ������ˮ�� */
  char m_TxOrg[10];         /* ���׻����� */
  char m_TermMark[9];       /* �����ն����� */
  char m_TxTeller[13];      /* ��Ա�� */
  char m_CheckTeller[13];   /* ����Ա */
  char m_AuthTeller[13];    /* ��Ȩ��Ա */
  char m_Status[2];         /* ״̬ */
  char m_IntoAcctFlag[2];   /* �����������ʶ */
  char m_ExIntoAcctFlag[2]; /* �����������ʶ */
  char m_CashFlag[2];       /* ��ת��ʶ */
  char m_PayTool[3];        /* ���׹��� */
  char m_BatFlag[2];        /* ����������ʶ */
  char m_AutoTxFlag[2];     /* �Զ����ױ�ʶ */
  char m_MoneyBillType[2];  /* ǩԼ������� */
  int m_PrtNum;             /* ��ӡ���� */
  char m_AcctChkFlag[3];    /* ���ʱ�ʶ */
  int m_ApId;               /* ������id */
  char m_PeriodNo[9];       /* �ں� */
  char m_TimeNo[4];         /* �κ� */
  char m_CurrCode[3];       /* ���� */
  double m_TxAmt;           /* ���׽�� */
  double m_OdAmt;           /* ͸֧�� */
  char m_PayerAcct[33];     /* �������˺Ż򿨺� */
  char m_PayerSubacct[33];  /* �����˷��ʺ� */
  char m_PayeeAcct[33];     /* �տ����˺� */
  char m_PayeeSubacct[33];  /* �տ������˺� */
  double m_PayeeBalance;    /* �տ������ */
  double m_PayerBalance;    /* ��������� */
  char m_VouchType[4];      /* ƾ֤���� */
  char m_VouchStartno[11];  /* ƾ֤����� */
  char m_VouchEndno[11];    /* ƾֹ֤���� */
  int m_CentVisaid;         /* �ͻ�ǩԼID�����л��� */
  int m_DisperseVisaid;     /* �ͻ�ǩԼID(ɢ��) */
  int m_CentUserId;         /* ���пͻ�ϵͳID */
  int m_DispUserId;         /* ɢ���ͻ�ϵͳID */
  char m_CostStartdate[9];  /* �Ʒ���ʼ�� */
  char m_CostEnddate[9];    /* �Ʒ���ֹ�� */
  char m_ChgDate[9];        /* �����춯���� */
  char m_ChgTime[7];        /* �����춯ʱ�� */
  int m_ChgAgSerial;        /* �춯�м�ҵ��ϵͳ������ˮ�� */
  char m_ChgChSerial[11];   /* �춯����������ˮ�� */
  char m_ChgHoSerial[20];   /* �춯������ˮ�� */
  char m_ChgExSerial[39];   /* �춯����ϵͳ������ˮ�� */
  char m_RetCode[21];       /* ������ */
  int m_ErrHostNodeId;      /* �������������ڵ�� */
  char m_RetMsg[101];       /* ������Ϣ */
  char m_Notes[51];         /* ��ע */
  char m_DispUserNo[41];    /* ɢ���ڼ��л��û��Ż��ͬ�� */
  double m_LateFee;         /* ���ɽ� */
  char m_RevField1[6];      /* �����ֶ�1 */
  char m_RevField2[11];     /* �����ֶ�2 */
  char m_RevField3[21];     /* �����ֶ�3 */
} FinDetail;

typedef struct _DyZjgj
{
   char m_AgTranDate[9];    /* ƽ̨���� */
   int m_AgSerial;          /* ƽ̨��ˮ */
   char m_TxOrg[10];        /* ������ */
   char m_PayerAcct[33];    /* �������ʺ� */
   char m_PayerVisaOrg[10]; /* �����˿������� */
   char m_PayerName[41];    /* ���������� */
   char m_PayeeAcct[33];    /* �տ����ʺ� */
   char m_PayeeVisaOrg[10]; /* �տ����ʺſ����� */
   char m_PayeeName[41];    /* �տ������� */
   char m_TxAmt[18];        /* ���׽�� */
   char m_CurrType[3];      /* ���� */
   char m_TxTeller[13];     /* ���׹�Ա */
   char m_Status[2];        /* �ɹ���־ */
   int m_CentVisaId;        /* ���л�ǩԼid */
   char m_TxDate[9];        /* �������� */
   char m_Remark[51];       /* ��ע */
} DyZjgj;
EXEC SQL END DECLARE SECTION;

#define LOGDYZJGJ(DyZjgj)\
   LOG4C(( LOG_DEBUG, "ƽ̨����=[%s]\n", DyZjgj.m_AgTranDate));\
   LOG4C(( LOG_DEBUG, "ƽ̨��ˮ=[%d]\n", DyZjgj.m_AgSerial));\
   LOG4C(( LOG_DEBUG, "������=[%s]\n", DyZjgj.m_TxOrg));\
   LOG4C(( LOG_DEBUG, "�������ʺ�=[%s]\n", DyZjgj.m_PayerAcct));\
   LOG4C(( LOG_DEBUG, "�����˿�������=[%s]\n", DyZjgj.m_PayerVisaOrg));\
   LOG4C(( LOG_DEBUG, "����������=[%s]\n", DyZjgj.m_PayerName));\
   LOG4C(( LOG_DEBUG, "�տ����ʺ�=[%s]\n", DyZjgj.m_PayeeAcct));\
   LOG4C(( LOG_DEBUG, "�տ����ʺſ�����=[%s]\n", DyZjgj.m_PayeeVisaOrg));\
   LOG4C(( LOG_DEBUG, "�տ�������=[%s]\n", DyZjgj.m_PayeeName));\
   LOG4C(( LOG_DEBUG, "���׽��=[%s]\n", DyZjgj.m_TxAmt));\
   LOG4C(( LOG_DEBUG, "����=[%s]\n", DyZjgj.m_CurrType));\
   LOG4C(( LOG_DEBUG, "���׹�Ա=[%s]\n", DyZjgj.m_TxTeller));\
   LOG4C(( LOG_DEBUG, "�ɹ���־=[%s]\n", DyZjgj.m_Status));\
   LOG4C(( LOG_DEBUG, "���л�ǩԼid=[%d]\n", DyZjgj.m_CentVisaId));\
   LOG4C(( LOG_DEBUG, "��������=[%s]\n", DyZjgj.m_TxDate));\
   LOG4C(( LOG_DEBUG, "��ע=[%s]\n", DyZjgj.m_Remark));

#define LOGFINDETAIL(FinDetail)\
   LOG4C(( LOG_DEBUG, "�м�ҵ��ϵͳ������ˮ��=[%d]\n", FinDetail.m_AgSerial));\
   LOG4C(( LOG_DEBUG, "�м�ҵ����������=[%s]\n", FinDetail.m_AgTranDate));\
   LOG4C(( LOG_DEBUG, "���˷�����=[%s]\n", FinDetail.m_ClassNo));\
   LOG4C(( LOG_DEBUG, "��Ʒ����=[%d]\n", FinDetail.m_ProdCode));\
   LOG4C(( LOG_DEBUG, "��������=[%s]\n", FinDetail.m_AreaCode));\
   LOG4C(( LOG_DEBUG, "��������=[%d]\n", FinDetail.m_ChanCode));\
   LOG4C(( LOG_DEBUG, "�м�ҵ������=[%s]\n", FinDetail.m_AgTxCode));\
   LOG4C(( LOG_DEBUG, "�м�ҵ��ϵͳ����=[%s]\n", FinDetail.m_SysDate));\
   LOG4C(( LOG_DEBUG, "��������=[%s]\n", FinDetail.m_ChanDate));\
   LOG4C(( LOG_DEBUG, "����ʱ��=[%s]\n", FinDetail.m_TxTime));\
   LOG4C(( LOG_DEBUG, "����������=[%s]\n", FinDetail.m_ChTxCode));\
   LOG4C(( LOG_DEBUG, "����������ˮ��=[%s]\n", FinDetail.m_ChSerial));\
   LOG4C(( LOG_DEBUG, "����������=[%s]\n", FinDetail.m_HostTranCode));\
   LOG4C(( LOG_DEBUG, "������ˮ��=[%s]\n", FinDetail.m_HoSerial));\
   LOG4C(( LOG_DEBUG, "����ϵͳ������=[%s]\n", FinDetail.m_ExTranCode));\
   LOG4C(( LOG_DEBUG, "����ϵͳ������ˮ��=[%s]\n", FinDetail.m_ExSerial));\
   LOG4C(( LOG_DEBUG, "���׻�����=[%s]\n", FinDetail.m_TxOrg));\
   LOG4C(( LOG_DEBUG, "�����ն�����=[%s]\n", FinDetail.m_TermMark));\
   LOG4C(( LOG_DEBUG, "��Ա��=[%s]\n", FinDetail.m_TxTeller));\
   LOG4C(( LOG_DEBUG, "����Ա=[%s]\n", FinDetail.m_CheckTeller));\
   LOG4C(( LOG_DEBUG, "��Ȩ��Ա=[%s]\n", FinDetail.m_AuthTeller));\
   LOG4C(( LOG_DEBUG, "״̬=[%s]\n", FinDetail.m_Status));\
   LOG4C(( LOG_DEBUG, "�����������ʶ=[%s]\n", FinDetail.m_IntoAcctFlag));\
   LOG4C(( LOG_DEBUG, "�����������ʶ=[%s]\n", FinDetail.m_ExIntoAcctFlag));\
   LOG4C(( LOG_DEBUG, "��ת��ʶ=[%s]\n", FinDetail.m_CashFlag));\
   LOG4C(( LOG_DEBUG, "���׹���=[%s]\n", FinDetail.m_PayTool));\
   LOG4C(( LOG_DEBUG, "����������ʶ=[%s]\n", FinDetail.m_BatFlag));\
   LOG4C(( LOG_DEBUG, "�Զ����ױ�ʶ=[%s]\n", FinDetail.m_AutoTxFlag));\
   LOG4C(( LOG_DEBUG, "ǩԼ�������=[%s]\n", FinDetail.m_MoneyBillType));\
   LOG4C(( LOG_DEBUG, "��ӡ����=[%d]\n", FinDetail.m_PrtNum));\
   LOG4C(( LOG_DEBUG, "���ʱ�ʶ=[%s]\n", FinDetail.m_AcctChkFlag));\
   LOG4C(( LOG_DEBUG, "������id=[%d]\n", FinDetail.m_ApId));\
   LOG4C(( LOG_DEBUG, "�ں�=[%s]\n", FinDetail.m_PeriodNo));\
   LOG4C(( LOG_DEBUG, "�κ�=[%s]\n", FinDetail.m_TimeNo));\
   LOG4C(( LOG_DEBUG, "����=[%s]\n", FinDetail.m_CurrCode));\
   LOG4C(( LOG_DEBUG, "���׽��=[%f]\n", FinDetail.m_TxAmt));\
   LOG4C(( LOG_DEBUG, "͸֧��=[%f]\n", FinDetail.m_OdAmt));\
   LOG4C(( LOG_DEBUG, "�������˺Ż򿨺�=[%s]\n", FinDetail.m_PayerAcct));\
   LOG4C(( LOG_DEBUG, "�����˷��ʺ�=[%s]\n", FinDetail.m_PayerSubacct));\
   LOG4C(( LOG_DEBUG, "�տ����˺�=[%s]\n", FinDetail.m_PayeeAcct));\
   LOG4C(( LOG_DEBUG, "�տ������˺�=[%s]\n", FinDetail.m_PayeeSubacct));\
   LOG4C(( LOG_DEBUG, "�տ������=[%f]\n", FinDetail.m_PayeeBalance));\
   LOG4C(( LOG_DEBUG, "���������=[%f]\n", FinDetail.m_PayerBalance));\
   LOG4C(( LOG_DEBUG, "ƾ֤����=[%s]\n", FinDetail.m_VouchType));\
   LOG4C(( LOG_DEBUG, "ƾ֤�����=[%s]\n", FinDetail.m_VouchStartno));\
   LOG4C(( LOG_DEBUG, "ƾֹ֤����=[%s]\n", FinDetail.m_VouchEndno));\
   LOG4C(( LOG_DEBUG, "�ͻ�ǩԼID�����л���=[%d]\n", FinDetail.m_CentVisaid));\
   LOG4C(( LOG_DEBUG, "�ͻ�ǩԼID(ɢ��)=[%d]\n", FinDetail.m_DisperseVisaid));\
   LOG4C(( LOG_DEBUG, "���пͻ�ϵͳID=[%d]\n", FinDetail.m_CentUserId));\
   LOG4C(( LOG_DEBUG, "ɢ���ͻ�ϵͳID=[%d]\n", FinDetail.m_DispUserId));\
   LOG4C(( LOG_DEBUG, "�Ʒ���ʼ��=[%s]\n", FinDetail.m_CostStartdate));\
   LOG4C(( LOG_DEBUG, "�Ʒ���ֹ��=[%s]\n", FinDetail.m_CostEnddate));\
   LOG4C(( LOG_DEBUG, "�����춯����=[%s]\n", FinDetail.m_ChgDate));\
   LOG4C(( LOG_DEBUG, "�����춯ʱ��=[%s]\n", FinDetail.m_ChgTime));\
   LOG4C(( LOG_DEBUG, "�춯�м�ҵ��ϵͳ������ˮ��=[%d]\n", FinDetail.m_ChgAgSerial));\
   LOG4C(( LOG_DEBUG, "�춯����������ˮ��=[%s]\n", FinDetail.m_ChgChSerial));\
   LOG4C(( LOG_DEBUG, "�춯������ˮ��=[%s]\n", FinDetail.m_ChgHoSerial));\
   LOG4C(( LOG_DEBUG, "�춯����ϵͳ������ˮ��=[%s]\n", FinDetail.m_ChgExSerial));\
   LOG4C(( LOG_DEBUG, "������=[%s]\n", FinDetail.m_RetCode));\
   LOG4C(( LOG_DEBUG, "�������������ڵ��=[%d]\n", FinDetail.m_ErrHostNodeId));\
   LOG4C(( LOG_DEBUG, "������Ϣ=[%s]\n", FinDetail.m_RetMsg));\
   LOG4C(( LOG_DEBUG, "��ע=[%s]\n", FinDetail.m_Notes));\
   LOG4C(( LOG_DEBUG, "ɢ���ڼ��л��û��Ż��ͬ��=[%s]\n", FinDetail.m_DispUserNo));\
   LOG4C(( LOG_DEBUG, "���ɽ�=[%f]\n", FinDetail.m_LateFee));\
   LOG4C(( LOG_DEBUG, "�����ֶ�1=[%s]\n", FinDetail.m_RevField1));\
   LOG4C(( LOG_DEBUG, "�����ֶ�2=[%s]\n", FinDetail.m_RevField2));\
   LOG4C(( LOG_DEBUG, "�����ֶ�3=[%s]\n", FinDetail.m_RevField3));
      
#endif
