
#ifndef __SHM_BASE_H__
#define __SHM_BASE_H__

#define TABLE_NUMS                       17

#define CHKCFG                           0
#define MSGFLDCFG                        1
#define MSGNAMEID                        2
#define NODECOMMCFG                      3
#define NODETRANCFG                      4
#define RELACOND                         5
#define SERVICE                          6
#define SERVICECFG                       7
#define SUBSERVICE                       8
#define SUBSVCFLDSRCCFG                  9
#define SUBSVCREQCFG                     10
#define TRANFLDCFG                       11
#define RETCODEMAP                       12
#define TRANCODEMAP			 13
#define KERNSVC                          14
#define COMMROUTE                        15
#define FLDMAP                           16
#define LOGWATCH                         17





#define MONFLD_LEN			 256
#define FLDTYPE_LEN                      1
#define MAXVALUE_LEN                     20
#define MINVALUE_LEN                     20
#define FLAG_LEN                         1
#define MAXFLAG_LEN                      1
#define MINFLAG_LEN                      1
#define FLDENGNAME_LEN                   30
#define FLDCHNNAME_LEN                   30
#define LENFLDDEF_LEN                    2
#define FLDDEF_LEN                       2
#define LENFLDALIGN_LEN                  1
#define FLDALIGN_LEN                     1
#define SEPSYMBOL_LEN                    200
#define MSGAPPNAME_LEN                   40
#define NODENAME_LEN                     40
#define CFGMODE_LEN                      1
#define SYNCFLAG_LEN                     1
#define HOSTIP_LEN                       15
#define HOSTNAME_LEN                     40
#define MONITORIP_LEN                    15
#define MTRHOSTNAME_LEN                  40
#define ENCRPTTYPE_LEN                   2
#define WORKDATE_LEN                     10
#define WORKFLAG_LEN                     1
#define RESPAWNFLAG_LEN                  1
#define RUNNINGFLAG_LEN                  1
#define LOGTABLENAME_LEN                 20
#define STCD_LEN                         1
#define CRTTRANCODETYPE_LEN              50
#define CRTTRANCODEPOS_LEN               50 
#define CRTTRANCODESEQ_LEN               50 
#define CRTMSGFIXFLD_LEN                 100
#define WRGCODETYPE_LEN                  50
#define WRGTRANCODEPOS_LEN               50
#define WRGTRANCODESEQ_LEN               50
#define WRGMSGFIXFLD_LEN                 100
#define LFLDTYPE_LEN                     1
#define RELATION_LEN                     10
#define RFLDTYPE_LEN                     4
#define TRANCODE_LEN                     20
#define SVCNAME_LEN                      40
#define SVCNOTE_LEN                      100
#define SVCTRANCODE_LEN                  20
#define SUBTRANCODE_LEN                  20
#define SUBNAME_LEN                      40
#define SUCCCODE_LEN                     40
#define SRCNOTE_LEN                      40
#define REPTIMESSEQ_LEN                  50
#define REPTIMESNUM_LEN                  50
#define REPTYPE_LEN                      50
#define TRANFLD_LEN                      600
#define LOGFLD_LEN                       1200
#define RETCODE_LEN                      20
#define ERRMESG_LEN                      60
#define CITYCODE_LEN			 5
#define TTXKIND_LEN			 2
#define TTXCODE_LEN			 6
#define TTXNAME_LEN			 30
#define VTXCODE_LEN			 6
#define VREQCODE_LEN			 6
#define VTXNAME_LEN			 30
#define CASHFLAG_LEN			 1
#define KTXCODE_LEN			 6
#define KTXNAME_LEN			 30
#define OTXCODE_LEN			 6
#define OTXNAME_LEN			 30
#define BUSICODE_LEN			 4
#define AGUNITNO_LEN			 5
#define STATE_LEN			 1
#define KERNTRANCODE_LEN                 9
#define ROUTECOND_LEN                    10
#define ROUTEPARA_LEN                    40
#define FLDMAPFLD_LEN                    40
#define STANID_LEN                       21
#define DATEYYYYMMDD_LEN                 8
#define TIMEHHMMSS_LEN                   6
#define BALUDOBUF_LEN                    ( 1024 * 10 )
#define NOTES_LEN                        60
#define MACRO_LEN                        50

