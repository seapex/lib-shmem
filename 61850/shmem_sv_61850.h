/*! \file shmem_sv_61850.h
    \brief share memory for 61850 SV.
    Copyright (c) 2018  Xi'an Boyuu Electric, Inc.
*/
#ifndef _SHMEM_SV_61850_H_
#define _SHMEM_SV_61850_H_

#include <time.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    time_t t1st;    //time of 1st point
    uint32_t num;   //number of sample point. 25600, 12800 or 4000
    uint32_t reserved[5];
    uint8_t unit[4];    //[0-3]:channel. 0=10mV/A, 1=mV/A, 2=0.1mV/A, 3=0.01mV/A...
    uint8_t type[4];    //[0-3]:channel. 0=not used, 1=voltage, 2=current
    int32_t val[4][3][25600];   //[0-3]:channel; [0-2]:A-C / [0-1]:+,- for DC
} SV_1sBlock;

//共享内存数据区
struct ShmemSV61850 {
    uint32_t wdata_cnt; //write data to sv_buf count. changed by sv_server only
    uint32_t rdata_cnt; //read data from sv_buf count. changed by dpqnet_mn only
    SV_1sBlock sv_1s_block;

    uint16_t quit_cmd; //31729=quit
    uint16_t debug_var; //varialbes for debug
    uint32_t reserve[25];
    uint16_t request_cmd;  //Request command from 61850 Server
    uint16_t response_cmd; //Reaponse command from pqm3mn
    uint32_t resp_rec_num; //返回的记录数量
    uint8_t data[1024];
};

ShmemSV61850 *InitShmSV61850(); //Initialize share memory

#ifdef __cplusplus
}
#endif

#endif  //_SHMEM_SV_61850_H_ 

