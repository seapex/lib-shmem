
#include <stdio.h>
#include <string.h>

#include "share_mem.h"
#include "shmem_srv61850.h"
#include "parse_optn_shmem.h"

ShmemSvr61850 *pshmem_;

struct SGCBInfo {
    uint8_t tol;    //total number of sg,
    uint8_t act;    //PHD(LD0) active sg,
    uint8_t edt;    //PHD(LD0) edit sg,
    uint8_t ld_act;    //LDx active sg,
    uint8_t ld_edt;    //LDx edit sg
};

/*!
Load ParamPhD from file

    Output: par_phd -- 
*/
void LoadParamPhD(ParamPhD *par_phd)
{
    char *fname = "paramphd.txt";
    FILE *fstream = fopen(fname, "r");
    if (fstream == NULL) {
        printf("Can't open file %s\n", fname);
        return;
    }
    char stri[8];
    fscanf(fstream, "device_model=%hd\n", &par_phd->device_model);
    fscanf(fstream, "device_id=%s\n", &par_phd->device_id);
    fscanf(fstream, "stts_spc=%hd", &par_phd->stts_spc[0]);
    for (int i=1; i<kPQParaEnd; i++) fscanf(fstream, ",%hd", &par_phd->stts_spc[i]);
    fgets(stri, 8, fstream);
    fscanf(fstream, "ip[0]=%hhd.%hhd.%hhd.%hhd\n", &par_phd->ip[0][0], &par_phd->ip[0][1], &par_phd->ip[0][2], &par_phd->ip[0][3]);
    fscanf(fstream, "ip[1]=%hhd.%hhd.%hhd.%hhd\n", &par_phd->ip[1][0], &par_phd->ip[1][1], &par_phd->ip[1][2], &par_phd->ip[1][3]);
    fscanf(fstream, "port=%hd,%hd\n", &par_phd->port[0], &par_phd->port[1]);
    fscanf(fstream, "mask[0]=%hhd.%hhd.%hhd.%hhd\n", &par_phd->mask[0][0], &par_phd->mask[0][1], &par_phd->mask[0][2], &par_phd->mask[0][3]);
    fscanf(fstream, "mask[1]=%hhd.%hhd.%hhd.%hhd\n", &par_phd->mask[1][0], &par_phd->mask[1][1], &par_phd->mask[1][2], &par_phd->mask[1][3]);
    fscanf(fstream, "mac[0]=%hhx:%hhx:%hhx:%hhx\n", &par_phd->mac[0][0], &par_phd->mac[0][1], &par_phd->mac[0][2], &par_phd->mac[0][3]);
    fscanf(fstream, "mac[1]=%hhx:%hhx:%hhx:%hhx\n", &par_phd->mac[1][0], &par_phd->mac[1][1], &par_phd->mac[1][2], &par_phd->mac[1][3]);
    fscanf(fstream, "gate[0]=%hhd.%hhd.%hhd.%hhd\n", &par_phd->gate[0][0], &par_phd->gate[0][1], &par_phd->gate[0][2], &par_phd->gate[0][3]);
    fscanf(fstream, "gate[1]=%hhd.%hhd.%hhd.%hhd\n", &par_phd->gate[1][0], &par_phd->gate[1][1], &par_phd->gate[1][2], &par_phd->gate[1][3]);
    fscanf(fstream, "frqm_spc=%hhd\n", &par_phd->frqm_spc);
    fscanf(fstream, "hrm_type=%hhd\n", &par_phd->hrm_type);
    fscanf(fstream, "ihrm_type=%hhd\n", &par_phd->ihrm_type);
    fscanf(fstream, "man_rec_en=%hhd\n", &par_phd->man_rec_en);
    fscanf(fstream, "evnt_tol=%hhd\n", &par_phd->evnt_tol);
    fscanf(fstream, "cmtrd_sv_meth=%hhd\n", &par_phd->cmtrd_sv_meth);
    fscanf(fstream, "stdy_trgr_ena=%hhx\n", &par_phd->stdy_trgr_ena);
    fscanf(fstream, "synctm_type=%hhd\n", &par_phd->synctm_type);
    fscanf(fstream, "evnt_max_dur=%hd\n", &par_phd->evnt_max_dur);
    fscanf(fstream, "evnt_end_num=%hd\n", &par_phd->evnt_end_num);
    fscanf(fstream, "sys_zone=%hhd\n", &par_phd->sys_zone);
    fscanf(fstream, "gps_zone_t=%hhd\n", &par_phd->gps_zone_t);
    fscanf(fstream, "ntp_ip=%hhd.%hhd.%hhd.%hhd\n", &par_phd->ntp_ip[0], &par_phd->ntp_ip[1], &par_phd->ntp_ip[2], &par_phd->ntp_ip[3]);
    fscanf(fstream, "commu_num=%hhd,%hhd\n", &par_phd->commu_num[0], &par_phd->commu_num[1]);
    fscanf(fstream, "baud=%d,%d\n", &par_phd->baud[0], &par_phd->baud[1]);
    fscanf(fstream, "prtcl=%hhd,%hhd;%hhd,%hhd\n", &par_phd->prtcl[0][0], &par_phd->prtcl[0][1], &par_phd->prtcl[1][0], &par_phd->prtcl[1][1]);
    fscanf(fstream, "alarm_ena=%hhx\n", &par_phd->alarm_ena);
    fscanf(fstream, "cvt_ena=%hhd\n", &par_phd->cvt_ena);
    fscanf(fstream, "cvt_cc=%hhd\n", &par_phd->cvt_cc);
    fscanf(fstream, "cvt_chnl=%hhd,%hhd,%hhd\n", &par_phd->cvt_chnl[0], &par_phd->cvt_chnl[1], &par_phd->cvt_chnl[2]);
    fscanf(fstream, "cvt_cnct=%hhd,%hhd;%hhd,%hhd;%hhd,%hhd\n", &par_phd->cvt_cnct[0][0], &par_phd->cvt_cnct[0][1], &par_phd->cvt_cnct[1][0], &par_phd->cvt_cnct[1][1], &par_phd->cvt_cnct[2][0], &par_phd->cvt_cnct[2][1]);
    fscanf(fstream, "cvt_c1c2=%g,%g;%g,%g;%g,%g\n", &par_phd->cvt_c1c2[0][0], &par_phd->cvt_c1c2[0][1], &par_phd->cvt_c1c2[1][0], &par_phd->cvt_c1c2[1][1], &par_phd->cvt_c1c2[2][0], &par_phd->cvt_c1c2[2][1]);
    fscanf(fstream, "intrplt_t=%hhd,%hhd\n", &par_phd->intrplt_t[0], &par_phd->intrplt_t[1]);
    fscanf(fstream, "softver=%hhd.%hhd.%hhd\n", &par_phd->softver[0], &par_phd->softver[1], &par_phd->softver[2]);
    fscanf(fstream, "cyc10_ena=%hhd\n", &par_phd->cyc10_ena);
    fscanf(fstream, "cyc10_sz=%hhd\n", &par_phd->cyc10_sz);
    fscanf(fstream, "cyc150_ena=%hhd\n", &par_phd->cyc150_ena);
    fscanf(fstream, "cyc150_sz=%hhd\n", &par_phd->cyc150_sz);
    fscanf(fstream, "sig_simu=%hhd\n", &par_phd->sig_simu);
    fscanf(fstream, "input0_thr=%hhd,%hhd\n", &par_phd->input0_thr[0], &par_phd->input0_thr[1]);
    fscanf(fstream, "cvt_r1r2=%g,%g;%g,%g;%g,%g\n", &par_phd->cvt_r1r2[0][0], &par_phd->cvt_r1r2[0][1], &par_phd->cvt_r1r2[1][0], &par_phd->cvt_r1r2[1][1], &par_phd->cvt_r1r2[2][0], &par_phd->cvt_r1r2[2][1]);
    fscanf(fstream, "time_diff=%hd\n", &par_phd->time_diff);
    fscanf(fstream, "security_ena=%hhd\n", &par_phd->security_ena);
    fscanf(fstream, "pst_fltr=%hhd\n", &par_phd->pst_fltr);
    fscanf(fstream, "powfrq_fltr=%hhd\n", &par_phd->powfrq_fltr);
    fscanf(fstream, "iover=%hhd.%hhd.%hhd\n", &par_phd->iover[0], &par_phd->iover[1], &par_phd->iover[2]);
    fscanf(fstream, "ver_time=%d\n", &par_phd->ver_time);
    fscanf(fstream, "audtlog_sz=%hd\n", &par_phd->audtlog_sz);
    fscanf(fstream, "ver_61850=%s\n", par_phd->ver_61850);
    fscanf(fstream, "pr_dly_cnt=%hd\n", &par_phd->pr_dly_cnt);
    fscanf(fstream, "b_dbg_mode=%hhd\n", &par_phd->b_dbg_mode);
    fscanf(fstream, "rvc_wav_ena=%hhd\n", &par_phd->rvc_wav_ena);
    fscanf(fstream, "dc_val_type=%hhd\n", &par_phd->dc_val_type);
    fscanf(fstream, "st_tm_alg=%hhd\n", &par_phd->st_tm_alg);

    fscanf(fstream, "schdl_recwv[0]=%hhd,%hhd,%d\n", &par_phd->schdl_recwv[0].ena_ld, &par_phd->schdl_recwv[0].repeat, &par_phd->schdl_recwv[0].time);
    fscanf(fstream, "schdl_recwv[1]=%hhd,%hhd,%d\n", &par_phd->schdl_recwv[1].ena_ld, &par_phd->schdl_recwv[1].repeat, &par_phd->schdl_recwv[1].time);
    fscanf(fstream, "schdl_recwv[2]=%hhd,%hhd,%d\n", &par_phd->schdl_recwv[2].ena_ld, &par_phd->schdl_recwv[2].repeat, &par_phd->schdl_recwv[2].time);
    fscanf(fstream, "schdl_recwv[3]=%hhd,%hhd,%d\n", &par_phd->schdl_recwv[3].ena_ld, &par_phd->schdl_recwv[3].repeat, &par_phd->schdl_recwv[3].time);

    fscanf(fstream, "cvt_prl_res=%hd\n", &par_phd->cvt_prl_res);
    fscanf(fstream, "cvt_dbg=%hhd\n", &par_phd->cvt_dbg);
    fscanf(fstream, "eth_speed=%hhd\n", &par_phd->eth_speed);
    fscanf(fstream, "debug=%hd\n", &par_phd->debug);
    fscanf(fstream, "hrm_ns=%hhd\n", &par_phd->hrm_ns);

    fscanf(fstream, "ne_par.ena=%hhd\n", &par_phd->ne_par.ena);
    fscanf(fstream, "ne_par.num=%hhd,%hhd\n", &par_phd->ne_par.num[0], &par_phd->ne_par.num[1]);
    fscanf(fstream, "ne_par.spc=%hhd,%hhd,%hhd,%hhd,%hhd\n", &par_phd->ne_par.spc[0], &par_phd->ne_par.spc[1], &par_phd->ne_par.spc[2], &par_phd->ne_par.spc[3], &par_phd->ne_par.spc[4]);

    int n;
    for (int i=0; i<8; i++) {
        fscanf(fstream, "scnet_par[%d]=%hhx:%hhx:%hhx;%hhd;%hhd;%hhx:%hhx:%hhx;%hhd\n", &n, 
                &par_phd->scnet_par[i].mac[0], &par_phd->scnet_par[i].mac[1], &par_phd->scnet_par[i].mac[2],
                &par_phd->scnet_par[i].svtyp, &par_phd->scnet_par[i].ouit, 
                &par_phd->scnet_par[i].oui[0],  &par_phd->scnet_par[i].oui[1], &par_phd->scnet_par[i].oui[2],
                &par_phd->scnet_par[i].model);
    }
    fclose(fstream);
}

