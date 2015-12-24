
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
   int              iNodeDscrb;   /* 0-�����ڵ�, 1-�����ڵ� */
   int              iNetProto;    /* 0-tcpip,1-sna,2-msgque */
   int              iLinkType;    /* 0-������,1-������      */
   char             aczSyncFlag[ SYNCFLAG_LEN + 1 ];
   char             aczHostIp[ HOSTIP_LEN + 1 ];
   char             aczHostName[ HOSTNAME_LEN + 1 ];
   int              iHostCommPort;
   char             aczMonitorIp[ MONITORIP_LEN + 1 ];
   char             aczMtrHostName[ MTRHOSTNAME_LEN + 1 ];
   int              iMtrPort;    /* �Ƿ��� */
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
   int              iTimeOut;            /* ��Ϊ������� */
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
   int              iRecordPerTime;            /* ��Ϊ�ӷ������ */
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
   char              aczElSerial[16];   /*�ڲ���ˮ��*/
   int               iChnNodeId;        /*�����ڵ��*/
   int               iSvcId;            /*�����*/
   int               iSubSeq;           /*�ӷ������*/
   int               iMaxRetryTime;     /*����ط�����*/
   int               iRetryTime;        /*���ط�����*/
   int               iSuccFlag;         /*�ɹ���ʶ*/
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

/*��������ձ�*/
typedef struct _TranCodeMap
{
   char aczCityCode[CITYCODE_LEN + 1]; /*������              */
   char aczTTxKind[TTXKIND_LEN + 1];   /*�����ն�����        */
   char aczTTxCode[TTXCODE_LEN + 1];   /*�����ն˽�����      */
   char aczTTxName[TTXNAME_LEN + 1];   /*�����ն˽�������    */
   char aczVTxCode[VTXCODE_LEN + 1];   /*�м�ҵ������      */
   char aczVReqCode[VREQCODE_LEN + 1]; /*�м��ѯ������      */
   char aczVTxName[VTXNAME_LEN + 1];   /*�м�ҵ��������    */
   char aczCashFlag[CASHFLAG_LEN + 1]; /*��ת��ʶ            */
   char aczKTxCode[KTXCODE_LEN + 1];   /*����ҵ��ϵͳ������  */
   char aczKTxName[KTXNAME_LEN + 1];   /*����ҵ��ϵͳ��������*/
   char aczOTxCode[OTXCODE_LEN + 1];   /*����ϵͳ������      */
   char aczOTxName[OTXNAME_LEN + 1];   /*�ⲿ����ϵͳ��������*/
   char aczBusiCode[BUSICODE_LEN + 1]; /*ҵ�����            */
   char aczAgUnitNo[AGUNITNO_LEN + 1]; /*��λ���            */
   char aczState[STATE_LEN + 1];       /*״̬                */
} TranCodeMap;

typedef struct _KernSvc
{
   int  iNodeId;                           /*�����ڵ��          */
   char aczTranCode[KERNTRANCODE_LEN + 1]; /*���Ľ�����          */
   char aczSvcName[ SVCNAME_LEN + 1 ];     /*���ķ�������        */
} KernSvc;

typedef struct _CommRoute
{
   int iNodeId;                           /*���ö���������ڵ��*/
   int iCommType;                         /*ͨѶ����0-que,1-tcp */
   char aczCond[ROUTECOND_LEN + 1];       /*·��������Ҫ���֤ȯ*/
   char aczPara[ROUTEPARA_LEN + 1];       /*·�ɲ�����ʽ:��1:��2*/
} CommRoute;

typedef struct _FldMap
{
   char aczSource[FLDMAPFLD_LEN + 1];      /*�ֶ�ӳ��Դ�ֶ�*/
   char aczTarget[FLDMAPFLD_LEN + 1];      /*�ֶ�ӳ��Ŀ���ֶ�*/
} FldMap;

typedef struct _ConstCond
{
   int iCondId;                    /*������*/
   int iLayerId;                   /*���*/
   int iSubSeq;                    /*�ӷ����ڲ���*/
   int iMsgDscrb;                  /*��������*/
   int iFldSeq;                    /*�������*/
   char aczFldType[FLDTYPE_LEN + 1];        	/*����������*/
   int iStartBit;                  		/*��ʼλ*/
   int iEndBit;                  		/*��ֹλ*/
   char aczMaxValue[MAXVALUE_LEN + 1];          /*����ֵ*/
   char aczMinValue[MINVALUE_LEN + 1];          /*����ֵ*/
   char aczFlag[FLAG_LEN + 1];          	/*ȡ����־*/
   char aczMaxFlag[MAXFLAG_LEN + 1];            /*���ޱ�־*/
   char aczMinFlag[MINFLAG_LEN + 1];            /*���ޱ�־*/
} ConstCond;

typedef struct _Ballog
{
   char aczDate[DATEYYYYMMDD_LEN + 1];          /* �������� */
   char aczTranCode[TRANCODE_LEN + 1];          /* ���״��� */
   char aczElserial[STANID_LEN + 1];            /* ��ˮ�� */
   long lChnNodeId;                             /* �����ڵ�� */
   long lSvcId;                                 /* ����� */
   long lSubSeq;                                /* �ڲ��ӷ������ */
   long MaxRetryTime;                           /* ������Դ��� */
   long lDelayTime;                             /* �������ʱ�� */
   long lRetryTime;                             /* �����Դ��� */
   int  iSuccFlag;                              /* �����ɹ���־ */
   char aczLastBalTime[TIMEHHMMSS_LEN + 1];     /* char���ϴγ���ʱ�� */
   long lLastBalTime;                           /* long���ϴγ���ʱ�� */
   char aczUdoBuf[BALUDOBUF_LEN + 1];           /* UDO���� */
} Ballog;

typedef struct _MonFldCfg
{
   int iNodeId;                    /*���ӵĽڵ�����*/
   int iMsgDesc;                   /*��������      */
   char aczMonFld[MONFLD_LEN + 1]; /*���ӱ����ֶ�  */
} MonFldCfg;

typedef struct _LogWatch
{
   int iTag;                       /* 0-��־, 1-��� */
   int iNodeId;                    /* �ڵ���       */
   int iDictId;                    /* �����ֵ���   */
   int iFldId;                     /* �ֶα��       */
   int iFldType;                   /* �ֶ�����       */
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
   int       *piRepTmFldIdx; /*�����±�?*/
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
  * �����ڴ�ͨ����
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
** ���ܣ��ӹ����ڴ��ж�ȡָ�������ݵ���̬����
** ���������1 void *epShmAddr �����ڴ��ַ 
**           2 int eiTblSerialNum �����к�
** ���������1 *opiNum ��¼����
** ����ֵ��    �ɹ����ض�̬�����ַ pstReadShmAddr��
**             ʧ�ܷ��� NULL,����*opiNum Ϊ-1;
*/
void * SHM_ReadData ( char *epShmAddr, int eiTblSerialNum, int *opiNum );

/*
**  ��������: ���ݱ���Ӧ�ú�,���������ڴ�,�õ�ĳһ�����ʼ��ַ
**            �����������
**
**  ������:   PKG_GetInfoFromShmAddr
**
**  �������: int eiMsgAppId: ����Ӧ�ú�
**            int iFldSeq   : ����
**
**  �������: pcShmStartAddr: ����Ӧ�ú�ΪMsgAppId 
**                            ����Ϊ iFldSeq����ռ���ʼ��ַ
**            * piSepFldNum    : ��������
**
**  ����ֵ:   int  0 �ɹ�, ����ʧ��
** ����˵��: �����ڲ������˿ռ�, �������ע���ͷ� 
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