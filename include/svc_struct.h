#ifndef _SVC_STRUCT_H_
#define _SVC_STRUCT_H_

EXEC SQL BEGIN DECLARE SECTION;
typedef struct _svc_merchant
{
    int       m_nId;
    char      m_sCode[16];
    char      m_sName[121];
    char      m_sShortName[31];
    char      m_sAcqOrgid[5];
    char      m_sAddress[151];
    char      m_sTel[16];
    char      m_sMobile[16];
    char      m_sEmail[41];
    char      m_sPerson[33];
    char      m_sMerType[31];
    char      m_sPaperType[31];
    char      m_sPaperId[31];
    char      m_sParentCode[16];
    double    m_fTotalLimit;
    double    m_fPayLimit;
    char      m_sAcctType[5];
    char      m_sAcctNo[33];
    char      m_sAcctBank[101];
    double    m_fBrokerRate;
    double    m_fBrokerMax;
    double    m_fBrokerMin;
    char      m_sUpdateTime[15];
    char      m_sStatus[2];
    char      m_sRemark[101];
    char      m_sDac[17];
    char      m_sCheckFlag[2];
    int       m_nCheckUser;
    char      m_sCheckReason[101];
    char      m_sCheckDate[15];
    int       m_nDayTimes;
    char      m_sTxBeginTime[7];
    char      m_sTxEndTime[7];
}T_SVC_Merchant;

typedef struct _svc_card_info
{
    int         m_nId;
    char        m_sCardBin[13];
    char        m_sCardNo[33];
    char        m_sCardUser[31];
    char        m_sPin[33];
    char        m_sInqPin[33];
    char        m_sMemberType[2];
    char        m_sMemberId[21];
    char        m_sActiveDate[9];
    char        m_sValiDate[9];
    char        m_sTryDate[9];
    int         m_nTryTimes;
    char        m_sCvn2[11];
    char        m_sTk1[121];
    char        m_sTk2[41];
    char        m_sTk3[121];
    char        m_sStatus[2];
    double      m_fInitAmt;
    int         m_nInitScore;
    double      m_fYearFee;
    double      m_fForegift;
    char        m_sUpdateTime[15];
    char        m_sDac[17];
}T_SVC_CardInfo;
typedef struct _svc_card_info_ind
{
    short       m_nId;
    short       m_nCardBin;
    short       m_nCardNo;
    short       m_nCardUser;
    short       m_nPin;
    short       m_nInqPin;
    short       m_nMemberType;
    short       m_nMemberId;
    short       m_nActiveDate;
    short       m_nValiDate;
    short       m_nTryDate;
    short       m_nTryTimes;
    short       m_nCvv2;
    short       m_sTk1;
    short       m_nTk2;
    short       m_nTk3;
    short       m_nStatus;
    short       m_nInitAmt;
    short       m_nInitScore;
    short       m_nYearFee;
    short       m_nForegift;
    short       m_nUpdateTime;
    short       m_nDac;
}T_SVC_CardInfoInd;


typedef struct _svc_card_account
{
    int         m_nId;
    char        m_sAccountId[21];
    char        m_sCardNo[33];
    char        m_sFlag[2];
    char        m_sDac[17];
}T_SVC_CardAccount;

typedef struct _svc_card_account_ind
{
    short       m_nId;
    short       m_nAccountId;
    short       m_nCardNo;
    short       m_nFlag;
    short       m_nDac;
}T_SVC_CardAccountInd;

typedef struct _svc_account_info
{
    int         m_nId;
    char        m_sAccountId[21];
    char        m_sCardBin[13];
    double      m_fTotalValue;
    double      m_fUseValue;
    char        m_sStatus[2];
    char        m_sUpdateTime[15];
    char        m_sDac[17];
}T_SVC_AccountInfo;
typedef struct _svc_account_info_ind
{
    short       m_nId;
    short       m_nAccountId;
    short       m_nCardBin;
    short       m_nTotalValue;
    short       m_nValue;
    short       m_nStatus;
    short       m_nUpdateTime;
    short       m_nDac;
}T_SVC_AccountInfoInd;