/*!
Load SG4LD from file

    Input:  ldx -- LD index. 0~
    Output: par_ld -- 
*/
void LoadSG4LD(SG4LD *par_ld, uint8_t ldx)
{
    char fname[64];
    sprintf(fname, "sg4ld%d.txt", ldx+1);
    FILE *fstream = fopen(fname, "r");
    if (fstream == NULL) {
        printf("Can't open file %s\n", fname);
        return;
    }
    fscanf(fstream, "version=%hd\n", &par_ld->version);
    fscanf(fstream, "ulevel=%hhd\n", &par_ld->ulevel);
    fscanf(fstream, "connect_type=%hhd\n", &par_ld->connect_type);
    fscanf(fstream, "pt=%d,%d\n", &par_ld->pt[0], &par_ld->pt[1]);
    fscanf(fstream, "ct=%d,%d\n", &par_ld->ct[0], &par_ld->ct[1]);   
    fscanf(fstream, "vdev_lmt=%hd,%hd\n", &par_ld->vdev_lmt[0], &par_ld->vdev_lmt[1]);
    fscanf(fstream, "unblnc_lmt=%hd\n", &par_ld->unblnc_lmt);
    fscanf(fstream, "negcmp_lmt=%hd\n", &par_ld->negcmp_lmt);
    fscanf(fstream, "freq_lmt=%hd,%hd\n", &par_ld->freq_lmt[0], &par_ld->freq_lmt[1]);
    fscanf(fstream, "pst_lmt=%hd\n", &par_ld->pst_lmt);
    fscanf(fstream, "plt_lmt=%hd\n", &par_ld->plt_lmt);
    fscanf(fstream, "uhrm_lmt=%hd", &par_ld->uhrm_lmt[0]);
    for (int i=1; i<50; i++) fscanf(fstream, ",%hd", &par_ld->uhrm_lmt[i]);
    fgets(fname, 64, fstream);
    fscanf(fstream, "ihrm_lmt=%hd", &par_ld->ihrm_lmt[0]);
    for (int i=1; i<50; i++) fscanf(fstream, ",%hd", &par_ld->ihrm_lmt[i]);
    fgets(fname, 64, fstream);
    fscanf(fstream, "ulmt_type=%hhd\n", &par_ld->ulmt_type);
    fscanf(fstream, "fluct_en=%hhd\n", &par_ld->fluct_en);
    fscanf(fstream, "fluct_db=%hd\n", &par_ld->fluct_db);

    fscanf(fstream, "capacity=%g,%g,%g\n", &par_ld->capacity[0], &par_ld->capacity[1], &par_ld->capacity[2]);
    
    fscanf(fstream, "evnt_en=%hhd\n", &par_ld->evnt_en);
    fscanf(fstream, "rvc_en=%hhd\n", &par_ld->rvc_en);
    fscanf(fstream, "evnt_limit=%hd,%hd,%hd\n", &par_ld->evnt_limit[0], &par_ld->evnt_limit[1], &par_ld->evnt_limit[2]);
    fscanf(fstream, "rvc_lmt=%hd\n", &par_ld->rvc_lmt);
    
    fscanf(fstream, "dc_topo=%hhd\n", &par_ld->dc_topo);
    fscanf(fstream, "ac_dc=%hhd\n", &par_ld->ac_dc);
    fscanf(fstream, "evnt_rate_lmt=%hd,%hd\n", &par_ld->evnt_rate_lmt[0], &par_ld->evnt_rate_lmt[1]);
    fscanf(fstream, "evnt_rate_en=%hhd\n", &par_ld->evnt_rate_en);
    fscanf(fstream, "attntr=%hhd\n", &par_ld->attntr);
    
    fscanf(fstream, "ievnt_limit=%hd\n", &par_ld->ievnt_limit);
    fscanf(fstream, "ievnt_rate_lmt=%hd\n", &par_ld->ievnt_rate_lmt);
    fscanf(fstream, "ievnt_en=%hhd\n", &par_ld->ievnt_en);
    fscanf(fstream, "ievnt_rate_en=%hhd\n", &par_ld->ievnt_rate_en);
    fscanf(fstream, "ilmt_type=%hhd\n", &par_ld->ilmt_type);

    fclose(fstream);
}

