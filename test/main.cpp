
#include <stdio.h>
#include <string.h>

#include "share_mem.h"
#include "shmem_srv61850.h"

ShmemSvr61850 *pshmem_;

struct SGCBInfo {
    uint8_t tol;    //total number of sg,
    uint8_t act;    //PHD(LD0) active sg,
    uint8_t edt;    //PHD(LD0) edit sg,
    uint8_t ld_act;    //LDx active sg,
    uint8_t ld_edt;    //LDx edit sg
};

/*!
Select Setting Group

    Input:  type -- 0=ParamPhD, 1=SG4LD
            sgt -- SG type. 0=edit, 1=active
            sg_id -- 1~
            ldx -- LD index. 0=LD1,1=LD2..
    Ouput:  resp -- response. ParamPhD or SG4LD
*/
const char* kTypeName[] = {"ParamPhD", "SG4LD"};
const char* kSGTypeName[] = {"edit", "active"};
void SelectSG(void *resp, uint8_t type, uint8_t sgt, uint8_t sg_id, uint8_t ldx)
{
    uint8_t reqst[4];
    reqst[0] = type;
    reqst[1] = sg_id;
    reqst[2] = ldx;
    if (sgt) {  //active
        shm_SendCmd(pshmem_, kSelectActSG, reqst, 3, resp);
    } else {    //edit
        shm_SendCmd(pshmem_, kSelectEditSG, reqst, 3, resp);
    }

    ParamPhD *par_phd;
    SG4LD *par_ld;
    printf("Select %s %s SG %d.", kTypeName[type], kSGTypeName[sgt], sg_id);
    if (type) { //SG4LD
        par_ld = (SG4LD*)resp;
        printf(" LD%d\n", ldx+1);
        printf("vdev_lmt=%d,%d; freq_lmt=%d,%d\n", par_ld->vdev_lmt[0], par_ld->vdev_lmt[1], par_ld->freq_lmt[0], par_ld->freq_lmt[1]);
    } else {    //ParamPhD
        par_phd = (ParamPhD*)resp;
        printf("\n");
    }
}

/*!
Set Setting Group

    Input:  par -- ParamPhD or SG4LD
            type -- 0=ParamPhD, 1=SG4LD
            ldx -- LD index. 0=LD1,1=LD2.. only for SG4LD
    Ouput:  par -- response. ParamPhD or SG4LD
*/
void SetSG(void *par, uint8_t type, uint8_t ldx)
{
    uint8_t reqst[1024];
    reqst[0] = type;
    reqst[1] = ldx;

    printf("Set %s SG.", kTypeName[type]);
    ParamPhD *par_phd;
    SG4LD *par_ld;
    int sz;
    if (type) { //SG4LD
        sz = sizeof(SG4LD);
        par_ld = (SG4LD*)par;
        par_ld->vdev_lmt[0] = 75;
        par_ld->vdev_lmt[1] = 65;
        par_ld->freq_lmt[0] = 195;
        par_ld->freq_lmt[1] = 205;
        printf(" LD%d\n", ldx+1);
    } else {    //ParamPhD
        sz = sizeof(ParamPhD);
        par_phd = (ParamPhD*)par;
        printf("\n");
    }
    memcpy(reqst+2, par, sz);

    shm_SendCmd(pshmem_, kConfEditSG, reqst, sz+2, par);
}

int main(int argc, char *argv[])
{
    pshmem_ = InitShmSvr61850(); //Create share memory and initialize.
    printf("sizeof(ParamPhD)=%d, sizeof(SG4LD)=%d\n", sizeof(ParamPhD), sizeof(SG4LD));

    SGCBInfo sgcb_info;
    ParamPhD par_phd;
    SG4LD par_ld;
    uint8_t ldx = 0;
    
    //Get SG state
    shm_SendCmd(pshmem_, kInitSGCB, NULL, 0, &sgcb_info);
    printf("SGCB:%d;%d,%d;%d,%d\n", sgcb_info.tol, sgcb_info.act, sgcb_info.edt, sgcb_info.ld_act, sgcb_info.ld_edt);

    //Select active SG
    SelectSG(&par_phd, 0, 0, sgcb_info.act, 0);     //ParamPhD, active SG
    SelectSG(&par_ld, 1, 0, sgcb_info.ld_act, ldx);   //SG4LD, active SG for LDx

    //Select edit SG
    //SelectSG(&par_phd, 0, 1, sgcb_info.act, 0);     //ParamPhD, set edit SG to active SG
    //SelectSG(&par_ld, 1, 1, sgcb_info.ld_act, ldx);   //SG4LD, set edit SG to active SG for LDx

    //Set SG
    SetSG(&par_ld, 1, ldx);     //ParamPhD, set SG for LDx
    SelectSG(&par_ld, 1, 1, sgcb_info.ld_act, ldx);   //SG4LD, active SG for LDx

    FreeShareMem(pshmem_);
}