typedef struct _svc_detail
{
    int         m_nPtSerial;
    char        m_sPtDate[9];
    char        m_sBatchNo[9];
    char        m_sInvoice[9];
    char        m_sMerchantId[16];
    char        m_sTerminalId[9];
    int         m_nChCode;
    char        m_sChMsgid[5];
    char        m_sChTxCode[13];
    char        m_sChDate[9];
    char        m_sChTime[7];
    char        m_sChSerial[9];
    char        m_sChBatchNo[9];
    char        m_sChRetCode[13];
    char        m_sChRetMsg[101];
    char        m_sHoRetCode[13];
    char        m_sHoRetMsg[101];
    char        m_sHoSerial[21];
    char        m_sSysDate[9];
    char        m_sSysTime[7];
    char        m_sAuthNo[13];
    char        m_sRefNum[21];
    char        m_sCardNo[33];
    char        m_sTxTeller[16];
    double      m_fTxAmt;
    double      m_fChkAmt;
    char        m_sIssOrgid[21];
    char        m_sAcqOrgid[21];
    char        m_sSettleId[21];
    char        m_sStatus[2];
    char        m_sSettleDate[9];
    char        m_sSettleTime[7];
    char        m_sAcctChkFlag[2];
    char        m_sOldStatus[2];
    int         m_nOldPtSerial;
    char        m_sOldPtDate[9];
    char        m_sDac[17];
}T_SVC_Detail;
typedef struct _svc_detail_ind
{
    short       m_nPtSerial;
    short       m_nPtDate;
    short       m_nBatchNo;
    short       m_nInvoice;
    short       m_nMerchantId;
    short       m_nTerminalId;
    short       m_nChCode;
    short       m_nChMsgid;
    short       m_nChTxCode;
    short       m_nChDate;
    short       m_nChTime;
    short       m_nChSerial;
    short       m_nChBatchNo;
    short       m_nChRetCode;
    short       m_nChRetMsg;
    short       m_nHoRetCode;
    short       m_nHoRetMsg;
    short       m_nHoSerial;
    short       m_nSysDate;
    short       m_nSysTime;
    short       m_nAuthNo;
    short       m_nRefNum;
    short       m_nCardNo;
    short       m_nTxTeller;
    short       m_nTxAmt;
    short       m_nChkAmt;
    short       m_nIssOrgid;
    short       m_nAcqOrgid;
    short       m_nSettleId;
    short       m_nStatus;
    short       m_nSettleDate;
    short       m_nSettleTime;
    short       m_nAcctChkFlag;
    short       m_nOldStatus;
    short       m_nOldPtSerial;
    short       m_nOldPtDate;
    short       m_nDac;
}T_SVC_DetailInd;

typedef struct _svc_accdetail
{
    int         m_nPtSerial;
    char        m_sPtDate[9];
    char        m_sHoTxCode[13];
    char        m_sMerchantId[16];
    char        m_sTerminalId[9];
    char        m_sAuthNo[13];
    char        m_sRefNum[21];
    char        m_sCardNo[33];
    char        m_sTxTeller[21];
    double      m_fTxAmt;
    int         m_nTxScore;
    double      m_fBalAmt;
    int         m_nBalScore;
    char        m_sAcqOrgid[21];
    char        m_sIssOrgid[21];
    char        m_sServOrgid[21];
    char        m_sSysDate[9];
    char        m_sSysTime[7];
    char        m_sSettleDate[9];
    char        m_sSettleTime[7];
    double      m_fShopAmt;
    int         m_nShopScore;
    double      m_fIssFee;
    double      m_fAcqFee;
    double      m_fServiceFee;
    char        m_sStatus[2];
    int         m_nNodeId;
    int         m_nOldPtSerial;
    char        m_sOldPtDate[9];
    char        m_sDac[17];
}T_SVC_AccDetail;
typedef struct _svc_accdetail_ind
{
    short       m_nPtSerial;
    short       m_nPtDate;
    short       m_nHoTxCode;
    short       m_nMerchantId;
    short       m_nTerminalId;
    short       m_nAuthNo;
    short       m_nRefNum;
    short       m_nCardNo;
    short       m_nTxTeller;
    short       m_nTxAmt;
    short       m_nTxScore;
    short       m_nBalAmt;
    short       m_nBalScore;
    short       m_nAcqOrgid;
    short       m_nIssOrgid;
    short       m_nServOrgid;
    short       m_nSysDate;
    short       m_nSysTime;
    short       m_nSettleDate;
    short       m_nSettleTime;
    short       m_nShopAmt;
    short       m_nShopScore;
    short       m_nIssFee;
    short       m_nAcqFee;
    short       m_nServiceFee;
    short       m_nStatus;
    short       m_nNodeId;
    short       m_nOldPtSerial;
    short       m_nOldPtDate;
    short       m_nDac;
}T_SVC_AccDetailInd;

