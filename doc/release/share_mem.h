/*! \file share_mem.h
    \brief share memory.
    Copyright (c) 2018  Xi'an Boyuu Electric, Inc.
*/
#ifndef _SHARE_MEM_H_
#define _SHARE_MEM_H_

//#include <time.h>
//#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

void *InitShareMem(int *semid, int shm_sz, const char *filename, int subid);
void FreeShareMem(void *pshmem);

//对信号量数组semnum编号的信号量做P操作
int Poperate(int semid, int semnum);
//对信号量数组semnum编号的信号量做V操作
int Voperate(int semid, int semnum);

#ifdef __cplusplus
}
#endif

#endif  //_SHARE_MEM_H_ 