typedef struct _ChkCfg
{
   int              iSvcId;
   int              iCurSubSeq;
   int              iCondId;
   int              iNextSubSeq;
   int              iDfltFlag;
} ChkCfg;


typedef struct _MsgFldCfg
{
   int              iMsgAppId;
   int              iFldSeq;
   int              iSepFldSeq;
   char             aczFldEngName[ FLDENGNAME_LEN + 1 ];
   char             aczFldChnName[ FLDCHNNAME_LEN + 1 ];
   int              iSepFldFlag;
   int              iLenFldLen;
   int              iFldLen;
   int              iLenFldType;
   int              iFldType;
   char             aczLenFldDef[ LENFLDDEF_LEN + 1 ];
   char             aczFldDef[ FLDDEF_LEN + 1 ];
   char             aczLenFldAlign[ LENFLDALIGN_LEN + 1 ];
   char             aczFldAlign[ FLDALIGN_LEN + 1 ];
   char             aczSepSymbol[ SEPSYMBOL_LEN + 1 ];
   char             aczMacro[MACRO_LEN +1];
} MsgFldCfg;

typedef struct _MsgNameId
{
   int              iMsgAppId;
   char             aczMsgAppName[ MSGAPPNAME_LEN + 1 ];
} MsgNameId;

typedef struct _NodeCommCfg
{
   int              iNodeId;
   char             aczNodeName[ NODENAME_LEN + 1 ];
   int              iNodeDscrb;   /* 0-渠道节点, 1-主机节点 */
   int              iNetProto;    /* 0-tcpip,1-sna,2-msgque */
   int              iLinkType;    /* 0-短连接,1-长连接      */
   char             aczSyncFlag[ SYNCFLAG_LEN + 1 ];
   char             aczHostIp[ HOSTIP_LEN + 1 ];
   char             aczHostName[ HOSTNAME_LEN + 1 ];
   int              iHostCommPort;
   char             aczMonitorIp[ MONITORIP_LEN + 1 ];
   char             aczMtrHostName[ MTRHOSTNAME_LEN + 1 ];
   int              iMtrPort;    /* 是否监控 */
   int              iMsgAppId;
   char             aczEncrptType[ ENCRPTTYPE_LEN + 1 ];
   int              iMinProcess;
   int              iMaxProcess;
   int              iUsableFlag;
   int              iSvrKeyId;
   int              iSvwKeyId;
} NodeCommCfg;

typedef struct _NodeTranCfg
{
   int              iNodeId;
   int              iMsgAppId;
   char             aczMsgAppName[ MSGAPPNAME_LEN + 1 ];
   int              iToMidTranCodeId;
   int              iToMsgTranCodeId;
   int              iToMidId;
   int              iToMsgId;
   int              iToMidIsSt;
   int              iToMsgIsSt;
   char             aczCrtTranCodeType[ CRTTRANCODETYPE_LEN + 1 ];
   char             aczCrtTranCodePos[ CRTTRANCODEPOS_LEN + 1 ];
   char             aczCrtTranCodeSeq[ CRTTRANCODESEQ_LEN + 1 ];
   char             aczCrtMsgFixFld[ CRTMSGFIXFLD_LEN + 1 ];
   char             aczWrgTranCodeType[ WRGCODETYPE_LEN + 1 ];
   char             aczWrgTranCodePos[ WRGTRANCODEPOS_LEN + 1 ];
   char             aczWrgTranCodeSeq[ WRGTRANCODESEQ_LEN + 1 ];
   char             aczWrgMsgFixFld[ WRGMSGFIXFLD_LEN + 1 ];
   char             aczLogTabName[ LOGTABLENAME_LEN + 1 ];
   char             aczLogFld[ LOGFLD_LEN + 1];
   int              iMaxRetryTime;
   int              iDelayTime;
} NodeTranCfg;