typedef struct _svc_integral
{
    int         m_nId;
    char        m_sAccountId[21];
    char        m_sCardBin[13];
    int         m_nScore;
    char        m_sValidDate[9];
    char        m_sStatus[2];
    char        m_sRemark[61];
    char        m_sUpdateTime[15];
    char        m_sDac[17];
}T_SVC_Integral;
typedef struct _svc_integral_ind
{
    short        m_nId;
    short        m_nAccountId;
    short        m_nCardBin;
    short        m_nScore;
    short        m_nValidDate;
    short        m_nStatus;
    short        m_nRemark;
    short        m_nUpdateTime;
    short        m_nDac;
}T_SVC_IntegralInd;


typedef struct _svc_card_kind
{
    int         m_nId;
    char        m_sCardBin[13];
    char        m_sIssCode[5];
    char        m_sCardHead[5];
    char        m_sName[101];
    char        m_sMedia[2];
    char        m_sIsRealName[2];
    double      m_fYearFee;
    char        m_sIsFixValue;
    double      m_fPreValue;
    double      m_fForgift;
    double      m_fCost;
    double      m_fTopValue;
    double      m_fLeastValue;
    char        m_sScoreRate[21];
    double      m_fOnceLimit;
    double      m_fDayLimit;
    int         m_nTryTimes;
    int         m_nInitScore;
    char        m_sRemark[121];
    char        m_sStatus[2];
    char        m_sDac[17];
    int         m_nDayTimes;
}T_SVC_CardKind;
typedef struct _svc_card_kind_ind
{
    short       m_nId;
    short       m_nCardBin;
    short       m_nIssCode;
    short       m_nCardHead;
    short       m_nName;
    short       m_nMedia;
    short       m_nIsRealName;
    short       m_nYearFee;
    short       m_nIsFixValue;
    short       m_nPreValue;
    short       m_nForgift;
    short       m_nCost;
    short       m_nTopValue;
    short       m_nLeastValue;
    short       m_nScoreRate;
    short       m_nOnceLimit;
    short       m_nDayLimit;
    short       m_nTryTimes;
    short       m_nInitScore;
    short       m_nRemark;
    short       m_nStatus;
    short       m_nDac;
    short       m_nDayTimes;
}T_SVC_CardKindInd;

typedef struct _svc_card_range
{
    int         m_nId;
    char        m_sCardBin[13];
    char        m_sAcqOrgid[21];
    double      m_fAcqRate;
    double      m_fIssRate;
    double      m_fServRate;
    char        m_sChkFlag[2];
    int         m_nChkUser;
    char        m_sChkDate[15];
    char        m_sStatus[2];
    char        m_sUpdateTime[15];
    char        m_sDac[17];
}T_SVC_CardRange;
typedef struct _svc_card_range_ind
{
    short      m_nId;
    short      m_nCardBin;
    short      m_nAcqOrgid;
    short      m_nAcqRate;
    short      m_nIssRate;
    short      m_nServRate;
    short      m_nCheckFlag;
    short      m_nChkUser;
    short      m_nChkDate;
    short      m_nStatus;
    short      m_nUpdateTime;
    short      m_nDac;
}T_SVC_CardRangeInd;

typedef struct _svc_daily_acc
{
    char      m_sCardBin[13];
    double    m_fBalValue;
    int       m_nBalScore;
    int       m_nCardNum;
    double    m_fAddValue;
    double    m_fAdjustValue;
    double    m_fDecValue;
    int       m_nAddScore;
    int       m_nAdjustScore;
    int       m_nDecScore;
    int       m_nActiveToday;
    double    m_fActiveValue;
    int       m_nActiveScore;
    int       m_nDepositToday;
    double    m_fDepositValue;
    double    m_fForegift;
    char      m_sSettleResult[2];
    char      m_sSettleDate[9];
    char      m_sUpdateTime[15];
    char      m_sDac[17];
}T_SVC_DailyAcc;
typedef struct _svc_daily_acc_ind
{
    short     m_nCardBin;
    short     m_nBalValue;
    short     m_nBalScore;
    short     m_nCardNum;
    short     m_nAddValue;
    short     m_nAdjustValue;
    short     m_nDecValue;
    short     m_nAddScore;
    short     m_nAdjustScore;
    short     m_nDecScore;
    short     m_nActiveToday;
    short     m_nActiveValue;
    short     m_nActiveScore;
    short     m_nDepositToday;
    short     m_nDepositValue;
    short     m_nForegift;
    short     m_nSettleResult;
    short     m_nSettleDate;
    short     m_nUpdateTime;
    short     m_nDac;
}T_SVC_DailyAccInd;

