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
  * �л�ϵͳ���ں����м��л��Ķ��˷�����
  * ��Ӧ������� 000000
  * ����ֵ: 0 - �ɹ�
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

const static int SETTLE_DCC_FILE = 0; /*��DCC�ļ���ˮΪ��׼���� */
const static int SETTLE_REVERSED = 1; /*���Ѿ��������Ľ��� */
const static int SETTLE_MORE_SER = 2; /*��ƽ̨��DCC�����ˮ */

EXEC SQL BEGIN DECLARE SECTION;

struct _DccException
{
   char   m_AgentDate[9];         /* ƽ̨����               */
   int    m_Class;                /* ���ȼ�               */
   char   m_ChkNo[4];             /* ���˷������           */
   int    m_TotalSettle;          /* ƽ̨���˱�־           */
   int    m_VisaId;               /* ����VisaId             */
   char   m_ErrFlag[3];           /* ����״̬               */
   char   m_RecordDate[9];        /* ��¼����               */
   char   m_RecordTime[7];        /* ��¼ʱ��               */
   struct _FinDetail d_stFinDetail; 
   char   m_DccFile[128];         /* DCC�ļ���              */
   char   m_DccSerial[20];        /* DCC��ˮ��              */
   double m_DccAmt;               /* DCC���                */
} ;
EXEC SQL END DECLARE SECTION;

struct _DccFileLine
{
   char m_CurrCode[3];            /* ����                    */
   char m_Amt[18];                /* ������: +/- + 15 +. +2  */
   char m_DccSerial[20];          /* ������ˮ��              */
   char m_AgType[2];              /* �������                */
   char m_Account[29];            /* �˺�                    */
   char m_EndDate[9];             /* ��ֹǩԼ                */
   char m_AgSerial[20];           /* ǰ����ˮ��              */
} ;

class SettleBase
{
public:
   bool                 m_TotalSettle;  /*ͳһ���˱�־*/
   int                  m_Client;       /*�ͻ���SOCKET�ļ�������*/

   int                  m_VisaId;       /*���л�ǩԼID*/
   struct _SysCtl       m_SysCtl;       /*ϵͳ���Ʊ���Ϣ*/
   struct _CentCustCtl  m_CentCustCtl;  /*���л�������Ϣ*/
   struct _FinDetail    m_FinDetail;    /*������ϸ���¼*/
   struct _DccException m_DccException; /*��DCC���˳������������¼*/
   long                 m_Serial;       /*DCCת���Ժ���ˮ*/
   char                 m_Date[9];      /*��������*/
   double               m_Amt;          /*���*/

   struct _DccFileLine  m_DccFileLine;  /*DCC�ļ���¼*/
   int                  m_Condition;    /*DCC�ļ���¼�ͽ�����ϸ�ȽϽ��*/
                                        /*0 - ��ˮ�ͽ�����*/
                                        /*1 - DCC��(������ϸ��¼������)*/
                                        /*2 - ƽ̨��(DCC�ļ���¼������)*/
                                        /*3 - ����*/
                                        /*4 - ������*/
   FILE                *m_File;         /*DCC�ļ�ָ��*/
   char                 m_DccFile[128]; /*DCC�ļ���*/
   char                 m_CentFileName[128];
   int                  m_ProdCode;     /*��Ʒ*/
   char                 m_OrgId[20];    /*������*/
   char                 m_VisaOper[20]; /*ǩԼ��Ա*/
protected:
   /*�������ں���ˮȡ������ϸ*/
   int getFinDetailByDateSerial( char * epczDate, long lSerial );

   /*�����������ļ�һ��ת��Ϊһ���ṹ*/
   int getDccFileRecord( char *line );

   /*��DCC�ļ�Ϊ׼���м�ҵ����*/
   void settleAccountAccordingDcc( void );

   /*�޸�ƽ̨�ж�����ˮΪDCC�޴���ˮ*/
   void modifyInvalidRecord( void );

   /*����ָ������(����)��ǰ����*/
   int bakupData( char *epczBakupDate );

   /*���ü��л�����״̬Ϊ����*/
   int setNormalTrans( void );

   /*�ж��Ƿ���������ˮ, �з��� true û�л��߳����� false*/
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

   /*ÿ�ζ��˶����л����˷�����*/
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

/*****ƽ̨�ܶ���*****/
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