typedef struct _RelaCond
{
   int              iCondId;
   int              iLayerId;
   int              iConstFlag;
   int              iLSubSeq;
   int              iLMsgDscrb;
   int              iLFldSeq;
   char             aczLFldType[ LFLDTYPE_LEN + 1 ];
   int              iLStartBit;
   int              iLEndBit;
   char             aczMaxValue[ MAXVALUE_LEN + 1 ];
   char             aczMinValue[ MINVALUE_LEN + 1 ];
   char             aczFlag[ FLAG_LEN + 1 ];
   char             aczMaxFlag[ MAXFLAG_LEN + 1 ];
   char             aczMinFlag[ MINFLAG_LEN + 1 ];
   char             aczRelation[ RELATION_LEN + 1 ];
   int              iRSubSeq;
   int              iRMsgDscrb;
   int              iRFldSeq;
   char             aczRFldType[ RFLDTYPE_LEN + 1 ];
   int              iRStartBit;
   int              iREndBit;
} RelaCond;

typedef struct _Service
{
   int              iNodeId;
   char             aczTranCode[ TRANCODE_LEN + 1 ];
   int              iSvcId;
   char             aczSvcName[ SVCNAME_LEN + 1 ];
   int              iTimeOut;            /* 改为服务分类 */
   int              iBalanceFlag;
   char             aczSvcNote[ SVCNOTE_LEN + 1 ];
} Service;

typedef struct _ServiceCfg
{
   int              iChnNodeId;
   char             aczSvcTranCode[ SVCTRANCODE_LEN + 1 ];
   int              iSvcId;
   int              iHostNodeId;
   int              iSubSeq;
   int              iCorrSubId;
   int              iErrorHandleType;
   int              iErrSubId;
   int              iRetryTime;
   int              iOutTime;
   int              iParentSubSeq;
   int              iSubLayer;
   int              iBatFlag;
   int              iBatMode;
   int              iLstId;
} ServiceCfg;

typedef struct _SubService
{
   int              iNodeId;
   char             aczTranCode[ TRANCODE_LEN + 1 ];
   int              iSubId;
   char             aczSubName[ SUBNAME_LEN + 1 ];
   int              iTempletId;
   int              iTranCodeSeq;
   char             aczSuccCode[ SUCCCODE_LEN + 1 ];
   int              iRecordPerTime;            /* 改为子服务分类 */
   int              iMsgAppId;
   char             aczNotes[NOTES_LEN+1];
} SubService;

typedef struct _SubSvcFldSrcCfg
{
   int              iFldAsmId;
   int              iFldFragSeq;
   int              iSrcInfType;
   int              iSrcSubSeq;
   int              iSrcFldSeq;
   int              iSrcRepFldFlag;
   int              iSrcBeginPos;
   int              iSrcLength;
   char             aczSrcNote[ SRCNOTE_LEN + 1 ];
} SubSvcFldSrcCfg;

typedef struct _SubSvcReqCfg
{
   int              iSvcId;
   int              iSubSeq;
   int              iFldSeq;
   int              iBatSubSeq;
   int              iBatSubFldSeq;
   int              iFldType;
   int              iRepFldFlag;
   int              iRepTmSubSeq;
   int              iRepTmFld;
   int              iRepTmType;
   int              iFldFragCalFlag;
   int              iFldAsmId;
   int              iBalFlag;
} SubSvcReqCfg;

typedef struct _TranFldCfg
{
   int              iNodeId;
   char             aczTranCode[ TRANCODE_LEN + 1 ];
   int              iMsgDscrb;
   char             aczRepTimesSeq[ REPTIMESSEQ_LEN + 1 ];
   char             aczRepTimesNum[ REPTIMESNUM_LEN + 1 ];
   char             aczRepType[ REPTYPE_LEN + 1 ];
   char             aczTranFld[ TRANFLD_LEN + 1 ];
} TranFldCfg;

typedef struct _BalLog
{
   char              aczElSerial[16];   /*内部流水号*/
   int               iChnNodeId;        /*渠道节点号*/
   int               iSvcId;            /*服务号*/
   int               iSubSeq;           /*子服务序号*/
   int               iMaxRetryTime;     /*最大重发次数*/
   int               iRetryTime;        /*已重发次数*/
   int               iSuccFlag;         /*成功标识*/
} BalLog;

