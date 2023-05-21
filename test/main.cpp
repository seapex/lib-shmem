
#include <stdio.h>
#include <string.h>

#include "share_mem.h"
#include "shmem_srv61850.h"


int main(int argc, char *argv[])
{
    ShmemSvr61850 *pshmem_ = InitShmSvr61850(); //Create share memory and initialize.

    uint8_t reqst[8];
    memset(reqst, 0, sizeof(reqst));
    ParamPhD par_phd;
    uint8_t *pchar;

    reqst[1] = 1;
    shm_SendCmd(pshmem_, kSelectActSG, reqst, 3, &par_phd);
    pchar = (uint8_t *)&par_phd;
    printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", pchar[0], pchar[1], pchar[2], pchar[3], pchar[4], pchar[5], pchar[6], pchar[7]);
    printf("model=%d id=%s\n", par_phd.device_model, par_phd.device_id);
}