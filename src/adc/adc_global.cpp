#include <stdlib.h>
#include <string.h>

#include "apr.h"
#include "apr_pools.h"
#include "adc_global.h"

/**
  * 全局变量定义 -- 开始
  */
apr_pool_t *gpstGlobalVariablePool;

typedef enum _FTYPE { STRING, SHORT, INT, LONG, FLOAT, DOUBLE, BINARY } FTYPE;

typedef struct _Vairable Variable;
struct _Vairable
{
   char *m_pczName;
   FTYPE m_tType;
   union _Value
   {
      char     *m_string;
      short int m_short;
      int       m_int;
      long      m_long;
      float     m_float;
      double    m_double;
      char     *m_binary;
   } Value;
   Variable *m_pstNext;
};

static Variable stHead;

/**
  * 该函数不对外公布, 只是在系统初始化时使用
  */
void globalInit( void )
{
   apr_pool_clear( gpstGlobalVariablePool );
   stHead.m_pstNext = NULL;
   stHead.m_pczName = NULL;
}

Variable *getVariable( char *pczName )
{
   Variable *pstTmp;

   pstTmp = stHead.m_pstNext;
   while( pstTmp != NULL )
   {
      if( strcmp( pstTmp->m_pczName, pczName ) == 0 )
      {
         return pstTmp;
      }
      pstTmp = pstTmp->m_pstNext;
   }

   return NULL;
}

void addVariable( Variable * pstVariable )
{
   pstVariable->m_pstNext = stHead.m_pstNext;
   stHead.m_pstNext = pstVariable;
}

int addInt( char *pczName, int iValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //新增变量
   {
      pstVar = (Variable*)apr_palloc( gpstGlobalVariablePool,sizeof(Variable) );
      pstVar->m_pczName = (char*)apr_palloc( gpstGlobalVariablePool,
                                            strlen(pczName)+1 );
      strcpy( pstVar->m_pczName, pczName );
      pstVar->m_tType = INT;
      addVariable( pstVar );
   }
   else
   {
      if( pstVar->m_tType != INT )
      {
         return -1;
      }
   }

   pstVar->Value.m_int = iValue;

   return 0;
}


int getInt( char *pczName, int *piValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL || pstVar->m_tType != INT )   //变量不存在
   {
     return -1;
   }

   *piValue = pstVar->Value.m_int;

   return 0;
}


int addString( char *pczName, char *pczValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //新增变量
   {
      pstVar = (Variable*)apr_palloc( gpstGlobalVariablePool,sizeof(Variable) );
      pstVar->m_pczName = (char*)apr_palloc( gpstGlobalVariablePool,
                                            strlen(pczName)+1 );
      strcpy( pstVar->m_pczName, pczName );
      pstVar->m_tType = STRING;
      pstVar->Value.m_string = NULL;
      addVariable( pstVar );
   }
   else
   {
      if( pstVar->m_tType != STRING )
      {
         return -1;
      }
   }

   pstVar->Value.m_string = (char*)apr_palloc( gpstGlobalVariablePool,
                                              strlen(pczValue)+1 );
   strcpy( pstVar->Value.m_string, pczValue );

   return 0;
}


int getString( char *pczName, char *pczValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL || pstVar->m_tType != STRING )   //变量不存在
   {
      return -1;
   }

   strcpy( pczValue, pstVar->Value.m_string );

   return 0;
}



int addShort( char *pczName, short shValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //新增变量
   {
      pstVar = (Variable*)apr_palloc( gpstGlobalVariablePool,sizeof(Variable) );
      pstVar->m_pczName = (char*)apr_palloc( gpstGlobalVariablePool,
                                            strlen(pczName)+1 );
      strcpy( pstVar->m_pczName, pczName );
      pstVar->m_tType = SHORT;
      addVariable( pstVar );
   }
   else
   {
      if( pstVar->m_tType != SHORT )
      {
         return -1;
      }
   }

   pstVar->Value.m_short = shValue;

   return 0;
}


int getShort( char *pczName, short int *pshValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL || pstVar->m_tType != SHORT )   //变量不存在
   {
     return -1;
   }

   *pshValue = pstVar->Value.m_short;

   return 0;
}