typedef struct _svc_gov_acct
{
    char      m_sIssCode[21];
    char      m_sAcctBank[101];
    char      m_sAcctNo[33];
    double    m_fAcctAmt;
    double    m_fIssBal;
    double    m_fGovRate;
    double    m_fLeastAmt;
    char      m_sStatus[2];
    char      m_sSettleDate[9];
    char      m_sUpdateTime[15];
    char      m_sDac[17];
}T_SVC_GovAcct;
typedef struct _svc_gov_acct_ind
{
    short    m_nIssCode;
    short    m_nAcctBank;
    short    m_nAcctNo;
    short    m_nAcctAmt;
    short    m_nIssBal;
    short    m_nGovRate;
    short    m_nLeastAmt;
    short    m_nStatus;
    short    m_nSettleDate;
    short    m_nUpdateTime;
    short    m_nDac;
}T_SVC_GovAcctInd;

typedef struct _svc_iss_organ
{
    int      m_nId;
    char     m_sIssCode[5];
    char     m_sName[101];
    char     m_sShortName[21];
    char     m_sServCode[3];
    char     m_sProvince[31];
    char     m_sCity[31];
    char     m_sCountry[31];
    char     m_sAddress[121];
    char     m_sPostCode[11];
    char     m_sTel[21];
    char     m_sFax[21];
    char     m_sLinkMan[21];
    char     m_sAcctType[5];
    char     m_sAcctNo[33];
    char     m_sAcctBank[101];
    char     m_sParentCode[5];
    char     m_sGovAcctNo[33];
    char     m_sGovAcctType[5];
    char     m_sGovAcctBank[101];
    double   m_fStoreRate;
    char     m_sRemark[101];
    char     m_sStatus[2];
}T_SVC_IssOrgan;
typedef struct _svc_iss_organ_ind
{
    short   m_nId;
    short   m_nIssCode;
    short   m_nName;
    short   m_nShortName;
    short   m_nServCode;
    short   m_nProvince;
    short   m_nCity;
    short   m_nCountry;
    short   m_nAddress;
    short   m_nPostCode;
    short   m_nTel;
    short   m_nFax;
    short   m_nLinkMan;
    short   m_nAcctType;
    short   m_nAcctNo;
    short   m_nAcctBank;
    short   m_nParentCode;
    short   m_nGovAcctNo;
    short   m_nGovAcctType;
    short   m_nGovAcctBank;
    short   m_nStoreRate;
    short   m_nRemark;
    short   m_nStatus;
}T_SVC_IssOrganInd;

typedef struct _svc_merchant_acc_detail
{
    char     m_sHoTxCode[13];
    char     m_sMerchantId[16];
    int      m_nTxNum;
    double   m_fTxAmt;
    int      m_nTxScore;
    double   m_fAddAmt;
    int      m_nAddScore;
    char     m_sSettleDate[9];
    double   m_fShopValue;
    int      m_nShopScore;
    double   m_fIssFee;
    double   m_fAcqFee;
    double   m_fServiceFee;
    char     m_sUpdateTime[15];
    char     m_sDac[17];
}T_SVC_MerchantSettle;
typedef struct _svc_merchant_acc_detail_ind
{
    short    m_nHoTxCode;
    short    m_nMerchantId;
    short    m_nTxNum;
    short    m_nTxAmt;
    short    m_nTxScore;
    short    m_nAddAmt;
    short    m_nAddScore;
    short    m_nSettleDate;
    short    m_nShopValue;
    short    m_nShopScore;
    short    m_nIssFee;
    short    m_nAcqFee;
    short    m_nServiceFee;
    short    m_nUpdateTime;
    short    m_nDac;
}T_SVC_MerchantSettleInd;