typedef struct _RetCodeMap
{
   int iHostNodeId;
   int iChanNodeId;
   char aczChanRetCode[ RETCODE_LEN + 1 ];
   char aczChanRetMsg[ ERRMESG_LEN + 1 ];
   char aczHostRetCode[ RETCODE_LEN + 1 ];
   char aczHostRetMsg[ ERRMESG_LEN + 1 ];
   char aczFlag[2];
} RetCodeMap;

/*交易码对照表*/
typedef struct _TranCodeMap
{
   char aczCityCode[CITYCODE_LEN + 1]; /*地区号              */
   char aczTTxKind[TTXKIND_LEN + 1];   /*交易终端种类        */
   char aczTTxCode[TTXCODE_LEN + 1];   /*交易终端交易码      */
   char aczTTxName[TTXNAME_LEN + 1];   /*交易终端交易名称    */
   char aczVTxCode[VTXCODE_LEN + 1];   /*中间业务交易码      */
   char aczVReqCode[VREQCODE_LEN + 1]; /*中间查询交易码      */
   char aczVTxName[VTXNAME_LEN + 1];   /*中间业务交易名称    */
   char aczCashFlag[CASHFLAG_LEN + 1]; /*现转标识            */
   char aczKTxCode[KTXCODE_LEN + 1];   /*核心业务系统交易码  */
   char aczKTxName[KTXNAME_LEN + 1];   /*核心业务系统交易名称*/
   char aczOTxCode[OTXCODE_LEN + 1];   /*外连系统交易码      */
   char aczOTxName[OTXNAME_LEN + 1];   /*外部互连系统交易名称*/
   char aczBusiCode[BUSICODE_LEN + 1]; /*业务代号            */
   char aczAgUnitNo[AGUNITNO_LEN + 1]; /*单位编号            */
   char aczState[STATE_LEN + 1];       /*状态                */
} TranCodeMap;

typedef struct _KernSvc
{
   int  iNodeId;                           /*主机节点号          */
   char aczTranCode[KERNTRANCODE_LEN + 1]; /*核心交易码          */
   char aczSvcName[ SVCNAME_LEN + 1 ];     /*核心服务名称        */
} KernSvc;

typedef struct _CommRoute
{
   int iNodeId;                           /*配置定义的主机节点号*/
   int iCommType;                         /*通讯类型0-que,1-tcp */
   char aczCond[ROUTECOND_LEN + 1];       /*路由条件主要针对证券*/
   char aczPara[ROUTEPARA_LEN + 1];       /*路由参数形式:参1:参2*/
} CommRoute;

typedef struct _FldMap
{
   char aczSource[FLDMAPFLD_LEN + 1];      /*字段映射源字段*/
   char aczTarget[FLDMAPFLD_LEN + 1];      /*字段映射目标字段*/
} FldMap;

typedef struct _ConstCond
{
   int iCondId;                    /*条件号*/
   int iLayerId;                   /*层号*/
   int iSubSeq;                    /*子服务内部号*/
   int iMsgDscrb;                  /*报文描述*/
   int iFldSeq;                    /*报文域号*/
   char aczFldType[FLDTYPE_LEN + 1];        	/*报文域类型*/
   int iStartBit;                  		/*启始位*/
   int iEndBit;                  		/*终止位*/
   char aczMaxValue[MAXVALUE_LEN + 1];          /*上限值*/
   char aczMinValue[MINVALUE_LEN + 1];          /*下限值*/
   char aczFlag[FLAG_LEN + 1];          	/*取反标志*/
   char aczMaxFlag[MAXFLAG_LEN + 1];            /*上限标志*/
   char aczMinFlag[MINFLAG_LEN + 1];            /*下限标志*/
} ConstCond;

typedef struct _Ballog
{
   char aczDate[DATEYYYYMMDD_LEN + 1];          /* 冲正日期 */
   char aczTranCode[TRANCODE_LEN + 1];          /* 交易代码 */
   char aczElserial[STANID_LEN + 1];            /* 流水号 */
   long lChnNodeId;                             /* 渠道节点号 */
   long lSvcId;                                 /* 服务号 */
   long lSubSeq;                                /* 内部子服务序号 */
   long MaxRetryTime;                           /* 最大重试次数 */
   long lDelayTime;                             /* 冲正间隔时间 */
   long lRetryTime;                             /* 已重试次数 */
   int  iSuccFlag;                              /* 冲正成功标志 */
   char aczLastBalTime[TIMEHHMMSS_LEN + 1];     /* char型上次冲正时间 */
   long lLastBalTime;                           /* long型上次冲正时间 */
   char aczUdoBuf[BALUDOBUF_LEN + 1];           /* UDO数据 */
} Ballog;

