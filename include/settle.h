#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "log.h"
#include "adc_syserr.h"

EXEC SQL include sqlca;
EXEC SQL include common.h;
EXEC SQL include adc_struct.h;

#ifndef __SETTLE_ACTION_H__
#define __SETTLE_ACTION_H__

/**
  * 切换系统日期和所有集中户的对账分类编号
  * 对应命令代码 000000
  * 返回值: 0 - 成功
  */
int changeDate( void );

  #ifdef __cplusplus
  extern "C" {
  #endif
     int console001_main( UDP epstInList, UDP opstOutList );
     int console007_main( UDP epstInList, UDP opstOutList );
  #ifdef __cplusplus
  }
  #endif

const static int SETTLE_DCC_FILE = 0; /*以DCC文件流水为标准对账 */
const static int SETTLE_REVERSED = 1; /*对已经冲正过的交易 */
const static int SETTLE_MORE_SER = 2; /*对平台比DCC多的流水 */

EXEC SQL BEGIN DECLARE SECTION;

struct _DccException
{
   char   m_AgentDate[9];         /* 平台日期               */
   int    m_Class;                /* 差错等级               */
   char   m_ChkNo[4];             /* 对账分类序号           */
   int    m_TotalSettle;          /* 平台对账标志           */
   int    m_VisaId;               /* 对账VisaId             */
   char   m_ErrFlag[3];           /* 对帐状态               */
   char   m_RecordDate[9];        /* 记录日期               */
   char   m_RecordTime[7];        /* 记录时间               */
   struct _FinDetail d_stFinDetail; 
   char   m_DccFile[128];         /* DCC文件名              */
   char   m_DccSerial[20];        /* DCC流水号              */
   double m_DccAmt;               /* DCC金额                */
} ;
EXEC SQL END DECLARE SECTION;

struct _DccFileLine
{
   char m_CurrCode[3];            /* 币种                    */
   char m_Amt[18];                /* 发生额: +/- + 15 +. +2  */
   char m_DccSerial[20];          /* 主机流水号              */
   char m_AgType[2];              /* 代理类别                */
   char m_Account[29];            /* 账号                    */
   char m_EndDate[9];             /* 终止签约                */
   char m_AgSerial[20];           /* 前置流水号              */
} ;

class SettleBase
{
public:
   bool                 m_TotalSettle;  /*统一对账标志*/
   int                  m_Client;       /*客户端SOCKET文件描述符*/

   int                  m_VisaId;       /*集中户签约ID*/
   struct _SysCtl       m_SysCtl;       /*系统控制表信息*/
   struct _CentCustCtl  m_CentCustCtl;  /*集中户控制信息*/
   struct _FinDetail    m_FinDetail;    /*金融明细表记录*/
   struct _DccException m_DccException; /*和DCC对账出现以外情况记录*/
   long                 m_Serial;       /*DCC转换以后流水*/
   char                 m_Date[9];      /*对账日期*/
   double               m_Amt;          /*金额*/

   struct _DccFileLine  m_DccFileLine;  /*DCC文件记录*/
   int                  m_Condition;    /*DCC文件记录和金融明细比较结果*/
                                        /*0 - 流水和金额相等*/
                                        /*1 - DCC多(金融明细记录无意义)*/
                                        /*2 - 平台多(DCC文件记录无意义)*/
                                        /*3 - 金额不符*/
                                        /*4 - 其它错*/
   FILE                *m_File;         /*DCC文件指针*/
   char                 m_DccFile[128]; /*DCC文件名*/
   char                 m_CentFileName[128];
   int                  m_ProdCode;     /*产品*/
   char                 m_OrgId[20];    /*机构号*/
   char                 m_VisaOper[20]; /*签约柜员*/
protected:
   /*根据日期和流水取金融明细*/
   int getFinDetailByDateSerial( char * epczDate, long lSerial );

   /*将主机返回文件一行转换为一个结构*/
   int getDccFileRecord( char *line );

   /*以DCC文件为准对中间业务账*/
   void settleAccountAccordingDcc( void );

   /*修改平台中多余流水为DCC无此流水*/
   void modifyInvalidRecord( void );

   /*备份指定日期(包括)以前数据*/
   int bakupData( char *epczBakupDate );

   /*设置集中户对账状态为正常*/
   int setNormalTrans( void );

   /*判断是否有新增流水, 有返回 true 没有或者出错返回 false*/
   bool hasNewFinDetail( void );

public:
   SettleBase( bool totalSettle, int centVisaId, int client , char *pszOrg, char *pszOper)
   {
      m_TotalSettle = totalSettle;
      m_VisaId      = centVisaId;
      m_Client      = client;
      sprintf ( m_OrgId, "%.*s", sizeof(m_OrgId)-1, pszOrg );
      sprintf ( m_VisaOper, "%.*s", sizeof(m_VisaOper)-1, pszOper );
   }

   void setFlagVisaId( bool totalSettle, int centVisaId )
   {
      m_TotalSettle = totalSettle;
      m_VisaId  = centVisaId;
   }

   void setFlagProdCode( bool totalSettle, int ProdCode )
   {
      m_TotalSettle = totalSettle;
      m_ProdCode  = ProdCode;
   }

   /*每次对账都会切换对账分类编号*/
   int changeClassNo( void );
   int getFileFromDcc( char *epczTransCode, long lTrace );

   int settleDcc( void );

   virtual int settleDCCWithExtend( int action ) = 0;

   int getSysCtl( void );

   int getCentCustCtl( void );

   int settleCentCust( void );

   int colectFinDetail( void );

   int bakupFinDetail( void );

   virtual int bakupExtend( void ) = 0;

   void settleReversedAccount(void);

   void handleDCCException( void );

   char * getCentFileName( void )
   {
      return m_CentFileName;
   }
   int getSettleException(const char *epczDate, char *opczCode, char *opczMsg ); 
   int getSettleGjt(const char *epczDate, char *opczCode, char *opczMsg ); 
};

/*****平台总对账*****/
class AgentSettle : public SettleBase
{
private:
   int writeFile( FILE *fp );

public:
   AgentSettle( bool totalSettle, int centVisaId, int client, char *pszOrg, char *pszVisaOper):
   SettleBase( totalSettle, centVisaId, client, pszOrg, pszVisaOper)
   {
   }


   virtual int settleDCCWithExtend( int )
   {
      return 0;
   }

   virtual int bakupExtend( void )
   {
      return 0;
   }

   int settleCentCust( void );
};

/*****test*****/
class TestSettle : public SettleBase
{
private:
   int writeFile( FILE *fp );

public:
   TestSettle( bool totalSettle, int centVisaId, int client, char *pszOrg, char *pszOper):
   SettleBase( totalSettle, centVisaId, client, pszOrg, pszOper)
   {
   }

   virtual int settleDCCWithExtend( int )
   {
      return 0;
   }

   virtual int bakupExtend( void )
   {
      return 0;
   }

   int settleCentCust( void );
};

class CommonSettle : public SettleBase
{
public:
   CommonSettle( bool totalSettle, int centVisaId, int client, char *pszOrg, char *pszOper):
   SettleBase( totalSettle, centVisaId, client, pszOrg, pszOper)
   {
   }

   virtual int settleDCCWithExtend( int )
   {
      return 0;
   }

   virtual int bakupExtend( void )
   {
      return 0;
   }

   virtual int settleCentCust( void )
   {
      return 0;
   }
};

#endif