/*!
Save ParamPhD to file

    Input:  par_phd -- 
*/
void SaveParamPhD(ParamPhD *par_phd)
{
    char *fname = "paramphd.txt";
    FILE *fstream = fopen(fname, "w");
    if (fstream == NULL) {
        printf("Can't open file %s\n", fname);
        return;
    }
    fprintf(fstream, "device_model=%d\n", par_phd->device_model);
    fprintf(fstream, "device_id=%s\n", par_phd->device_id);
    fprintf(fstream, "stts_spc=");
    for (int i=0; i<kPQParaEnd; i++) fprintf(fstream, "%d,", par_phd->stts_spc[i]);
    fprintf(fstream, "\n");
    fprintf(fstream, "ip[0]=%d.%d.%d.%d\n", par_phd->ip[0][0], par_phd->ip[0][1], par_phd->ip[0][2], par_phd->ip[0][3]);
    fprintf(fstream, "ip[1]=%d.%d.%d.%d\n", par_phd->ip[1][0], par_phd->ip[1][1], par_phd->ip[1][2], par_phd->ip[1][3]);
    fprintf(fstream, "port=%d,%d\n", par_phd->port[0], par_phd->port[1]);
    fprintf(fstream, "mask[0]=%d.%d.%d.%d\n", par_phd->mask[0][0], par_phd->mask[0][1], par_phd->mask[0][2], par_phd->mask[0][3]);
    fprintf(fstream, "mask[1]=%d.%d.%d.%d\n", par_phd->mask[1][0], par_phd->mask[1][1], par_phd->mask[1][2], par_phd->mask[1][3]);
    fprintf(fstream, "mac[0]=%02x:%02x:%02x:%02x\n", par_phd->mac[0][0], par_phd->mac[0][1], par_phd->mac[0][2], par_phd->mac[0][3]);
    fprintf(fstream, "mac[1]=%02x:%02x:%02x:%02x\n", par_phd->mac[1][0], par_phd->mac[1][1], par_phd->mac[1][2], par_phd->mac[1][3]);
    fprintf(fstream, "gate[0]=%d.%d.%d.%d\n", par_phd->gate[0][0], par_phd->gate[0][1], par_phd->gate[0][2], par_phd->gate[0][3]);
    fprintf(fstream, "gate[1]=%d.%d.%d.%d\n", par_phd->gate[1][0], par_phd->gate[1][1], par_phd->gate[1][2], par_phd->gate[1][3]);
    fprintf(fstream, "frqm_spc=%d\n", par_phd->frqm_spc);
    fprintf(fstream, "hrm_type=%d\n", par_phd->hrm_type);
    fprintf(fstream, "ihrm_type=%d\n", par_phd->ihrm_type);
    fprintf(fstream, "man_rec_en=%d\n", par_phd->man_rec_en);
    fprintf(fstream, "evnt_tol=%d\n", par_phd->evnt_tol);
    fprintf(fstream, "cmtrd_sv_meth=%d\n", par_phd->cmtrd_sv_meth);
    fprintf(fstream, "stdy_trgr_ena=0x%02X\n", par_phd->stdy_trgr_ena);
    fprintf(fstream, "synctm_type=%d\n", par_phd->synctm_type);
    fprintf(fstream, "evnt_max_dur=%d\n", par_phd->evnt_max_dur);
    fprintf(fstream, "evnt_end_num=%d\n", par_phd->evnt_end_num);
    fprintf(fstream, "sys_zone=%d\n", par_phd->sys_zone);
    fprintf(fstream, "gps_zone_t=%d\n", par_phd->gps_zone_t);
    fprintf(fstream, "ntp_ip=%d.%d.%d.%d\n", par_phd->ntp_ip[0], par_phd->ntp_ip[1], par_phd->ntp_ip[2], par_phd->ntp_ip[3]);
    fprintf(fstream, "commu_num=%d,%d\n", par_phd->commu_num[0], par_phd->commu_num[1]);
    fprintf(fstream, "baud=%d,%d\n", par_phd->baud[0], par_phd->baud[1]);
    fprintf(fstream, "prtcl=%d,%d;%d,%d\n", par_phd->prtcl[0][0], par_phd->prtcl[0][1], par_phd->prtcl[1][0], par_phd->prtcl[1][1]);
    fprintf(fstream, "alarm_ena=0x%02X\n", par_phd->alarm_ena);
    fprintf(fstream, "cvt_ena=%d\n", par_phd->cvt_ena);
    fprintf(fstream, "cvt_cc=%d\n", par_phd->cvt_cc);
    fprintf(fstream, "cvt_chnl=%d,%d,%d\n", par_phd->cvt_chnl[0], par_phd->cvt_chnl[1], par_phd->cvt_chnl[2]);
    fprintf(fstream, "cvt_cnct=%d,%d;%d,%d;%d,%d\n", par_phd->cvt_cnct[0][0], par_phd->cvt_cnct[0][1], par_phd->cvt_cnct[1][0], par_phd->cvt_cnct[1][1], par_phd->cvt_cnct[2][0], par_phd->cvt_cnct[2][1]);
    fprintf(fstream, "cvt_c1c2=%g,%g;%g,%g;%g,%g\n", par_phd->cvt_c1c2[0][0], par_phd->cvt_c1c2[0][1], par_phd->cvt_c1c2[1][0], par_phd->cvt_c1c2[1][1], par_phd->cvt_c1c2[2][0], par_phd->cvt_c1c2[2][1]);
    fprintf(fstream, "intrplt_t=%d,%d\n", par_phd->intrplt_t[0], par_phd->intrplt_t[1]);
    fprintf(fstream, "softver=%d.%d.%d\n", par_phd->softver[0], par_phd->softver[1], par_phd->softver[2]);
    fprintf(fstream, "cyc10_ena=%d\n", par_phd->cyc10_ena);
    fprintf(fstream, "cyc10_sz=%d\n", par_phd->cyc10_sz);
    fprintf(fstream, "cyc150_ena=%d\n", par_phd->cyc150_ena);
    fprintf(fstream, "cyc150_sz=%d\n", par_phd->cyc150_sz);
    fprintf(fstream, "sig_simu=%d\n", par_phd->sig_simu);
    fprintf(fstream, "input0_thr=%d,%d\n", par_phd->input0_thr[0], par_phd->input0_thr[1]);
    fprintf(fstream, "cvt_r1r2=%g,%g;%g,%g;%g,%g\n", par_phd->cvt_r1r2[0][0], par_phd->cvt_r1r2[0][1], par_phd->cvt_r1r2[1][0], par_phd->cvt_r1r2[1][1], par_phd->cvt_r1r2[2][0], par_phd->cvt_r1r2[2][1]);
    fprintf(fstream, "time_diff=%d\n", par_phd->time_diff);
    fprintf(fstream, "security_ena=%d\n", par_phd->security_ena);
    fprintf(fstream, "pst_fltr=%d\n", par_phd->pst_fltr);
    fprintf(fstream, "powfrq_fltr=%d\n", par_phd->powfrq_fltr);
    fprintf(fstream, "iover=%d.%d.%d\n", par_phd->iover[0], par_phd->iover[1], par_phd->iover[2]);
    fprintf(fstream, "ver_time=%d\n", par_phd->ver_time);
    fprintf(fstream, "audtlog_sz=%d\n", par_phd->audtlog_sz);
    fprintf(fstream, "ver_61850=%s\n", par_phd->ver_61850);
    fprintf(fstream, "pr_dly_cnt=%d\n", par_phd->pr_dly_cnt);
    fprintf(fstream, "b_dbg_mode=%d\n", par_phd->b_dbg_mode);
    fprintf(fstream, "rvc_wav_ena=%d\n", par_phd->rvc_wav_ena);
    fprintf(fstream, "dc_val_type=%d\n", par_phd->dc_val_type);
    fprintf(fstream, "st_tm_alg=%d\n", par_phd->st_tm_alg);

    fprintf(fstream, "schdl_recwv[0]=%d,%d,%d\n", par_phd->schdl_recwv[0].ena_ld, par_phd->schdl_recwv[0].repeat, par_phd->schdl_recwv[0].time);
    fprintf(fstream, "schdl_recwv[1]=%d,%d,%d\n", par_phd->schdl_recwv[1].ena_ld, par_phd->schdl_recwv[1].repeat, par_phd->schdl_recwv[1].time);
    fprintf(fstream, "schdl_recwv[2]=%d,%d,%d\n", par_phd->schdl_recwv[2].ena_ld, par_phd->schdl_recwv[2].repeat, par_phd->schdl_recwv[2].time);
    fprintf(fstream, "schdl_recwv[3]=%d,%d,%d\n", par_phd->schdl_recwv[3].ena_ld, par_phd->schdl_recwv[3].repeat, par_phd->schdl_recwv[3].time);

    fprintf(fstream, "cvt_prl_res=%d\n", par_phd->cvt_prl_res);
    fprintf(fstream, "cvt_dbg=%d\n", par_phd->cvt_dbg);
    fprintf(fstream, "eth_speed=%d\n", par_phd->eth_speed);
    fprintf(fstream, "debug=%d\n", par_phd->debug);
    fprintf(fstream, "hrm_ns=%d\n", par_phd->hrm_ns);

    fprintf(fstream, "ne_par.ena=%d\n", par_phd->ne_par.ena);
    fprintf(fstream, "ne_par.num=%d,%d\n", par_phd->ne_par.num[0], par_phd->ne_par.num[1]);
    fprintf(fstream, "ne_par.spc=%d,%d,%d,%d,%d\n", par_phd->ne_par.spc[0], par_phd->ne_par.spc[1], par_phd->ne_par.spc[2], par_phd->ne_par.spc[3], par_phd->ne_par.spc[4]);

    for (int i=0; i<8; i++) {
        fprintf(fstream, "scnet_par[%d]=%02x:%02x:%02x;%d;%d;%02x:%02x:%02x;%d\n", i, 
                par_phd->scnet_par[i].mac[0], par_phd->scnet_par[i].mac[1], par_phd->scnet_par[i].mac[2],
                par_phd->scnet_par[i].svtyp, par_phd->scnet_par[i].ouit, 
                par_phd->scnet_par[i].oui[0],  par_phd->scnet_par[i].oui[1], par_phd->scnet_par[i].oui[2],
                par_phd->scnet_par[i].model);
    }
    fclose(fstream);
}