typedef struct _svc_service_fee
{
    char     m_sIssOrgid[21];
    char     m_sOutBankName[101];
    char     m_sOutAcctNo[33];
    double   m_fSettleAmt;
    char     m_sServOrgid[21];
    char     m_sInBankName[101];
    char     m_sInAcctNo[33];
    char     m_sSettleDate[9];
    char     m_sOper[31];
    char     m_sPayDate[9];
    char     m_sInvoice[31];
    char     m_sRemark[101];
    char     m_sStatus[2];
    char     m_sUpdateTime[15];
    char     m_sDac[17];
}T_SVC_ServiceFee;
typedef struct _svc_service_fee_ind
{
    short   m_nIssOrgid;
    short   m_nOutBankName;
    short   m_nOutAcctNo;
    short   m_nSettleAmt;
    short   m_nServOrgid;
    short   m_nInBankName;
    short   m_nInAcctNo;
    short   m_nSettleDate;
    short   m_nOper;
    short   m_nPayDate;
    short   m_nInvoice;
    short   m_nRemark;
    short   m_nStatus;
    short   m_nUpdateTime;
    short   m_nDac;
}T_SVC_ServiceFeeInd;

typedef struct _svc_iss_fee
{
    char     m_sIssOrgid[21];
    char     m_sOutBankName[101];
    char     m_sOutAcctNo[33];
    double   m_fSettleAmt;
    char     m_sInBankName[101];
    char     m_sInAcctNo[33];
    char     m_sSettleDate[9];
    char     m_sOper[31];
    char     m_sPayDate[9];
    char     m_sInvoice[31];
    char     m_sRemark[101];
    char     m_sStatus[2];
    char     m_sUpdateTime[15];
    char     m_sDac[17];
}T_SVC_IssFee;
typedef struct _svc_iss_fee_ind
{
    short   m_nIssOrgid;
    short   m_nOutBankName;
    short   m_nOutAcctNo;
    short   m_nSettleAmt;
    short   m_nInBankName;
    short   m_nInAcctNo;
    short   m_nSettleDate;
    short   m_nOper;
    short   m_nPayDate;
    short   m_nInvoice;
    short   m_nRemark;
    short   m_nStatus;
    short   m_nUpdateTime;
    short   m_nDac;
}T_SVC_IssFeeInd;

typedef struct _svc_acq_fee
{
    char     m_sIssOrgid[21];
    char     m_sOutBankName[101];
    char     m_sOutAcctNo[33];
    double   m_fSettleAmt;
    char     m_sAcqOrgid[21];
    char     m_sInBankName[101];
    char     m_sInAcctNo[33];
    char     m_sSettleDate[9];
    char     m_sOper[31];
    char     m_sPayDate[9];
    char     m_sInvoice[31];
    char     m_sRemark[101];
    char     m_sStatus[2];
    char     m_sUpdateTime[15];
    char     m_sDac[17];
}T_SVC_AcqFee;
typedef struct _svc_acq_fee_ind
{
    short   m_nIssOrgid;
    short   m_nOutBankName;
    short   m_nOutAcctNo;
    short   m_nSettleAmt;
    short   m_nAcqOrgid;
    short   m_nInBankName;
    short   m_nInAcctNo;
    short   m_nSettleDate;
    short   m_nOper;
    short   m_nPayDate;
    short   m_nInvoice;
    short   m_nRemark;
    short   m_nStatus;
    short   m_nUpdateTime;
    short   m_nDac;
}T_SVC_AcqFeeInd;

typedef struct _svc_shop_amt
{
    char     m_sIssOrgid[21];
    char     m_sOutBankName[101];
    char     m_sOutAcctNo[33];
    double   m_fSettleAmt;
    char     m_sMerchantId[16];
    char     m_sInBankName[101];
    char     m_sInAcctNo[33];
    char     m_sSettleDate[9];
    char     m_sOper[31];
    char     m_sPayDate[9];
    char     m_sInvoice[31];
    char     m_sRemark[101];
    char     m_sStatus[2];
    char     m_sUpdateTime[15];
    char     m_sDac[17];
}T_SVC_ShopAmt;
typedef struct _svc_shop_amt_ind
{
    short   m_nIssOrgid;
    short   m_nOutBankName;
    short   m_nOutAcctNo;
    short   m_nSettleAmt;
    short   m_nMerchantId;
    short   m_nInBankName;
    short   m_nInAcctNo;
    short   m_nSettleDate;
    short   m_nOper;
    short   m_nPayDate;
    short   m_nInvoice;
    short   m_nRemark;
    short   m_nStatus;
    short   m_nUpdateTime;
    short   m_nDac;
}T_SVC_ShopAmtInd;

