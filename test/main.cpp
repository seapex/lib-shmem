
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
        printf(" LD%d:\n", ldx+1);
        printf("ulevel=%d\n", par_ld->ulevel);
        printf("connect_type=%d\n", par_ld->connect_type);
        printf("pt=%d,%d\n", par_ld->pt[0], par_ld->pt[1]);
        printf("ct=%d,%d\n", par_ld->ct[0], par_ld->ct[1]);
        printf("vdev_lmt=%d,%d\n", par_ld->vdev_lmt[0], par_ld->vdev_lmt[1]);
        printf("unblnc_lmt=%d\n", par_ld->unblnc_lmt);
        printf("negcmp_lmt=%d\n", par_ld->negcmp_lmt);
        printf("freq_lmt=%d,%d\n", par_ld->freq_lmt[0], par_ld->freq_lmt[1]);
        printf("pst_lmt=%d, plt_lmt=%d\n", par_ld->pst_lmt, par_ld->plt_lmt);
        printf("ulmt_type=%d\n", par_ld->ulmt_type);
        printf("fluct_en=%d, fluct_db=%d\n", par_ld->fluct_en, par_ld->fluct_db);
        printf("capacity=%g,%g,%g\n", par_ld->capacity[0], par_ld->capacity[1], par_ld->capacity[2]);
        printf("evnt_en=%d\n", par_ld->evnt_en);
        printf("rvc_en=%d\n", par_ld->rvc_en);
        printf("evnt_limit=%d,%d,%d\n", par_ld->evnt_limit[0], par_ld->evnt_limit[1], par_ld->evnt_limit[2]);
        printf("rvc_lmt=%d\n", par_ld->rvc_lmt);
        printf("dc_topo=%d\n", par_ld->dc_topo);
        printf("ac_dc=%d\n", par_ld->ac_dc);
        printf("evnt_rate_lmt=%d,%d\n", par_ld->evnt_rate_lmt[0], par_ld->evnt_rate_lmt[1]);
        printf("evnt_rate_en=%d\n", par_ld->evnt_rate_en);
        printf("attntr=%d\n", par_ld->attntr);
        printf("ievnt_limit=%d\n", par_ld->ievnt_limit);
        printf("ievnt_rate_lmt=%d\n", par_ld->ievnt_rate_lmt);
        printf("ievnt_en=%d\n", par_ld->ievnt_en);
        printf("ievnt_rate_en=%d\n", par_ld->ievnt_rate_en);
        printf("ilmt_type=%d\n", par_ld->ilmt_type);
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
        //par_ld->dc_topo = 0;
        //par_ld->attntr = 0;
        par_ld->ievnt_limit = 1190;
        par_ld->ievnt_rate_lmt = 271;
        par_ld->ievnt_en = 1;
        par_ld->ievnt_rate_en = 1;
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
    if (argc < 2) {
        printf("Usage: %s LDx\n\n", argv[0]);
        return 1;
    }
    uint8_t ldx;
    sscanf(argv[1], "%hhd", &ldx);

    pshmem_ = InitShmSvr61850(); //Create share memory and initialize.
    printf("sizeof(ParamPhD)=%d, sizeof(SG4LD)=%d\n", sizeof(ParamPhD), sizeof(SG4LD));
    
    //Get SG state
    struct {
        uint8_t tol;    //Total number of SG
        uint8_t sg[4];  //SG parameter. [0-1]:active sg & edit sg of phydev(LD0); 1~
                        //[2-3]: active sg, edit sg of LD/LD1,LD2...; 1~
    } sgbuf;
    shm_SendCmd(pshmem_, kInitSGCB, NULL, 0, &sgbuf);
    printf("SGTol:%d; PhD(act,edt):%d,%d; LD(act,edt):%d,%d\n",
            sgbuf.tol, sgbuf.sg[0], sgbuf.sg[1], sgbuf.sg[2], sgbuf.sg[3]);

    ParamPhD par_phd;
    SG4LD par_ld;
    //Select active SG
    /*SelectSG(&par_phd, 0, 0, sgbuf.sg[0], 0);     //ParamPhD, active SG
    SelectSG(&par_ld, 1, 0, sgbuf.sg[2], ldx);   //SG4LD, active SG for LDx */

    //Select edit SG
    SelectSG(&par_phd, 0, 1, sgbuf.sg[0], 0);     //ParamPhD, set edit SG to active SG
    SelectSG(&par_ld, 1, 1, sgbuf.sg[2], ldx);   //SG4LD, set edit SG to active SG for LDx

    //Set SG
    SetSG(&par_ld, 1, ldx);     //set edit SG4LD
    SelectSG(&par_ld, 1, 1, sgbuf.sg[3], ldx);   //SG4LD, get edit SG for LDx */

    FreeShareMem(pshmem_);
}

