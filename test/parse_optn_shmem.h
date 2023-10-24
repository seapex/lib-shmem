/*! \file parse_optn_shmem.h
    \brief Parse command line option for tst_shmem.
*/
#ifndef _PARSE_OPTN_SHMEM_H_
#define _PARSE_OPTN_SHMEM_H_

#include <stdint.h>
//#include <getopt.h>
#include "parse_option.h"

enum SubCmdType { kCmdTypeEnd=1 };

class ParseOptnShmem:public ParseOption {
public:
    ParseOptnShmem();
    ~ParseOptnShmem(){};

    //Accessors
    uint8_t shm_cmd() { return shm_cmd_; }
    uint8_t dev() { return dev_; }
    uint8_t ldx() { return ldx_; }
    uint8_t sgn() { return sgn_; }
    uint8_t sgt() { return sgt_; }

protected:
    int HandleMain(int opt, char *arg=NULL);
    int HandleSubcmd(int opt, char *arg=NULL);
    int HandleOther(char *arg[], int argc);
private:
    void InitParam();
        
    uint8_t shm_cmd_;   //shmem command type. 0=read, 1=set, 2=select
    uint8_t dev_;   //device type. 0=phy, 1=LD
    uint8_t ldx_;   //LD index. 0=LD1, 1=LD2...
    uint8_t sgn_;   //SettingGroupNumber. 1~4
    uint8_t sgt_;   //setting group type. 0=active, 1=edit
};


#endif //_PARSE_OPTN_SHMEM_H_