typedef struct _Svc_ShopGroup_Detail
{
    char    m_sGroupId[21];
    char    m_sMerchantId[16];
    char    m_sInTime[15];
    int     m_nUserId;
    char    m_sCheckFlag[2];
    int     m_nCheckUser; 
    char    m_sCheckReason[101];
    char    m_sCheckDate[15];
    char    m_sDac[17];
}T_SVC_ShopGroupDetail;
typedef struct _Svc_ShopGroup_Detail_Ind
{
    short   m_nGroupId;
    short   m_nMerchantId;
    short   m_nInTime;
    short   m_nUserId;
    short   m_nCheckFlag;
    short   m_nCheckUser; 
    short   m_nCheckReason;
    short   m_nCheckDate;
    short   m_nDac;
}T_SVC_ShopGroupDetailInd;

typedef struct _Svc_ShopGroup
{
    int     m_nId;
    char    m_sGroupId[21];
    char    m_sGroupName[101];
    int     m_nUserId;
    char    m_sUpdateTime[15];
    double  m_fLeastAmt;
    double  m_fWarningAmt;
    double  m_fBalance;
    char    m_sCheckFlag[2];
    int     m_nCheckUser; 
    char    m_sCheckReason[101];
    char    m_sCheckDate[15];
    char    m_sRemark[101];
    char    m_sDac[17];
}T_SVC_ShopGroup;
typedef struct _Svc_ShopGroup_Ind
{
    short  m_nId;
    short  m_nGroupId;
    short  m_nGroupName;
    short  m_nUserId;
    short  m_nUpdateTime;
    short  m_nLeastAmt;
    short  m_nWarnIngAmt;
    short  m_nBalance;
    short  m_nCheckFlag;
    short  m_nCheckUser; 
    short  m_nCheckReason;
    short  m_nCheckDate;
    short  m_nRemark;
    short  m_nDac;
}T_SVC_ShopGroupInd;


typedef struct _Svc_CardSell
{
	int    m_nId;
	char m_sBillNo[31];
	char m_sOrderType[2];
	int m_nSeller;
	char m_sSellDate[15];
	char m_sCreateDate[15];
	char m_sCancelDate[15];
	char m_sCancelReason[301];
	char m_sAgentName[31];
	char m_sAgentCertifyNo[31];
	double m_fRec_Amt;
	double m_fAct_Amt;
	char m_sMemberType[2];
	char m_sBuyer[31];
	char m_sPayWay[2];
	char m_sRemark[301];
	char m_sCheckFlag[2];
	int m_nChkUser;
	char m_sCheckDate[15];
	char m_sCheckReason[301];
	char m_sCheck2Flag[2];
	int m_nChk2User;
	char m_sCheck2Date[15];
	char m_sCheck2Reason[301];
	char m_sStatus[2];
	char m_sDac[17];
}T_Svc_CardSell;

typedef struct _Svc_CardSell_ind
{
	short  m_nId;
	short  m_nBillNo;
	short  m_nOrderType;
	short  m_nSeller;
	short  m_nSellDate;
	short  m_nCreateDate;
	short  m_nCancelDate;
	short  m_nCancelReason;
	short  m_nAgentName;
	short  m_nAgentCertifyNo;
	short  m_nRec_Amt;
	short  m_nAct_Amt;
	short  m_nMemberType;
	short  m_nBuyer;
	short  m_nPayWay;
	short  m_nRemark;
	short  m_nCheckFlag;
	short  m_nChkUser;
	short  m_nCheckDate;
	short  m_nCheckReason;
	short  m_nCheck2Flag;
	short  m_nChk2User;
	short  m_nCheck2Date;
	short  m_nCheck2Reason;
	short  m_nStatus;
	short  m_nDac;
}T_Svc_CardSellInd;


typedef struct _Svc_cardsell_detail
{
    char   m_sId[33];
    int    m_nSellId;
    char   m_sCardNo[33];
    int    m_nUserId;
    double m_fAmt;
    char   m_sResult[2];
    char   m_sRetCode[11];
    char   m_sRetMsg[101];
    char   m_sDac[17];
}T_CardSell_Detail;
typedef struct _Svc_cardsell_detail_ind
{
    short  m_nId;
    short  m_nSellId;
    short  m_nCardNo;
    short  m_nUserId;
    short  m_nAmt;
    short  m_nResult;
    short  m_nRetCode;
    short  m_nRetMsg;
    short  m_nDac;
}T_CardSell_DetailInd;

