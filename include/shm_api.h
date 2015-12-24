
#ifndef __SHM_API_H
#define __SHM_API_H


/*
** 功能：从共享内存中读取指定表数据
** 输入参数：1 void *epShmAddr 共享内存地址 
**           2 int eiTblSerialNum 表序列号
** 输出参数：1 *opiNum 记录条数
** 返回值：    成功返回表数据地址 
**             失败返回 NULL 并置*opiNum为 -1;
** 说明：当表为空时，返回值不为空，*opiNum等于0
*/
void * Shm_ReadData ( void * epShmAddr, int eiTblSerialNum, int * opiNum );

#endif