typedef struct _MonFldCfg
{
   int iNodeId;                    /*监视的节点名称*/
   int iMsgDesc;                   /*报文类型      */
   char aczMonFld[MONFLD_LEN + 1]; /*监视报文字段  */
} MonFldCfg;

typedef struct _LogWatch
{
   int iTag;                       /* 0-日志, 1-监控 */
   int iNodeId;                    /* 节点编号       */
   int iDictId;                    /* 数据字典编号   */
   int iFldId;                     /* 字段编号       */
   int iFldType;                   /* 字段类型       */
} LogWatch;


/*****added at 2001/09/20**************/
typedef struct _ChkCfgAry
{
   int iNum;
   ChkCfg *pstChkCfg;
} ChkCfgAry;


typedef struct _MsgFldCfgAry
{
   int iNum;
   MsgFldCfg *pstMsgFldCfg;
} MsgFldCfgAry;

typedef struct _MsgFldCfgAry1
{
   int iNum;
   MsgFldCfg *pstMsgFldCfg;
   int       *piRepTmFldIdx; /*数组下标?*/
} MsgFldCfgAry1;

typedef struct _MsgNameIdAry
{
   int iNum;
   MsgNameId *pstMsgNameId;
} MsgNameIdAry;

typedef struct _NodeCommCfgAry
{
   int iNum;
   NodeCommCfg *pstNodeCommCfg;
} NodeCommCfgAry;

typedef struct _NodeTranCfgAry
{
   int iNum;
   NodeTranCfg *pstNodeTranCfg;
} NodeTranCfgAry;

typedef struct _RelaCondAry
{
   int iNum;
   RelaCond *pstRelaCond;
} RelaCondAry;

typedef struct _ServiceAry
{
   int iNum;
   Service *pstService;
} ServiceAry;

typedef struct _ServiceCfgAry
{
   int iNum;
   ServiceCfg *pstServiceCfg;
} ServiceCfgAry;

typedef struct _SubServiceAry
{
   int iNum;
   SubService *pstSubService;
} SubServiceAry;

typedef struct _SubSvcFldSrcCfgAry
{
   int iNum;
   SubSvcFldSrcCfg *pstSubSvcFldSrcCfg;
} SubSvcFldSrcCfgAry;

typedef struct _SubSvcReqCfgAry
{
   int iNum;
   SubSvcReqCfg *pstSubSvcReqCfg;
} SubSvcReqCfgAry;

typedef struct _TranFldCfgAry
{
   int iNum;
   TranFldCfg *pstTranFldCfg;
} TranFldCfgAry;

typedef struct _RetCodeMapAry
{
   int iNum;
   RetCodeMap *pstRetCodeMap;
} RetCodeMapAry;

typedef struct _TranCodeMapAry
{
   int iNum;
   TranCodeMap *pstTranCodeMap;
} TranCodeMapAry;

typedef struct _KernSvcAry
{
   int iNum;
   KernSvc *pstKernSvc;
} KernSvcAry;

typedef struct _CommRouteAry
{
   int iNum;
   CommRoute *pstCommRoute;
} CommRouteAry;

typedef struct _FldMapAry
{
   int iNum;
   FldMap *pstFldMap;
} FldMapAry;

typedef struct _ConstCondAry
{
   int iNum;
   ConstCond *pstConstCond;
} ConstCondAry;

/**
  * 共享内存通信区
  */
typedef struct _ShmCa
{
   int    m_iStatus;
   time_t m_tUpdateTime;
} ShmCa;

#define SHM_LOADING  0
#define SHM_LOADED   1
#define SHM_UPDATED  2

