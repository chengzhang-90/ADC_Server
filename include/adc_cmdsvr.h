#include <sys/types.h>
#include <unistd.h>

#include "apr.h"
#include "apr_pools.h"
#include "apr_network_io.h"
#include "apr_shm.h"
#include "apr_signal.h"
#include "apr_hash.h"

#include "adc_base.h"

#ifndef __APPLICATION_COMMAND_SERVER_H__
#define __APPLICATION_COMMAND_SERVER_H__

struct CommandConfig
{
   int m_iPort;                /* 系统命令端口 */
   int m_iGrpShmKey;           /* 系统管理共享内存 KEY */
   int m_iPrcShmKey;           /* 系统处理进程管理内存 KEY */
   int m_iMaxSys;              /* 最大子系统个数 */
   int m_iMaxChildPerSys;      /* 每个子系统最多进程数 */
};

/**
  * 系统管理器
  */
class CommandServer
{
private:
   apr_socket_t    *m_pstListen;              /* 命令侦听端口 */
   apr_pool_t      *m_pstRoot;                /* 系统全局缓冲 root */
   apr_pool_t      *m_pstLocal;               /* 系统局部缓冲 root */
   char             m_aczCommand[1024];       /* 系统接收的命令 */
   SystemConfig    *m_pstConfig;              /* 系统配置信息 */
   SystemRegister  *m_pstSystem;              /* 子系统信息 */
   ProcessRegister *m_pstProcess;             /* 系统进程登记器 */

private:
   int setupSignals( void );                /* 初始化信号设置 */
   int initShareMemory( void );             /* 初始化共享内存 */

   int doCommand( apr_socket_t * );         /* 读命令执行命令 */
   int start( apr_socket_t *, char * );     /* 创建子系统 */
   int stop( apr_socket_t *, char * );      /* 停止子系统 */
   int report( apr_socket_t *, char * );    /* 列出所有子系统 */

   pid_t runAnyManager( char *, int );        /* 运行 */

   void maintenance( void );                /* 维护系统信息 */

   int getEmptyIndex( void );
   int getRunningIndex( char *pczName );
   void saveBusiManager( int iIndex, char *pczName, pid_t );

   void checkExitedSystem( void );

public:
   CommandServer();
   ~CommandServer();

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

#endif

