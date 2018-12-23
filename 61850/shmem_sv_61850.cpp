#include <cstdio>
#include <cstring>

#include "share_mem.h"
#include "shmem_sv_61850.h"

static int semid_; //semaphore set identifier

ShmemSV61850 *InitShmSV61850()
{
    const char *fname = "/home/boyuu";
    int sub_id = 2;
    int sz = sizeof(ShmemSV61850);
    return (ShmemSV61850 *)InitShareMem(&semid_, sz, fname, sub_id);
}
