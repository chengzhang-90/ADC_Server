
#ifndef __ADC_STRUCT_H__
#define __ADC_STRUCT_H__

EXEC SQL BEGIN DECLARE SECTION;

typedef struct _SysCtl
{
   char m_BankCode[4];     /* 分行代码 */
   int m_NodeId;           /* 中间业务节点号 */
   char m_WorkDate[9];     /* 工作日期 */
   char m_LastWorkDate[9]; /* 上次工作日期 */
   int m_CurSerial;        /* 中间业务平台流水号 */
   char m_RunningFlag[2];  /* 中间业务系统状态 */
   char m_State[2];        /* 状态 */
   char m_TxUnit[10];      /* 交易机构号 */
   char m_TxKind[3];       /* 交易终端种类 */
   double m_Teller;        /* 交易柜员号 */
   char m_TermMark[9];     /* 交易终端号 */
   char m_TermName[61];    /* 交易终端类型中文名称 */
   char m_ChkTeller[13];   /* 复核柜员号 */
   char m_AutTeller[13];   /* 授权柜员号 */
} SysCtl;

typedef struct _CentCustCtl
{
   int m_VisaId;        /* 签约id */
   char m_WorkDate[9];  /* 工作日期 */
   char m_ChkNo[4];     /* 账务序号 */
   char m_PeriodNo[9];  /* 期号 */
   char m_TimesNo[4];   /* 次号 */
   char m_Notes[61];    /* 说明 */
   char m_LastChkNo[4]; /* 上次帐务序号 */
   char m_ChkFlag[2];   /* 对帐标识 */
} CentCustCtl;

typedef struct _CmdResult
{
   int m_CommandId;      /* 命令ID */
   int m_SysId;          /* 命令编号 */
   char m_StartDate[9];  /* 命令执行日期 */
   char m_StartTime[7];  /* 开始时间 */
   char m_EndTime[7];    /* 完成时间 */
   char m_EndDate[9];    /* 命令完成日期 */
   int m_CmdStatus;      /* 命令执行结果 */
   char m_Notes[301];    /* 备注 */
   char m_CmdInfo[201];  /* 下一命令所用信息 */
   char m_Operator[41];  /* 操作员 */
   char m_AgTranDate[9]; /* 命令执行日期 */
} CmdResult;

typedef struct _FinDetail
{
  int m_AgSerial;           /* 中间业务系统交易流水号 */
  char m_AgTranDate[9];     /* 中间业务账务日期 */
  char m_ClassNo[13];       /* 对账分类编号 */
  int m_ProdCode;           /* 产品代号 */
  char m_AreaCode[8];       /* 地区代码 */
  int m_ChanCode;           /* 渠道代号 */
  char m_AgTxCode[10];      /* 中间业务交易码 */
  char m_SysDate[9];        /* 中间业务系统日期 */
  char m_ChanDate[9];       /* 渠道日期 */
  char m_TxTime[7];         /* 交易时间 */
  char m_ChTxCode[10];      /* 渠道交易码 */
  char m_ChSerial[11];      /* 渠道交易流水号 */
  char m_HostTranCode[9];   /* 主机交易码 */
  char m_HoSerial[20];      /* 主机流水号 */
  char m_ExTranCode[21];    /* 外连系统交易码 */
  char m_ExSerial[39];      /* 外连系统交易流水号 */
  char m_TxOrg[10];         /* 交易机构号 */
  char m_TermMark[9];       /* 交易终端类型 */
  char m_TxTeller[13];      /* 柜员号 */
  char m_CheckTeller[13];   /* 复核员 */
  char m_AuthTeller[13];    /* 授权柜员 */
  char m_Status[2];         /* 状态 */
  char m_IntoAcctFlag[2];   /* 银行帐务处理标识 */
  char m_ExIntoAcctFlag[2]; /* 外联帐务处理标识 */
  char m_CashFlag[2];       /* 现转标识 */
  char m_PayTool[3];        /* 交易工具 */
  char m_BatFlag[2];        /* 单笔批量标识 */
  char m_AutoTxFlag[2];     /* 自动交易标识 */
  char m_MoneyBillType[2];  /* 签约钞汇类别 */
  int m_PrtNum;             /* 打印次数 */
  char m_AcctChkFlag[3];    /* 对帐标识 */
  int m_ApId;               /* 代销账id */
  char m_PeriodNo[9];       /* 期号 */
  char m_TimeNo[4];         /* 次号 */
  char m_CurrCode[3];       /* 币种 */
  double m_TxAmt;           /* 交易金额 */
  double m_OdAmt;           /* 透支额 */
  char m_PayerAcct[33];     /* 付款人账号或卡号 */
  char m_PayerSubacct[33];  /* 付款人分帐号 */
  char m_PayeeAcct[33];     /* 收款人账号 */
  char m_PayeeSubacct[33];  /* 收款人子账号 */
  double m_PayeeBalance;    /* 收款人余额 */
  double m_PayerBalance;    /* 付款人余额 */
  char m_VouchType[4];      /* 凭证种类 */
  char m_VouchStartno[11];  /* 凭证起号码 */
  char m_VouchEndno[11];    /* 凭证止号码 */
  int m_CentVisaid;         /* 客户签约ID（集中户） */
  int m_DisperseVisaid;     /* 客户签约ID(散户) */
  int m_CentUserId;         /* 集中客户系统ID */
  int m_DispUserId;         /* 散户客户系统ID */
  char m_CostStartdate[9];  /* 计费起始日 */
  char m_CostEnddate[9];    /* 计费终止日 */
  char m_ChgDate[9];        /* 帐务异动日期 */
  char m_ChgTime[7];        /* 帐务异动时间 */
  int m_ChgAgSerial;        /* 异动中间业务系统交易流水号 */
  char m_ChgChSerial[11];   /* 异动渠道交易流水号 */
  char m_ChgHoSerial[20];   /* 异动主机流水号 */
  char m_ChgExSerial[39];   /* 异动外连系统交易流水号 */
  char m_RetCode[21];       /* 错误码 */
  int m_ErrHostNodeId;      /* 发生错误主机节点号 */
  char m_RetMsg[101];       /* 错误信息 */
  char m_Notes[51];         /* 备注 */
  char m_DispUserNo[41];    /* 散户在集中户用户号或合同号 */
  double m_LateFee;         /* 滞纳金 */
  char m_RevField1[6];      /* 保留字段1 */
  char m_RevField2[11];     /* 保留字段2 */
  char m_RevField3[21];     /* 保留字段3 */
} FinDetail;