/***********************
ChkCfgAry          stChkCfgAry;
MsgFldCfgAry       stMsgFldCfgAry;
MsgNameIdAry       stMsgNameIdAry;
NodeCommCfgAry     stNodeCommCfgAry;
NodeTranCfgAry     stNodeTranCfgAry;
RelaCondAry        stRelaCondAry;
ServiceAry         stServiceAry;
ServiceCfgAry      stServiceCfgAry;
SubServiceAry      stSubServiceAry;
SubSvcReqCfgAry    stSubSvcReqCfgAry;
SubSvcFldSrcCfgAry stSubSvcFldSrcCfgAry;
TranFldCfgAry      stTranFldCfgAry;
RetCodeMapAry      stRetCodeMapAry;
TranCodeMapAry     stTranCodeMapAry;
KernSvcAry         stKernSvcAry;
CommRouteAry       stCommRouteAry;
FldMapAry          stFldMapAry;
ConstCondAry       stConstCondAry;
***********************/

static char *paczTableName[] = { "chkcfg",
                                 "msgfldcfg",
                                 "msgnameid",
                                 "nodecommcfg",
                                 "nodetrancfg",
                                 "relacond",
                                 "service",
                                 "servicecfg",
                                 "subservice",
                                 "subsvcfldsrccfg",
                                 "subsvcreqcfg",
                                 "tranfldcfg",
                                 "retcodemap",
                                 "trancode",
                                 "kernsvc",
				 "commroute",
				 "fldmap",
				 "logwatch" };

static int aiTableSize[] = { sizeof( ChkCfg ) + sizeof(ShmCa),
                             sizeof( MsgFldCfg ) + sizeof(ShmCa),
                             sizeof( MsgNameId ) + sizeof(ShmCa),
                             sizeof( NodeCommCfg ) + sizeof(ShmCa),
                             sizeof( NodeTranCfg ) + sizeof(ShmCa),
                             sizeof( RelaCond ) + sizeof(ShmCa),
                             sizeof( Service ) + sizeof(ShmCa),
                             sizeof( ServiceCfg ) + sizeof(ShmCa),
                             sizeof( SubService ) + sizeof(ShmCa),
                             sizeof( SubSvcFldSrcCfg ) + sizeof(ShmCa),
                             sizeof( SubSvcReqCfg ) + sizeof(ShmCa),
                             sizeof( TranFldCfg ) + sizeof(ShmCa),
                             sizeof( RetCodeMap ) + sizeof(ShmCa),
                             sizeof( TranCodeMap ) + sizeof(ShmCa),
                             sizeof( KernSvc )  + sizeof(ShmCa),
                             sizeof( CommRoute )  + sizeof(ShmCa),
                             sizeof( FldMap ) + sizeof(ShmCa),
                             sizeof( LogWatch ) + sizeof(ShmCa) };


#ifdef __cplusplus
extern "C" {
#endif

/*
** 功能：从共享内存中读取指定表数据到动态数组
** 输入参数：1 void *epShmAddr 共享内存地址 
**           2 int eiTblSerialNum 表序列号
** 输出参数：1 *opiNum 记录条数
** 返回值：    成功返回动态数组地址 pstReadShmAddr，
**             失败返回 NULL,并置*opiNum 为-1;
*/
void * SHM_ReadData ( char *epShmAddr, int eiTblSerialNum, int *opiNum );

/*
**  函数功能: 根据报文应用号,读表共享内存,得到某一域的起始地址
**            及其分域数量
**
**  函数名:   PKG_GetInfoFromShmAddr
**
**  输入参数: int eiMsgAppId: 报文应用号
**            int iFldSeq   : 域编号
**
**  输出参数: pcShmStartAddr: 报文应用号为MsgAppId 
**                            域编号为 iFldSeq的域空间起始地址
**            * piSepFldNum    : 分域数量
**
**  返回值:   int  0 成功, 其他失败
** 操作说明: 函数内部分配了空间, 请调用者注意释放 
*/

int  PKG_GetInfoFromShmAddr( int eiMsgAppId, 
                             int iFldSeq,
			     MsgFldCfg * pcShmStartAddr,
			     int * piSepFldNum );

#ifdef __cplusplus
}
#endif

#endif

/*
**end of file
*/
