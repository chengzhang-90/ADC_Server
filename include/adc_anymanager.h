#include <sys/types.h>
#include <unistd.h>

#include "apr.h"
#include "apr_pools.h"
#include "apr_network_io.h"
#include "apr_shm.h"
#include "apr_signal.h"
#include "apr_hash.h"

#include "adc_base.h"
#include "pub_base.h"

#ifndef __APPLICATION_FRAMEWORK_H__
#define __APPLICATION_FRAMEWORK_H__

/**
  * 配置信息
  */
struct ItemConfig
{
   char              *m_pczKey;
   char              *m_pczVal;
   struct ItemConfig *m_pstNext;
};

/**
  * 业务子系统管理器
  */
class AnyManager
{
private:
   int    m_iIndex;                   //系统编号
   int    m_iGrpShmKey;               //组信息管理共享内存 KEY
   int    m_iProcShmKey;              //进程管理共享内存 KEY
   int    m_iCfgShmKey;               //配置信息共享内存 KEY
   int    m_iMinProcess;              //最小进程数
   int    m_iActiveProcess;           //活动进程数
   int    m_iMaxProcess;              //最大进程数
   int    m_iMaxAllowProcess;         //最大允许进程数
   char   m_aczName[128];             //子系统名字

   ProcessRegister *m_pstProcess;     //进程登记管理器
   SystemRegister  *m_pstSystem;      //系统信息管理器

   apr_socket_t *m_pstListener;       //业务侦听 socket

   apr_pool_t  *m_pstPool;            //子系统的缓冲池
   apr_pool_t  *m_pstMaintanence;       //维护过程的缓冲池

   ItemConfig   m_stConfig;           //配置信息

private:
   void maintenance( void );          //维护子系统信息

   int  closeListener( void );

   void doCommand( void );
   void doStopCmd( void );
   void doLoadCfgCmd( void );

   int  makeChild( int iSlot );  //创建子进程
   void childMain( int iOffset );//运行子系统

   bool creatMoreChildren( void );
   void decreaseIdleChildren( void );
   void checkExitedChildren( bool bStoping );
   void killTimeoutChildren( void );
   void setupSignal( void );    //忽略信号

public:
   AnyManager( char *pczName, int iIndex, int iMaxChild );
   ~AnyManager();

   int loadConfig( void );                   //装载子系统配置信息
   char *getValue( const char *pczItem );    //取配置项

   int prepare( int iGrpShmKey, int iProcShmKey );

   /**
     * 运行子系统
     */
   int run( void );    //运行子系统
};

#endif

