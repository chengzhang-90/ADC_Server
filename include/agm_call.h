#include "udo.h"

#ifndef __AGENT_MANAGER_CALL_H__
#define __AGENT_MANAGER_CALL_H__

typedef struct _AgmFunRegister AgmFunRegister;
struct _AgmFunRegister
{
   char   m_Name[65];
   int  (*m_Func)( UDP req, UDP res );
};

typedef struct _AgmFunManager AgmFunManager;
struct _AgmFunManager
{
   int             m_Num;
   AgmFunRegister *m_Register;
};

typedef struct _AdpTxnPara AdpTxnPara;
struct _AdpTxnPara
{
   char m_aczErrorMessage[128];
   char m_aczTranCode[21];
   int  m_iTimeOut;
   int  m_iChangeReverseMode;
   int  m_iNodeId;
};

extern AdpTxnPara stTxnPara;

const static int REVERSE_NO_CHANGE = 0;
const static int REVERSE_FORCE_NO  = 1;


#endif