/*!
Save SG4LD to file

    Input:  par_ld -- 
            ldx -- LD index. 0~
*/
void SaveSG4LD(SG4LD *par_ld, uint8_t ldx)
{
    char fname[64];
    sprintf(fname, "sg4ld%d.txt", ldx+1);
    FILE *fstream = fopen(fname, "w");
    if (fstream == NULL) {
        printf("Can't open file %s\n", fname);
        return;
    }
    fprintf(fstream, "version=%d\n", par_ld->version);
    fprintf(fstream, "ulevel=%d\n", par_ld->ulevel);
    fprintf(fstream, "connect_type=%d\n", par_ld->connect_type);
    fprintf(fstream, "pt=%d,%d\n", par_ld->pt[0], par_ld->pt[1]);
    fprintf(fstream, "ct=%d,%d\n", par_ld->ct[0], par_ld->ct[1]);
    fprintf(fstream, "vdev_lmt=%d,%d\n", par_ld->vdev_lmt[0], par_ld->vdev_lmt[1]);
    fprintf(fstream, "unblnc_lmt=%d\n", par_ld->unblnc_lmt);
    fprintf(fstream, "negcmp_lmt=%d\n", par_ld->negcmp_lmt);
    fprintf(fstream, "freq_lmt=%d,%d\n", par_ld->freq_lmt[0], par_ld->freq_lmt[1]);
    fprintf(fstream, "pst_lmt=%d\n", par_ld->pst_lmt);
    fprintf(fstream, "plt_lmt=%d\n", par_ld->plt_lmt);
    fprintf(fstream, "uhrm_lmt=");
    for (int i=0; i<49; i++) fprintf(fstream, "%d,", par_ld->uhrm_lmt[i]);
    fprintf(fstream, "%d\n", par_ld->uhrm_lmt[49]);
    fprintf(fstream, "ihrm_lmt=");
    for (int i=0; i<50; i++) fprintf(fstream, "%d,", par_ld->ihrm_lmt[i]);
    fprintf(fstream, "%d\n", par_ld->ihrm_lmt[49]);
    fprintf(fstream, "ulmt_type=%d\n", par_ld->ulmt_type);
    fprintf(fstream, "fluct_en=%d\n", par_ld->fluct_en);
    fprintf(fstream, "fluct_db=%d\n", par_ld->fluct_db);
    
    fprintf(fstream, "capacity=%g,%g,%g\n", par_ld->capacity[0], par_ld->capacity[1], par_ld->capacity[2]);
    
    fprintf(fstream, "evnt_en=%d\n", par_ld->evnt_en);
    fprintf(fstream, "rvc_en=%d\n", par_ld->rvc_en);
    fprintf(fstream, "evnt_limit=%d,%d,%d\n", par_ld->evnt_limit[0], par_ld->evnt_limit[1], par_ld->evnt_limit[2]);
    fprintf(fstream, "rvc_lmt=%d\n", par_ld->rvc_lmt);
    
    fprintf(fstream, "dc_topo=%d\n", par_ld->dc_topo);
    fprintf(fstream, "ac_dc=%d\n", par_ld->ac_dc);
    fprintf(fstream, "evnt_rate_lmt=%d,%d\n", par_ld->evnt_rate_lmt[0], par_ld->evnt_rate_lmt[1]);
    fprintf(fstream, "evnt_rate_en=%d\n", par_ld->evnt_rate_en);
    fprintf(fstream, "attntr=%d\n", par_ld->attntr);
    
    fprintf(fstream, "ievnt_limit=%d\n", par_ld->ievnt_limit);
    fprintf(fstream, "ievnt_rate_lmt=%d\n", par_ld->ievnt_rate_lmt);
    fprintf(fstream, "ievnt_en=%d\n", par_ld->ievnt_en);
    fprintf(fstream, "ievnt_rate_en=%d\n", par_ld->ievnt_rate_en);
    fprintf(fstream, "ilmt_type=%d\n", par_ld->ilmt_type);

    fclose(fstream);
}

