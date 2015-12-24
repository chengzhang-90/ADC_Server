#ifndef __TMS_STRUCTURE_H__
#define __TMS_STRUCTURE_H__

EXEC SQL BEGIN DECLARE SECTION;
/*终端入库表*/
typedef struct _term_in
{
    char m_sOrganId[21];
    char m_sFacId[21];
    char m_sModel[21];
    char m_sSerialNo[39];
    char m_sBatchNo[11];
    char m_sInTime[15];
    char m_sStatus[2];
    int  m_nUserId;
    char m_sRemark[121];
    char m_sUpdateTime[15];
}T_TbTermIn;
typedef struct _term_in_ind
{
    short m_nOrganId;
    short m_nFacId;
    short m_nModel;
    short m_nSerialNo;
    short m_nBatchNo;
    short m_nInTime;
    short m_nStatus;
    short m_nUserId;
    short m_nRemark;
    short m_nUpdateTime;
}T_TbTermInInd;

/*终端出库表*/
typedef struct _term_out
{
    char m_sLogicCode[21];
    int  m_nUserId;
    char m_sFacId[21];
    char m_sModel[21];
    char m_sSerialNo[39];
    char m_sOutTime[15];
    char m_sStatus[2];
    char m_sRemark[121];
    char m_sUpdateTime[15];
}T_TbTermOut;
typedef struct _term_out_ind
{
    short m_nLogicCode;
    short m_nUserId;
    short m_nFacId;
    short m_nModel;
    short m_nSerialNo;
    short m_nOutTime;
    short m_nStatus;
    short m_nRemark;
    short m_nUpdateTime;
}T_TbTermOutInd;

/*厂商信息*/
typedef struct _facturer
{
    char m_sFacId[21];
    char m_sFacName[101];
    char m_sFacShortName[61];
    char m_sLinkMan[31];
    char m_sTel[21];
    char m_sAddress[121];
    char m_sPostCode[11];
    char m_sRemark[121];
    char m_sUpdateTime[15];
}T_TbFacturer;
typedef struct _facturer_ind
{
    short m_nFacId;
    short m_nFacName;
    short m_nFacShortName;
    short m_nLinkMan;
    short m_nTel;
    short m_nAddress;
    short m_nPostCode;
    short m_nRemark;
    short m_nUpdateTime;
}T_TbFacturerInd;

/*终端型号*/
typedef struct _model
{
    char m_sFacId[21];
    char m_sModel[21];
    char m_sModelName[61];
    char m_sModelType[2];
    char m_sModelPrt[2];  /* 打印机类型 */
    char m_sModelComm[2];
    char m_sPluginName[31];  /* 下载动态库名称 */
    char m_sRemark[121];
    char m_sUpdateTime[15];
}T_TbTermModel;
typedef struct _model_ind
{
    short m_nFacId;
    short m_nModel;
    short m_nModelName;
    short m_nModelType;
    short m_nModelPrt;
    short m_nModelComm;
    short m_nPluginName;
    short m_nRemark;
    short m_nUpdateTime;
}T_TbTermModelInd;

/*下载日志信息*/
typedef struct _down_log
{
    int     m_nPtSerial;
    char    m_sPtDate[9];
    char    m_sTxCode[11];
    char    m_sTxName[61];
    char    m_sLogTime[15];
    char    m_sLogicCode[21];
    char    m_sFacId[21];
    char    m_sModel[21];
    char    m_sHardSerial[39];
    char    m_sBeginTime[15];
    char    m_sEndTime[15];
    char    m_sClientAddress[31];
    char    m_sHostRetCode[11];
    char    m_sHostRetMsg[101];
    char    m_sChRetCode[11];
    char    m_sChRetMsg[101];
    char    m_sOperId[16];
    char    m_sReqInfo[121];
    char    m_sRspInfo[121];
    char    m_sStatus[2];
    char    m_sTaskId[21];
    char    m_sRemark[81];
}T_TbDownLog;
typedef struct _down_log_ind
{
    short   m_nPtSerial;
    short   m_nPtDate;
    short   m_nTxCode;
    short   m_nTxName;
    short   m_nLogTime;
    short   m_nLogicCode;
    short   m_nFacId;
    short   m_nModel;
    short   m_nHardSerial;
    short   m_nBeginTime;
    short   m_nEndTime;
    short   m_nClientAddress;
    short   m_nHostRetCode;
    short   m_nHostRetMsg;
    short   m_nChRetCode;
    short   m_nChRetMsg;
    short   m_nOperId;
    short   m_sReqInfo;
    short   m_sRspInfo;
    short   m_nStatus;
    short   m_nTaskId;
    short   m_nRemark;
}T_TbDownLogInd;