int addLong( char *pczName, long lValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //新增变量
   {
      pstVar = (Variable*)apr_palloc( gpstGlobalVariablePool,sizeof(Variable) );
      pstVar->m_pczName = (char*)apr_palloc( gpstGlobalVariablePool,
                                            strlen(pczName)+1 );
      strcpy( pstVar->m_pczName, pczName );
      pstVar->m_tType = LONG;
      addVariable( pstVar );
   }
   else
   {
      if( pstVar->m_tType != LONG )
      {
         return -1;
      }
   }

   pstVar->Value.m_long = lValue;

   return 0;
}


int getLong( char *pczName, long *plValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL || pstVar->m_tType != LONG )   //变量不存在
   {
     return -1;
   }

   *plValue = pstVar->Value.m_long;

   return 0;
}


int addFloat( char *pczName, float fValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //新增变量
   {
      pstVar = (Variable*)apr_palloc( gpstGlobalVariablePool,sizeof(Variable) );
      pstVar->m_pczName = (char*)apr_palloc( gpstGlobalVariablePool,
                                            strlen(pczName)+1 );
      strcpy( pstVar->m_pczName, pczName );
      pstVar->m_tType = FLOAT;
      addVariable( pstVar );
   }
   else
   {
      if( pstVar->m_tType != FLOAT )
      {
         return -1;
      }
   }

   pstVar->Value.m_float = fValue;

   return 0;
}


int getFloat( char *pczName, float *pfValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL || pstVar->m_tType != FLOAT )   //变量不存在
   {
     return -1;
   }

   *pfValue = pstVar->Value.m_float;

   return 0;
}



int addDouble( char *pczName, double dValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //新增变量
   {
      pstVar = (Variable*)apr_palloc( gpstGlobalVariablePool,sizeof(Variable) );
      pstVar->m_pczName = (char*)apr_palloc( gpstGlobalVariablePool,
                                            strlen(pczName)+1 );
      strcpy( pstVar->m_pczName, pczName );
      pstVar->m_tType = DOUBLE;
      addVariable( pstVar );
   }
   else
   {
      if( pstVar->m_tType != DOUBLE )
      {
         return -1;
      }
   }

   pstVar->Value.m_double = dValue;

   return 0;
}


int getDouble( char *pczName, double *pdValue )
{
   Variable *pstVar;

   pstVar = getVariable( pczName );
   if( pstVar == NULL || pstVar->m_tType != DOUBLE )   //变量不存在
   {
     return -1;
   }

   *pdValue = pstVar->Value.m_double;

   return 0;
}


int addBinary( char *pczName, void *pValue, int iLen )
{
   Variable *pstVar;
   int *piValue;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //新增变量
   {
      pstVar = (Variable*)apr_palloc( gpstGlobalVariablePool,sizeof(Variable) );
      pstVar->m_pczName = (char*)apr_palloc( gpstGlobalVariablePool,
                                            strlen(pczName)+1 );
      strcpy( pstVar->m_pczName, pczName );
      pstVar->m_tType = BINARY;
      addVariable( pstVar );
   }
   else
   {
      if( pstVar->m_tType != BINARY )
      {
         return -1;
      }
   }

   pstVar->Value.m_binary = (char*)apr_palloc( gpstGlobalVariablePool,
                                              iLen+sizeof(iLen)+1 );
   piValue = (int*)pstVar->Value.m_binary;
   *piValue = iLen;

   memcpy( pstVar->Value.m_binary+sizeof(iLen), pValue, iLen );

   return 0;
}


int getBinary( char *pczName, void *pValue, int *piLen )
{
   Variable *pstVar;
   int *piValLen;

   pstVar = getVariable( pczName );
   if( pstVar == NULL || pstVar->m_tType != BINARY )   //变量不存在
   {
     return -1;
   }

   piValLen = (int*)pstVar->Value.m_binary;
   *piLen = *piValLen;

   memcpy( pValue, pstVar->Value.m_binary+sizeof(int), *piLen );

   return 0;
}

int getLength( char *pczName )
{
   Variable *pstVar;
   int *piLen;

   pstVar = getVariable( pczName );
   if( pstVar == NULL )   //变量不存在
   {
     return -1;
   }

   switch( pstVar->m_tType )
   {
   case SHORT:
      return sizeof(short);
   case INT:
      return sizeof(int);
   case LONG:
      return sizeof(long);
   case FLOAT:
      return sizeof(float);
   case DOUBLE:
      return sizeof(double);
   case STRING:
      return strlen(pstVar->Value.m_string);
   default:
      piLen = (int*)pstVar->Value.m_string;
      return *piLen;
   }

   return -1;
}

/**
  * 全局变量定义 -- 结束
  */