/*!
Select Setting Group

    Input:  dev -- 0=ParamPhD, 1=SG4LD
            sgt -- SG type. 0=active, 1=edit
            sgn -- SettingGroupNumber. 1~
            ldx -- LD index. 0=LD1,1=LD2..
            sav -- 0=not save, 1=save
*/
const char* kDevName[] = {"ParamPhD", "SG4LD"};
const char* kSGTypeName[] = {"active", "edit"};
void SelectSG(uint8_t dev, uint8_t sgt, uint8_t sgn, uint8_t ldx, uint8_t sav)
{
    uint8_t reqst[4];
    reqst[0] = dev;
    reqst[1] = sgn;
    reqst[2] = ldx;
    uint8_t resp[1024];
    if (sgt) {  //edit
        shm_SendCmd(pshmem_, kSelectEditSG, reqst, 3, resp);
    } else {    //active
        shm_SendCmd(pshmem_, kSelectActSG, reqst, 3, resp);
    }
    printf("### Select %s %s SG %d.\n", kDevName[dev], kSGTypeName[sgt], sgn);
    if (!sav) return;

    if (dev) { //SG4LD
        SaveSG4LD((SG4LD*)resp, ldx);
    } else {    //ParamPhD
        SaveParamPhD((ParamPhD*)resp);
    }
}

