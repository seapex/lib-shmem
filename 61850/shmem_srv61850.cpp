#include <cstdio>
#include <cstring>

#include <poll.h>
#include "share_mem.h"
#include "shmem_srv61850.h"

static int semid_; //semaphore set identifier

ShmemSvr61850 *InitShmSvr61850()
{
    const char *fname = "/home/boyuu";
    int sub_id = 1;
    int sz = sizeof(ShmemSvr61850);
    return (ShmemSvr61850 *)InitShareMem(&semid_, sz, fname, sub_id);
}

/*!
Sleep x ms
*/
void msSleep(int x)
{
    poll(NULL, 0, x);
}

/*!
Send cmd to share memory in safe mode

    Input:  p -- pointer to share memory
            cmd -- command
            reqst -- request param
            size -- request param size
    Output: resp -- response data
    Return: number of return param. -1=failure
*/
int shm_SendCmd(ShmemSvr61850 *p, shm_APICmd cmd, void *reqst, int size, void *resp)
{
    if ( Poperate(semid_, kSemNmCommand) < 0 ) {
        printf("P operate error\n");
        return -1 ;
    }
    
    int ret = 0;
    if (reqst&&size) memcpy(p->data, reqst, size);
    p->request_cmd = cmd;
    int cnt = 0;
    while(p->request_cmd) {
        msSleep(20);
        if(++cnt > 100) {
            printf("Pqm3mn no response! p->request_cmd=%d (%s %d)\n", p->request_cmd, __FILE__, __LINE__);
            ret = -1;
            break;
        }
    }
    if(!ret&&p->response_cmd != cmd) {
        printf("p->request_cmd = %d, p->reponse_cmd = %d (%s %d)\n", p->request_cmd, p->response_cmd, __FILE__, __LINE__);
        ret = -1;
    }
    if (!ret) ret = p->resp_rec_num;
    if (resp&&ret>0) memcpy(resp, p->data, ret);
        
    if ( Voperate(semid_, kSemNmCommand) < 0 ) {
        printf("V operate error\n") ;
        return -1 ;
    }
    return ret;
}

/*!
Set share memory value under semaphore control

    Input:  p -- pointer to share memory
            dataid --
            value --
            idx -- logical device index
    Return: 0=success, <0=failure
*/
int SafeSetShm(ShmemSvr61850 *p, SafeSetDataID dataid, void * value, int idx)
{
    int sem_num;
    char * pch;
    int sz = 1;
    switch(dataid) {
        case kBrcbUpdate:
            pch = &p->ld_data[idx].brcb_up;
            sem_num = 0;
            break;
        case kUrcbUpdate:
            pch = &p->ld_data[idx].urcb_up;
            sem_num = 1;
            break;
        case kDRcdMade:
            pch = &p->ld_data[idx].rdre.rcdmade;
            sem_num = 2;
            break;
        default:
            printf("unknown dataid!\n");
            return -1;
    }
    if ( Poperate(semid_, sem_num) < 0 ) {
        perror("P operate error");
        return -1 ;
    }
    memcpy(pch, value, sz);
    if ( Voperate(semid_, sem_num) < 0 ) {
        perror("V operate error");
        return -1 ;
    }
    return 0;
}