typedef struct _Svc_Risk
{
    int    m_nPtSerial;
    char   m_sPtDate[9];
    char   m_sHoTxCode[13];
    char   m_sMerchantId[16];
    char   m_sTerminal[9];
    char   m_sCardNo[32];
    double m_fTxAmt;
    char   m_sRiskItem[5];
    char   m_sIsRing[2];
    char   m_sIsEmail[2];
    char   m_sIsMsg[2];
    char   m_sUpdateTime[15];
}T_SvcRisk;
typedef struct _Svc_Risk_Ind
{
    short m_nPtSerial;
    short m_nPtDate;
    short m_nHoTxCode;
    short m_nMerchantId;
    short m_nTerminal;
    short m_nCardNo;
    short m_nTxAmt;
    short m_nRiskItem;
    short m_nIsRisk;
    short m_nIsRisk2;
    short m_nIsRisk3;
    short m_nUpdateTime;
}T_SvcRiskInd;

typedef struct _Svc_Risk_Param
{
    char   m_sRiskItem[5];
    char   m_sCondition1[13];
    char   m_sCondition2[13];
    char   m_sCondition3[13];
    char   m_sTxAction[2];
    char   m_sRiskLevel[2];
    char   m_sLevelName[21];
    char   m_sRiskBeep[2];
    char   m_sBeepName[101];
    char   m_sRiskEmail[2];
    char   m_sEmail[301];
    char   m_sRiskMsg[2];
    char   m_sMobile[301];
    char   m_sStatus[2];
    char   m_sUpdateTime[15];
}T_SvcRiskParam;
typedef struct _Svc_Risk_Param_Ind
{
    short  m_nRiskItem;
    short  m_nCondition1;
    short  m_nCondition2;
    short  m_nCondition3;
    short  m_nTxAction;
    short  m_nRiskLevel;
    short  m_nLevelName;
    short  m_nRiskBeep;
    short  m_nBeepName;
    short  m_nRiskEmail;
    short  m_nEmail;
    short  m_nRiskMsg;
    short  m_nMobile;
    short  m_nStatus;
    short  m_nUpdateTime;
}T_SvcRiskParamInd;

typedef struct _Svc_detail_ext
{
   char       m_sExSerial[21];
   char       m_sRefnum[21];
   char        m_sMerchantId[16];
   char        m_sTerminalId[9];
   char        m_sChDate[9];
   char        m_sChTime[7];
   char        m_sChSerial[7];
   char        m_sAcctNo[33];
   char        m_sTxAmt[13];
}T_SvcDetailExt;

typedef struct _Svc_detail_ext_Ind
{
   short       m_nExSerial;
   short        m_nChSerial;
   short       m_nRefnum;
   short        m_nMerchantId;
   short        m_nTerminalId;
   short        m_nChDate;
   short        m_nChTime;
   short        m_nAcctNo;
   short        m_nTxAmt;
}T_SvcDetailExtInd;



typedef struct _Svc_detail_ext_normal
{
   char       m_sExSerial[21];
   char        m_sChSerial[7];
   char       m_sRefnum[21];
   char        m_sMerchantId[16];
   char        m_sTerminalId[9];
   char        m_sChDate[9];
   char        m_sChTime[7];
   char 	   m_sBatchNo[9];
   char        m_sChCode[3+1];
   char        m_sAcctNo[33];
   char        m_sTxAmt[13];
   char 	   m_sTrack2[41];
   char 	   m_sTrack3[121];
}T_SvcDetailExtNormal;

typedef struct _Svc_detail_ext_normal_Ind
{
	short  m_nExSerial;
	short  m_nRefnum;
	short  m_nMerchantId;
	short  m_nTerminalId;
	short  m_nChDate;
	short  m_nChTime;
	short  m_nChSerial;
	short  m_nBatchNo;
	short  m_nChCode;
	short  m_nAcctNo;
	short  m_nTxAmt;
	short  m_nTrack2;
	short  m_nTrack3;
}T_SvcDetailExtNormalInd;




typedef struct _svc_miteno_terminal
{
    char        m_sTermId[9];
    char        m_sMerchantId[16];
    char        m_sName[121];
    char        m_sShortName[41];
    char        m_sMainKey[33];
    char        m_sTransKey[33];
    char        m_sPinKey[33];
    char        m_sMacKey[33];
    char        m_sSettleFlag[2];
    char        m_sSettleDate[15];
    char        m_sSettleNDate[15];
    char        m_sLoginFlag[2];
    char        m_sLoginTime[15];
    char        m_sBatchNo[7];
    char        m_sPosTrace[7];
    char        m_sPosType[2];
    char        m_sUpdateTime[15];
    char        m_sStatus[2];
    char        m_sRemark[301];
    char        m_sDac[17];
    int         m_nKeyIndex;
    char        m_sUmsMainKey[33];
    char        m_sUmsPinKey[33];
    char        m_sUmsMacKey[33];
    double      m_fTotalLimit;
    double      m_fPayLimit;
    int         m_nDayTimes;
}T_SVC_Miteno_Terminal;