/* 任务终端*/
typedef struct _task_logic
{
    char    m_sTaskId[21];
    char    m_sLogicCode[21];
    int     m_nUserId;
    char    m_sStatus[2];
    char    m_sRemark[81];
    char    m_sUpdateTime[15];
}T_TbTaskLogic;
typedef struct _task_logic_ind
{
    short   m_nTaskId;
    short   m_nLogicCode;
    short   m_nUserId;
    short   m_nStatus;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbTaskLogicInd;

/*逻辑终端表*/
typedef struct _logic_term
{
    char    m_sOrganId[21];
    char    m_sLogicCode[21];
    int     m_nUserId;
    char    m_sTemplateAppId[21];
    char    m_sStrategyId[21];
    char    m_sTaskId[21];
    char    m_sTmsMasterKey[33];
    char    m_sMacKey[33];
    char    m_sInitFlag[2];
    char    m_sInitTime[15];
    char    m_sRegFlag[2];
    char    m_sRegTime[15];
    char    m_sChgFlag[2];
    char    m_sChgTime[15];
    int     m_nChgNum;
    char    m_sLoginTime[15];
    int     m_nLoginNum;
    char    m_sStatus[2];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbLogicTerm;
typedef struct _logic_term_ind
{
    short   m_nOrganId;
    short   m_nLogicCode;
    short   m_nUserId;
    short   m_nTemplateApp;
    short   m_nStrategyId;
    short   m_nTaskId;
    short   m_nTmsMasterKey;
    short   m_nMacKey;
    short   m_nInitFlag;
    short   m_nInitTime;
    short   m_nRegFlag;
    short   m_nRegTime;
    short   m_nChgFlag;
    short   m_nChgTime;
    short   m_nChgNum;
    short   m_nLoginTime;
    short   m_nLoginNum;
    short   m_nStatus;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbLogicTermInd;

/*联机策略表*/
typedef struct _link_strategy
{
    char    m_sStrategyId[21];
    char    m_sStrategyName[101];
    char    m_sBeginDate[9];
    char    m_sEndDate[9];
    char    m_sBeginTime[7];
    char    m_sEndTime[7];
    int     m_nInterval;
    int     m_nRepeatNum;
    char    m_sStrategyType[2];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbLinkStrategy;
typedef struct _link_strategy_ind
{
    short   m_nStrategyId;
    short   m_nStrategyName;
    short   m_nBeginDate;
    short   m_nEndDate;
    short   m_nBeginTime;
    short   m_nEndTime;
    short   m_nInterval;
    short   m_nRepeatNum;
    short   m_nStrategyType;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbLinkStrategyInd;

/*逻辑终端号的应用信息*/
typedef struct _logic_app_info
{
    char    m_sLogicCode[21];
    char    m_sTemplateId[21];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbLogicAppInfo;
typedef struct _logic_app_info_ind
{
    short   m_nLogicCode;
    short   m_nTemplateId;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbLogicAppInfoInd;

/*应用模板信息*/
typedef struct _logic_app_template
{
    char    m_sTemplateId[21];
    char    m_sAppId[21];
    char    m_sAppVer[31];
    char    m_sParamTemp[21];
    char    m_sAppDefault[2];
    int     m_nAppIndex;
    char    m_sAppStatus[2];
    char    m_sDownFlag[2];
    char    m_sDownType[2];
    char    m_sDownTime[15];
    char    m_sUpdateTime[15];
}T_TbTemplateApp;
typedef struct _logic_app_template_ind
{
    short   m_nTemplateId;
    short   m_nAppId;
    short   m_nAppVer;
    short   m_nParamTemp;
    short   m_nAppDefault;
    short   m_nAppIndex;
    short   m_nAppStauts;
    short   m_nDownFlag;
    short   m_nDownType;
    short   m_nDownTime;
    short   m_nUpdateTime;
}T_TbTemplateAppInd;

/*模版参数表*/
typedef struct _template_param_info
{
    char    m_sTemplateId[21];
    char    m_sParamId[21];
    char    m_sIsDefault[2];
    char    m_sValue[101];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbTemplateParam;
typedef struct _template_param_info_ind
{
    short   m_nTemplateId;
    short   m_nParamId;
    short   m_nIsDefault;
    short   m_nValue;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbTemplateParamInd;

/*密钥管理*/
typedef struct _term_key
{
    int     m_nKeyId;
    char    m_sKeyIndex[3];
    char    m_sKeyValue[33];
    char    m_sLogicCode[21];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbTermKey;
typedef struct _term_key_ind
{
    short   m_nKeyId;
    short   m_nKeyIndex;
    short   m_nKeyValue;
    short   m_nLogicCode;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbTermKeyInd;

/*终端信息表*/
typedef struct _term_info
{
    char    m_sMerchantId[16];
    char    m_sTermId[9];
    char    m_sAppId[21];
    char    m_sLogicCode[21];
    char    m_sIsMasterApp[2];
    char    m_sRegTime[15];
    int     m_nKeyId;
    char    m_sKeyIndex[3];
    char    m_sMasterKey[33];
    char    m_sMasterKey2[33];
    char    m_sMacKey[33];
    char    m_sMacKeyC[33];
    char    m_sPinKey[33];
    char    m_sPinKeyC[33];
    char    m_sProvince[31];
    char    m_sCity[31];
    char    m_sCounty[31];
    char    m_sAddress[121];
    char    m_sStatus[2];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbTermInfo;
typedef struct _term_info_ind
{
    short   m_nMerchantId;
    short   m_nTermId;
    short   m_nAppId;
    short   m_nLogicCode;
    short   m_nIsMasterApp;
    short   m_nRegTime;
    short   m_nKeyId;
    short   m_nKeyIndex;
    short   m_nMasterKey;
    short   m_nMasterKey2;
    short   m_nMacKey;
    short   m_nMacKeyC;
    short   m_nPinKey;
    short   m_nPinKeyC;
    short   m_nProvince;
    short   m_nCity;
    short   m_nCounty;
    short   m_nAddress;
    short   m_nStatus;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbTermInfoInd;

/*换机记录登记表*/
typedef struct _replace_term
{
    char    m_sLogicCode[21];
    char    m_sOldFacId[21];
    char    m_sOldModel[21];
    char    m_sOldSerialNo[39];
    char    m_sNewFacId[21];
    char    m_sNewModel[21];
    char    m_sNewSerialNo[39];
    char    m_sChgType[2];
    char    m_sReason[121];
    int     m_sUserId;
    char    m_sReplaceTime[15];
    char    m_sUpdateTime[15];
}T_TbReplaceTerm;
typedef struct _replace_term_ind
{
    short   m_nLogicCode;
    short   m_nOldFacId;
    short   m_nOldModel;
    short   m_nOldSerialNo;
    short   m_nNewFacId;
    short   m_nNewModel;
    short   m_nNewSerialNo;
    short   m_nChgType;
    short   m_nReason;
    short   m_nUserId;
    short   m_nReplaceTime;
    short   m_nUpdateTime;
}T_TbReplaceTermInd;

/*授权用户*/
typedef struct _auth_user
{
    char    m_sUserId[21];
    char    m_sUserType[2];
    char    m_sUserName[101];
    char    m_sPasswd[31];
    char    m_sPwdStatus[2];
    char    m_sPwdPeriod[15];
    char    m_sUserLevel[2];
    char    m_sStatus[2];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbAuthUser;
typedef struct _auth_user_ind
{
    short   m_nUserId;
    short   m_nUserType;
    short   m_nUserName;
    short   m_nPasswd;
    short   m_nPwdStatus;
    short   m_nPwdPeriod;
    short   m_nUserLevel;
    short   m_nStatus;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbAuthUserInd;

/*下载内容明细*/
typedef struct _down_log_detail
{
    int     m_nPtSerial;
    char    m_sPtDate[9];
    char    m_sLogicCode[21];
    char    m_sAppId[21];
    char    m_sVersion[31];
    char    m_sDownInfo[2];
    char    m_sLogTime[15];
    char    m_sRemark[81];
}T_TbDownLogDetail;
typedef struct _down_log_detail_ind
{
    short   m_nPtSerial;
    short   m_nPtDate;
    short   m_nLogicCode;
    short   m_nAppId;
    short   m_nVersion;
    short   m_nDownInfo;
    short   m_nLogTime;
    short   m_nRemark;
}T_TbDownLogDetailInd;

typedef struct _param_item
{
    char    m_sParamId[21];
    char    m_sParamName[101];
    char    m_sParamType[2];
    int     m_nParamLen;
    char    m_sDefaultValue[101];
    char    m_sFromTable[31];
    char    m_sFromItem[61];
    char    m_sRemark[121];
    char    m_sUpdateTime[15];
}T_TbParam;
typedef struct _param_item_ind
{
    short   m_nParamId;
    short   m_nParamName;
    short   m_nParamType;
    short   m_nParamLen;
    short   m_nDefaultValue;
    short   m_nFromTable;
    short   m_nFromItem;
    short   m_nRemark;
    short   m_nUpdateTime;
}T_TbParamInd;

/*终端的参数信息*/
typedef struct _term_param_info
{
    char    m_sLogicCode[21];
    char    m_sAppId[21];
    char    m_sTags[9];
    char    m_sValue[161];
    char    m_sUpdateTime[15];
}T_TbTermParamInfo;
typedef struct _term_param_info_ind
{
    short   m_nLogicCode;
    short   m_nAppId;
    short   m_nTags;
    short   m_nValue;
    short   m_nUpdateTime;
}T_TbTermParamInfoInd;

/*终端的软件信息*/
typedef struct _term_soft_info
{
    char    m_sLogicCode[21];
    char    m_sAppId[21];
    char    m_sVersion[31];
    int     m_nIndex;
    char    m_sOnOff[2];
    char    m_sUpdateTime[15];
}T_TbTermSoftInfo;
typedef struct _term_soft_info_ind
{
    short   m_nLogicCode;
    short   m_nAppId;
    short   m_nVersion;
    short   m_nIndex;
    short   m_nOnOff;
    short   m_nUpdateTime;
}T_TbTermSoftInfoInd;

/*终端的硬件信息*/
typedef struct _term_hard_info
{
    char    m_sLogicCode[21];
    char    m_sSwitchError[2];
    int     m_nSwitchNum;
    char    m_sReadCardError[2];
    int     m_nReadCardNum;
    char    m_sButtonError[2];
    int     m_nButtonNum;
    char    m_sDialError[2];
    int     m_nDialNum;
    char    m_sPrintType[2];
    char    m_sPrintError[2];
    int     m_nPrintLong;
    char    m_sPrintStatus[2];
    char    m_sPinpadError[2];
    char    m_sPinpadStatus[2];
    char    m_sOtherStatus[21];
    char    m_sUpdateTime[15];
}T_TbTermHardInfo;
typedef struct _term_hard_info_ind
{
    short   m_nLogicCode;
    short   m_nSwitchError;
    short   m_nSwitchNum;
    short   m_nReadCardError;
    short   m_nReadCardNum;
    short   m_nButtonError;
    short   m_nButtonNum;
    short   m_nDialError;
    short   m_nDialNum;
    short   m_nPrintType;
    short   m_nPrintError;
    short   m_nPrintLong;
    short   m_nPrintStatus;
    short   m_nPinpadError;
    short   m_nPinpadStatus;
    short   m_nOtherStatus;
    short   m_nUpdateTime;
}T_TbTermHardInfoInd;

typedef struct _down_soft_info
{
    char    m_sLogicCode[21];
    char    m_sAppId[21];
    char    m_sVersion[31];
    char    m_sTaskId[21];
    char    m_sDownInfo[2];
    char    m_sUpdateTime[15];
    char    m_sRemark[121];
}T_TbDownSoftInfo;
typedef struct _down_soft_info_ind
{
    short   m_nLogicCode;
    short   m_nAppId;
    short   m_nVersion;
    short   m_nTaskId;
    short   m_nDownInfo;
    short   m_nUpdateTime;
    short   m_nRemark;
}T_TbDownSoftInfoInd;

typedef struct _tblogic_link_cycle
{
    char    m_sLogicCode[21];
    char    m_sBeginDate[9];
    char    m_sEndDate[9];
    char    m_sBeginTime[7];
    char    m_sEndTime[7];
    int     m_nInterval;
    char    m_sLastLinkDate[15];
    char    m_sNextLinkDate[15];
} T_TbLogicLinkCycle;
typedef struct _tblogic_link_cycle_ind
{
    short   m_nLogicCode;
    short   m_nBeginDate;
    short   m_nEndDate;
    short   m_nBeginTime;
    short   m_nEndTime;
    short   m_nInterval;
    short   m_nLastLinkDate;
    short   m_nNextLinkDate;
} T_TbLogicLinkCycleInd;

typedef struct _tb_app_program
{
    char    m_sAppId[21];
    char    m_sAppVer[31];
    char    m_sFacId[21];
    char    m_sModel[21];
    char    m_sProgramName[31];
    int     m_nFileSize;
    char    m_sUpdateTime[15];
}T_TbAppProgram;
typedef struct _tb_app_program_ind
{
    short   m_nAppId;
    short   m_nAppVer;
    short   m_nFacId;
    short   m_nModel;
    short   m_nProgramName;
    short   m_nFileSize;
    short   m_nUpdateTime;
}T_TbAppProgramInd;


EXEC SQL END DECLARE SECTION;

#endif
