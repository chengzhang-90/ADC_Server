#include <sys/types.h>
#include <unistd.h>

#include "apr.h"
#include "apr_pools.h"
#include "apr_network_io.h"
#include "apr_shm.h"
#include "apr_signal.h"
#include "apr_hash.h"

#include "adc_base.h"

#ifndef __APPLICATION_FRAMEWORK_H__
#define __APPLICATION_FRAMEWORK_H__

struct SystemConfig
{
   int m_iListenPort;
   int m_iMaxSys;
   int m_iMaxChild;
   int m_iGrpShmKey;
   int m_iPrcShmKey;
};

class SystemManager
{
private:
   apr_socket_t    *m_pstListen;              /* 命令侦听端口 */
   apr_pool_t      *m_pstRoot;                /* 系统全局缓冲 root */
   apr_pool_t      *m_pstLocal;               /* 系统局部缓冲 root */
   char             m_aczCommand[1024];       /* 系统接收的命令 */
   SystemRegister  *m_pstSystem;              /* 子系统信息 */
   SystemConfig    *m_pstConfig;              /* 系统配置信息 */
   ProcessRegister *m_pstProcess;             /* 系统进程登记器 */

private:
   int setupSignals( void );                /* 初始化信号设置 */
   int initShareMemory( void );             /* 初始化共享内存 */

   int doCommand( apr_socket_t * );         /* 读命令执行命令 */
   int start( apr_socket_t *, char * );     /* 创建子系统 */
   int stop( apr_socket_t *, char * );      /* 停止子系统 */
   int report( apr_socket_t *, char * );    /* 列出所有子系统 */

   void maintenance( void );                  /* 维护系统信息 */

   int initSystemInfo( void );              /* 初始化系统基本信息 */

   int getEmptyIndex( void );
   int getRunningIndex( char *pczName );
   void saveBusiManager( int iIndex, char *pczName );

public:
   SystemManager();
   ~SystemManager();

   /**
     * 读系统管理器配置信息: 管理端口
     *                       最大子系统数
     *                       每个子系统允许最大进程数
     *                       共享内存 IPCKEY
     *                       子系统管理间隔时间(单位秒)
     */
   int loadConfig( char *pczXmlConfig );

   int run( void );

   void cleanup( void );
};

/**
  * 配置信息
  */
struct BusiConfig
{
   char              *m_pczKey;
   char              *m_pczVal;
   struct BusiConfig *m_pstNext;
};

/**
  * 业务子系统管理器
  */
class BusiManager
{
private:
   int    m_iSysNo;                   /* 系统编号 */
   int    m_iShmKey;                  /* 共享内存 KEY */
   int    m_iMinProcess;              /* 最小进程数 */
   int    m_iActiveProcess;           /* 活动进程数 */
   int    m_iMaxProcess;              /* 最大进程数 */
   int    m_iMaxAllowChild;           /* 最大允许进程数 */
   char   m_aczName[128];             /* 子系统名字 */
   ProcessRegister *m_pstProcess;     /* 系统进程登记器 */

   apr_socket_t *m_pstBusiListener;   /* 业务侦听 socket */
   apr_socket_t *m_pstCmdListener;    /* 命令侦听 socket */

   apr_pool_t  *m_pstPool;            /* 子系统的缓冲池 */
   apr_pool_t  *m_pMaintanence;       /* 维护过程的缓冲池 */
   apr_shm_t   *m_pstShm;             /* apache类型共享内存 */
   BusiConfig   m_stConfig;           /* 配置信息 */

private:
   void maintenance( void );            /* 维护子系统信息 */

   int  closeListener( void );

   void doCommand( apr_socket_t *pstCommand );
   void doStopCmd( apr_socket_t *pstCommand );
   void doLoadCfgCmd( apr_socket_t *pstCommand );
   void doLoadLibCmd( apr_socket_t *pstCommand );
   void doConnectDbCmd( apr_socket_t *pstCommand );

   int  makeChild( int iSlot );  /* 创建子进程 */
   void childMain( int iOffset );/* 运行子系统 */

   bool creatMoreChildren( void );
   void decreaseIdleChildren( void );
   void checkExitedChildren( void );
   void killTimeoutChildren( void );

public:
   BusiManager( char *pczName, int iSysNo, int iShmKey,
                ProcessRegister *pstProcess );
   ~BusiManager();

   int loadConfig( void );                   /* 装载子系统配置信息 */
   char *getValue( const char *pczItem );    /* 取配置项 */

   int prepare( void );

   int run( int iMaxAllocChild, SystemManager *pManager );    /* 运行子系统 */
};

#endif