typedef struct _svc_miteno_terminal_ind
{
    short       m_nTermId;
    short       m_nMerchantId;	
    short       m_nName;
    short       m_nShortName;
    short       m_nMainKey;
    short       m_nTransKey;
    short       m_nPinKey;
    short       m_nMacKey;
    short       m_nSettleFlag;
    short       m_nSettleDate;
    short       m_nSettleNDate;
    short       m_nLoginFlag;
    short       m_nLoginTime;
    short       m_nBatchno;
    short       m_nPosTrace;
    short       m_nPosType;
    short       m_nUpdateTime;
    short       m_nStatus;
    short       m_nRemark;
    short       m_nDac;
    short       m_nKeyIndex;
    short       m_nUmsMainKey;
    short       m_nUmsPinKey;
    short       m_nUmsMacKey;
    short       m_nTotalLimit;
    short       m_nPayLimit;
    short       m_nDayTimes;
}T_SVC_Miteno_TerminalInd;


typedef struct _svc_Miteno_detail
{
    int         m_nPtSerial;
    char        m_sPtDate[9];
    char        m_sBatchNo[9];
    char        m_sInvoice[9];
    char        m_sMerchantId[16];
    char        m_sTerminalId[9];
    int         m_nChCode;
    char        m_sChMsgid[5];
    char        m_sChTxCode[13];
    char        m_sChDate[9];
    char        m_sChTime[7];
    char        m_sChSerial[9];
    char        m_sChBatchNo[9];
    char        m_sChRetCode[13];
    char        m_sChRetMsg[101];
    char        m_sHoRetCode[13];
    char        m_sHoRetMsg[101];
    char        m_sHoSerial[21];
    char        m_sSysDate[9];
    char        m_sSysTime[7];
    char        m_sAuthNo[13];
    char        m_sRefNum[21];
    char        m_sCardNo[33];
    char        m_sTxTeller[16];
    double      m_fTxAmt;
    double      m_fChkAmt;
	double      m_fCreditAmt;
	double      m_fPayableAmt;
    char        m_sIssOrgid[21];
    char        m_sAcqOrgid[21];
    char        m_sSettleId[21];
    char        m_sStatus[2];
    char        m_sSettleDate[9];
    char        m_sSettleTime[7];
    char        m_sAcctChkFlag[2];
    char        m_sOldStatus[2];
    int         m_nOldPtSerial;
    char        m_sOldPtDate[9];
	char 		m_sOrderId[31];
	char 		m_sCouponID[51];
	char 		m_sTelNo[12];
    char        m_sDac[17];
}T_SVC_Miteno_Detail;
typedef struct _svc_Miteno_detail_ind
{
    short       m_nPtSerial;
    short       m_nPtDate;
    short       m_nBatchNo;
    short       m_nInvoice;
    short       m_nMerchantId;
    short       m_nTerminalId;
    short       m_nChCode;
    short       m_nChMsgid;
    short       m_nChTxCode;
    short       m_nChDate;
    short       m_nChTime;
    short       m_nChSerial;
    short       m_nChBatchNo;
    short       m_nChRetCode;
    short       m_nChRetMsg;
    short       m_nHoRetCode;
    short       m_nHoRetMsg;
    short       m_nHoSerial;
    short       m_nSysDate;
    short       m_nSysTime;
    short       m_nAuthNo;
    short       m_nRefNum;
    short       m_nCardNo;
    short       m_nTxTeller;
    short       m_nTxAmt;
    short       m_nChkAmt;
    short       m_nCreditAmt;
    short       m_nPayableAmt;
    short       m_nIssOrgid;
    short       m_nAcqOrgid;
    short       m_nSettleId;
    short       m_nStatus;
    short       m_nSettleDate;
    short       m_nSettleTime;
    short       m_nAcctChkFlag;
    short       m_nOldStatus;
    short       m_nOldPtSerial;
    short       m_nOldPtDate;
	short		m_nOrderId;
	short 		m_nCouponId;
	short 		m_nTelNo;
    short       m_nDac;
}T_SVC_Miteno_DetailInd;


EXEC SQL END DECLARE SECTION;

#endif