typedef struct _DyZjgj
{
   char m_AgTranDate[9];    /* 平台日期 */
   int m_AgSerial;          /* 平台流水 */
   char m_TxOrg[10];        /* 机构号 */
   char m_PayerAcct[33];    /* 付款人帐号 */
   char m_PayerVisaOrg[10]; /* 付款人开户机构 */
   char m_PayerName[41];    /* 付款人名称 */
   char m_PayeeAcct[33];    /* 收款人帐号 */
   char m_PayeeVisaOrg[10]; /* 收款人帐号开户行 */
   char m_PayeeName[41];    /* 收款人名称 */
   char m_TxAmt[18];        /* 交易金额 */
   char m_CurrType[3];      /* 币种 */
   char m_TxTeller[13];     /* 交易柜员 */
   char m_Status[2];        /* 成功标志 */
   int m_CentVisaId;        /* 集中户签约id */
   char m_TxDate[9];        /* 交易日期 */
   char m_Remark[51];       /* 备注 */
} DyZjgj;
EXEC SQL END DECLARE SECTION;

#define LOGDYZJGJ(DyZjgj)\
   LOG4C(( LOG_DEBUG, "平台日期=[%s]\n", DyZjgj.m_AgTranDate));\
   LOG4C(( LOG_DEBUG, "平台流水=[%d]\n", DyZjgj.m_AgSerial));\
   LOG4C(( LOG_DEBUG, "机构号=[%s]\n", DyZjgj.m_TxOrg));\
   LOG4C(( LOG_DEBUG, "付款人帐号=[%s]\n", DyZjgj.m_PayerAcct));\
   LOG4C(( LOG_DEBUG, "付款人开户机构=[%s]\n", DyZjgj.m_PayerVisaOrg));\
   LOG4C(( LOG_DEBUG, "付款人名称=[%s]\n", DyZjgj.m_PayerName));\
   LOG4C(( LOG_DEBUG, "收款人帐号=[%s]\n", DyZjgj.m_PayeeAcct));\
   LOG4C(( LOG_DEBUG, "收款人帐号开户行=[%s]\n", DyZjgj.m_PayeeVisaOrg));\
   LOG4C(( LOG_DEBUG, "收款人名称=[%s]\n", DyZjgj.m_PayeeName));\
   LOG4C(( LOG_DEBUG, "交易金额=[%s]\n", DyZjgj.m_TxAmt));\
   LOG4C(( LOG_DEBUG, "币种=[%s]\n", DyZjgj.m_CurrType));\
   LOG4C(( LOG_DEBUG, "交易柜员=[%s]\n", DyZjgj.m_TxTeller));\
   LOG4C(( LOG_DEBUG, "成功标志=[%s]\n", DyZjgj.m_Status));\
   LOG4C(( LOG_DEBUG, "集中户签约id=[%d]\n", DyZjgj.m_CentVisaId));\
   LOG4C(( LOG_DEBUG, "交易日期=[%s]\n", DyZjgj.m_TxDate));\
   LOG4C(( LOG_DEBUG, "备注=[%s]\n", DyZjgj.m_Remark));