/*!
Set Setting Group

    Input:  dev -- 0=ParamPhD, 1=SG4LD
            ldx -- LD index. 0=LD1,1=LD2.. only for SG4LD
*/
void SetSG(uint8_t dev, uint8_t ldx)
{
    uint8_t reqst[1024];

    printf("&&& Set %s SG.", kDevName[dev]);
    int sz;
    if (dev) { //SG4LD
        printf(" LD%d\n", ldx+1);
        LoadSG4LD((SG4LD*)reqst, ldx);
        sz = sizeof(SG4LD);
    } else {    //ParamPhD
        printf("\n");
        LoadParamPhD((ParamPhD*)reqst);
        sz = sizeof(ParamPhD);
    }
    memmove(&reqst[2], reqst, sz);
    reqst[0] = dev;
    reqst[1] = ldx;
    shm_SendCmd(pshmem_, kConfEditSG, reqst, sz+2, reqst);
}

int main(int argc, char *argv[])
{
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
    
    int ret = 0;
    do {
        if (argc<2) break;
        ParseOptnShmem parse_opt;
        ret = parse_opt.Parse(argc, argv);
        if (ret<0) break;
        printf("shm_cmd=%d, dev=%d, ldx=%d, sgn=%d, sgt=%d\n", parse_opt.shm_cmd(), parse_opt.dev(), 
        parse_opt.ldx(), parse_opt.sgn(), parse_opt.sgt());
        uint8_t sgn;
        switch (parse_opt.shm_cmd()) {
            case 0: //read
                if (parse_opt.dev()) {  //LD
                    sgn = sgbuf.sg[2];
                } else {    //ParamPhD
                    sgn = sgbuf.sg[0];
                }
                SelectSG(parse_opt.dev(), 0, sgn, parse_opt.ldx(), 1);
                break;
            case 1: //set
                SetSG(parse_opt.dev(), parse_opt.ldx());
                break;
            case 2: //select
                SelectSG(parse_opt.dev(), parse_opt.sgt(), parse_opt.sgn(), 0, 0);
            default:
                break;
        }    
    } while(0);

    FreeShareMem(pshmem_);
    return ret;
}