#define LOGFINDETAIL(FinDetail)\
   LOG4C(( LOG_DEBUG, "中间业务系统交易流水号=[%d]\n", FinDetail.m_AgSerial));\
   LOG4C(( LOG_DEBUG, "中间业务账务日期=[%s]\n", FinDetail.m_AgTranDate));\
   LOG4C(( LOG_DEBUG, "对账分类编号=[%s]\n", FinDetail.m_ClassNo));\
   LOG4C(( LOG_DEBUG, "产品代号=[%d]\n", FinDetail.m_ProdCode));\
   LOG4C(( LOG_DEBUG, "地区代码=[%s]\n", FinDetail.m_AreaCode));\
   LOG4C(( LOG_DEBUG, "渠道代号=[%d]\n", FinDetail.m_ChanCode));\
   LOG4C(( LOG_DEBUG, "中间业务交易码=[%s]\n", FinDetail.m_AgTxCode));\
   LOG4C(( LOG_DEBUG, "中间业务系统日期=[%s]\n", FinDetail.m_SysDate));\
   LOG4C(( LOG_DEBUG, "渠道日期=[%s]\n", FinDetail.m_ChanDate));\
   LOG4C(( LOG_DEBUG, "交易时间=[%s]\n", FinDetail.m_TxTime));\
   LOG4C(( LOG_DEBUG, "渠道交易码=[%s]\n", FinDetail.m_ChTxCode));\
   LOG4C(( LOG_DEBUG, "渠道交易流水号=[%s]\n", FinDetail.m_ChSerial));\
   LOG4C(( LOG_DEBUG, "主机交易码=[%s]\n", FinDetail.m_HostTranCode));\
   LOG4C(( LOG_DEBUG, "主机流水号=[%s]\n", FinDetail.m_HoSerial));\
   LOG4C(( LOG_DEBUG, "外连系统交易码=[%s]\n", FinDetail.m_ExTranCode));\
   LOG4C(( LOG_DEBUG, "外连系统交易流水号=[%s]\n", FinDetail.m_ExSerial));\
   LOG4C(( LOG_DEBUG, "交易机构号=[%s]\n", FinDetail.m_TxOrg));\
   LOG4C(( LOG_DEBUG, "交易终端类型=[%s]\n", FinDetail.m_TermMark));\
   LOG4C(( LOG_DEBUG, "柜员号=[%s]\n", FinDetail.m_TxTeller));\
   LOG4C(( LOG_DEBUG, "复核员=[%s]\n", FinDetail.m_CheckTeller));\
   LOG4C(( LOG_DEBUG, "授权柜员=[%s]\n", FinDetail.m_AuthTeller));\
   LOG4C(( LOG_DEBUG, "状态=[%s]\n", FinDetail.m_Status));\
   LOG4C(( LOG_DEBUG, "银行帐务处理标识=[%s]\n", FinDetail.m_IntoAcctFlag));\
   LOG4C(( LOG_DEBUG, "外联帐务处理标识=[%s]\n", FinDetail.m_ExIntoAcctFlag));\
   LOG4C(( LOG_DEBUG, "现转标识=[%s]\n", FinDetail.m_CashFlag));\
   LOG4C(( LOG_DEBUG, "交易工具=[%s]\n", FinDetail.m_PayTool));\
   LOG4C(( LOG_DEBUG, "单笔批量标识=[%s]\n", FinDetail.m_BatFlag));\
   LOG4C(( LOG_DEBUG, "自动交易标识=[%s]\n", FinDetail.m_AutoTxFlag));\
   LOG4C(( LOG_DEBUG, "签约钞汇类别=[%s]\n", FinDetail.m_MoneyBillType));\
   LOG4C(( LOG_DEBUG, "打印次数=[%d]\n", FinDetail.m_PrtNum));\
   LOG4C(( LOG_DEBUG, "对帐标识=[%s]\n", FinDetail.m_AcctChkFlag));\
   LOG4C(( LOG_DEBUG, "代销账id=[%d]\n", FinDetail.m_ApId));\
   LOG4C(( LOG_DEBUG, "期号=[%s]\n", FinDetail.m_PeriodNo));\
   LOG4C(( LOG_DEBUG, "次号=[%s]\n", FinDetail.m_TimeNo));\
   LOG4C(( LOG_DEBUG, "币种=[%s]\n", FinDetail.m_CurrCode));\
   LOG4C(( LOG_DEBUG, "交易金额=[%f]\n", FinDetail.m_TxAmt));\
   LOG4C(( LOG_DEBUG, "透支额=[%f]\n", FinDetail.m_OdAmt));\
   LOG4C(( LOG_DEBUG, "付款人账号或卡号=[%s]\n", FinDetail.m_PayerAcct));\
   LOG4C(( LOG_DEBUG, "付款人分帐号=[%s]\n", FinDetail.m_PayerSubacct));\
   LOG4C(( LOG_DEBUG, "收款人账号=[%s]\n", FinDetail.m_PayeeAcct));\
   LOG4C(( LOG_DEBUG, "收款人子账号=[%s]\n", FinDetail.m_PayeeSubacct));\
   LOG4C(( LOG_DEBUG, "收款人余额=[%f]\n", FinDetail.m_PayeeBalance));\
   LOG4C(( LOG_DEBUG, "付款人余额=[%f]\n", FinDetail.m_PayerBalance));\
   LOG4C(( LOG_DEBUG, "凭证种类=[%s]\n", FinDetail.m_VouchType));\
   LOG4C(( LOG_DEBUG, "凭证起号码=[%s]\n", FinDetail.m_VouchStartno));\
   LOG4C(( LOG_DEBUG, "凭证止号码=[%s]\n", FinDetail.m_VouchEndno));\
   LOG4C(( LOG_DEBUG, "客户签约ID（集中户）=[%d]\n", FinDetail.m_CentVisaid));\
   LOG4C(( LOG_DEBUG, "客户签约ID(散户)=[%d]\n", FinDetail.m_DisperseVisaid));\
   LOG4C(( LOG_DEBUG, "集中客户系统ID=[%d]\n", FinDetail.m_CentUserId));\
   LOG4C(( LOG_DEBUG, "散户客户系统ID=[%d]\n", FinDetail.m_DispUserId));\
   LOG4C(( LOG_DEBUG, "计费起始日=[%s]\n", FinDetail.m_CostStartdate));\
   LOG4C(( LOG_DEBUG, "计费终止日=[%s]\n", FinDetail.m_CostEnddate));\
   LOG4C(( LOG_DEBUG, "帐务异动日期=[%s]\n", FinDetail.m_ChgDate));\
   LOG4C(( LOG_DEBUG, "帐务异动时间=[%s]\n", FinDetail.m_ChgTime));\
   LOG4C(( LOG_DEBUG, "异动中间业务系统交易流水号=[%d]\n", FinDetail.m_ChgAgSerial));\
   LOG4C(( LOG_DEBUG, "异动渠道交易流水号=[%s]\n", FinDetail.m_ChgChSerial));\
   LOG4C(( LOG_DEBUG, "异动主机流水号=[%s]\n", FinDetail.m_ChgHoSerial));\
   LOG4C(( LOG_DEBUG, "异动外连系统交易流水号=[%s]\n", FinDetail.m_ChgExSerial));\
   LOG4C(( LOG_DEBUG, "错误码=[%s]\n", FinDetail.m_RetCode));\
   LOG4C(( LOG_DEBUG, "发生错误主机节点号=[%d]\n", FinDetail.m_ErrHostNodeId));\
   LOG4C(( LOG_DEBUG, "错误信息=[%s]\n", FinDetail.m_RetMsg));\
   LOG4C(( LOG_DEBUG, "备注=[%s]\n", FinDetail.m_Notes));\
   LOG4C(( LOG_DEBUG, "散户在集中户用户号或合同号=[%s]\n", FinDetail.m_DispUserNo));\
   LOG4C(( LOG_DEBUG, "滞纳金=[%f]\n", FinDetail.m_LateFee));\
   LOG4C(( LOG_DEBUG, "保留字段1=[%s]\n", FinDetail.m_RevField1));\
   LOG4C(( LOG_DEBUG, "保留字段2=[%s]\n", FinDetail.m_RevField2));\
   LOG4C(( LOG_DEBUG, "保留字段3=[%s]\n", FinDetail.m_RevField3));
      
#endif
