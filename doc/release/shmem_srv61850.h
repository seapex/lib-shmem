/*! \file shmem_srv61850.h
    \brief Share memory for 61850 server.
    Copyright (c) 2018  Xi'an Boyuu Electric, Inc.
*/
#ifndef _SHMEM_SRV61850_H_
#define _SHMEM_SRV61850_H_

#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdint.h>
#include "param_phd.h"

#ifdef __cplusplus
extern "C" {
#endif

//共享内存接口命令
typedef enum {
    kNoneCmd = 0, kInitSGCB, kSelectActSG, kSelectEditSG,
    kConfEditSG, kManRcdWave, kColdReset, kProgUpdate,
    kPqmnVer, kLogin61850, kIdentify61850,
} shm_APICmd;

typedef enum {
    kOverFreq, kUnderFreq, kPosVoltdv, kNegVoltdv, kAlmPst, kAlmPlt, kAlmFulct,
    kAlmUnblc, kAlmTHDu, kAlmHRU, kAlmHRI=58, 
    kAlmRmsVUplimt=107, kAlmRmsVLolimt, //暂态电压 高、低门限
    kAlmRmsAUplimt, kAlmRmsALolimt,     //暂态电流 高、低门限
    kAlmRmsVDiflimt, kAlmRmsADiflimt,   //暂态电压、电流突变门限
    kAlmTHDi,
    kAlmOddHRU, KAlmEvnHRU,     //奇次、偶次谐波含有率
    kAlmVLdintlimt,             //长期电压中断
    kAlmIUnblc,                 //电流不平衡
    kAlmINegseq,                 //负序电流
    kAlmTpEnd
} AlmType;

typedef enum {
    kSemNmVVStr, kSemNmVxStr,
    kSemNmVVEnd, kSemNmCommand=5
} SemaphoreNumber;  //信号量编号

typedef enum {
    kDRcdMade, kBrcbUpdate, kUrcbUpdate
} SafeSetDataID;

typedef struct {
  	uint16_t version;       //The version of definition of this struct
	uint8_t ulevel;         //voltage level. 0=380V, 1=6kV, 2=10kV, 3=35kV, 4=110kV, 5=220kV, 6=330kV, 7=500kV
	uint8_t connect_type;   //voltage connect type. 0=wye, 1=delta
    uint32_t pt[2];  //Potential Transformer. [0-1]:primary,secondary. unit:V
    uint32_t ct[2];  //Current Transformer.   [0-1]:primary,secondary. unit:A
	uint16_t vdev_lmt[2];   //voltage deviation limit. uint:1/1000. [0-1]:positive,negative
	uint16_t unblnc_lmt;    //voltage negtive unbalance limit. unit:1/1000. 
	uint16_t negcmp_lmt;    //current negtive component limit. unit:0.1A. 
	uint16_t freq_lmt[2];   //frequency limit. unit:0.001Hz. [0-1]:positive,negative
	uint16_t pst_lmt;       //Pst limit. unit:0.01
	uint16_t plt_lmt;       //Plt limit. unit:0.01
	uint16_t uhrm_lmt[50];  //voltage harmonic limit. [0]:THDu; [1-49]:2~50 HRu limit. unit:1/1000. 
	uint16_t ihrm_lmt[50];  //current harmonic limit. [0]:reserve; [1-49]:2~50 amplitude limit. unit:1/10A.
 	uint8_t ulmt_type;      //voltage harmonic limit type. 0=GB, 1=customization
	uint8_t fluct_en;       //Fluctuation measurement enable. bit0-2:A-C
	uint16_t fluct_db;      //Fluctuation deadband, unit:0.001/%
	
    float capacity[3];      //[0-2]:minimum short circuit, equipment supply, user agreement. unit:MVA
	
    uint8_t evnt_en;        //transient event monitor enable. bit0-2:A-C
	uint8_t rvc_en;         //rvc event monitor enable. bit0-2:A-C
    uint16_t evnt_limit[3]; //limit of transient event. [0-2]:swell,dip,interrupt. unit:1/1000.
	uint16_t rvc_lmt;       //limit of rvc. unit:1/1000.

    uint8_t dc_topo;        //DC topology. 0=bipolar, 1=unipolar, 2=pseudo-bipolar
    uint8_t ac_dc;          //0=AC, 1=DC
	uint16_t evnt_rate_lmt[2];  //limit of rate of change. [0-1]:high,low. unit:1/1000.
	uint8_t evnt_rate_en;   //rate of change event monitor enable. bit0-2:A-C
    uint8_t attntr;         //Signal attenuator, only for voltage channel. 0=none, 1=2X, 2=3X.
    
	uint16_t ievnt_limit;   //limit of current transient event. unit:1/1000.
	uint16_t ievnt_rate_lmt;    //limit of rate of current change. unit:1/1000.
	uint8_t ievnt_en;       //rate of change event monitor enable. bit0-2:A-C
	uint8_t ievnt_rate_en;  //rate of current change event monitor enable. bit0-2:A-C
 	uint8_t ilmt_type;      //current harmonic limit type. 0=GB, 1=customization
	char reserve[107];
} SG4LD;

typedef struct {
    float xStart;
    float xEnd;
    float yStart;
    float yEnd;
} CellType;

typedef struct {
    char Alm_stVal[kAlmTpEnd];    //GGIO.Almxx.stVal . 0=false, 1=true
    time_t Alm_t[kAlmTpEnd];      //GGIO.Almxx.t .
} PqmGgio;

typedef struct {
    time_t on_time;     //LPHD$PwrUp$t
    char pwr_on;        //LPHD$PwrUp$stVal
    time_t off_time;    //LPHD$PwrDn$t
    char pwr_off;       //LPHD$PwrDn$stVal
} PqmLphd;

typedef struct {
    time_t st_time;
    time_t lt_time;
    float phpst[3];     //MFLK$MX$PhPst
    float pppst[3];     //MFLK$MX$PPPst
    float phplt[3];     //MFLK$MX$PhPlt
    float ppplt[3];     //MFLK$MX$PPPlt
    float phfluc[3];    //MFLK$MX$PhFluc
    float ppfluc[3];    //MFLK$MX$PPFluc
    float phflucf[3];   //MFLK$MX$PhFlucf
    float ppflucf[3];   //MFLK$MX$PPFlucf
    uint16_t q[3];//quality,0-2=pst,plt,fluctuation
} PqmMflk;

typedef struct {
    time_t time;
    //HA
    float ha[3][51];    //harmonic current rms.[0-2]:A-C, [0-50]:0-50. unit:A
    float haang[3][51]; //harmonic current angle. [0-2]:A-C, [1-50]:1-50. unit:degree
    //HPhV,HPPV
    float hphv[3][51];  //harmonic voltage rms for phase to neutral. [0-2]:A-C, [0-50]:0-50. unit:V
    float hppv[3][51];  //harmonic voltage rms for phase to phase. [0-2]:AB-CA, [0-50]:0-50. unit:V
    float hvang[3][51]; //harmonic voltage angle. [0-2]:A-C, [1-50]:1-50. real only. unit:degree
    //HR
    float hrphv[3][51]; //harmonic ratio for phase to neutral. [0-2]:A-C, [2-50]:2-50. unit:%
    float hrppv[3][51]; //harmonic ratio for phase to phase. [0-2]:AB-CA, [2-50]:2-50. unit:%
    //THD
    float thda[3];      //current THD. [0-2]:A-C. unit:%
    float thdodda[3];   //current odd THD. [0-2]:A-C. unit:%
    float thdevna[3];   //current even THD. [0-2]:A-C. unit:%
    float thdphv[3];    //voltage THD for phase to neutral. [0-2]:A-C. unit:%
    float thdoddphv[3]; //voltage odd THD for phase to neutral. [0-2]:A-C. unit:%
    float thdevnphv[3]; //voltage even THD for phase to neutral. [0-2]:A-C. unit:%
    float thdppv[3];    //voltage THD for phase to phase. [0-2]:AB-CA. unit:%
    float thdoddppv[3]; //voltage odd THD for phase to phase. [0-2]:AB-CA. unit:%
    float thdevnppv[3]; //voltage even THD for phase to phase. [0-2]:AB-CA. unit:%
    //Harmonic power
    float hw[4][51];    //[0-3]:A-C,all [0-50]:0-50. unit:W
    float hvar[4][51];  //[0-3]:A-C,all [0-50]:0-50. unit:var
    float hva[4][51];   //[0-3]:A-C,all [0-50]:0-50. unit:VA
    float hpf[4][51];   //[0-3]:A-C,all [0-50]:0-50.
    //Total harmonic power    
    float tothw[4];     //active power. [0-3]A-C,all. unit:W
    float tothvar[4];   //reactive power. [0-3]A-C,all. unit:var
    float tothva[4];    //apparent power. [0-3]A-C,all. unit:VA
    float tothpf[4];    //power factor. [0-3]A-C,all
    uint16_t q;
} PqmMhai;

typedef struct {
    time_t time;
    float ha[3][51];    //interharmonic current rms.[0-2]:A-C, [0-50]:0-50. unit:A
    float hphv[3][51];  //interharmonic voltage rms for phase to neutral. [0-2]:A-C, [0-50]:0-50. unit:V
    float hppv[3][51];  //interharmonic voltage rms for phase to phase. [0-2]:A-C, [0-50]:0-50. unit:V
    float hrphv[3][51]; //interharmonic voltage HR for phase to neutral.[0-2]:A-C, [0-50]:0-50. unit:%
    float hrppv[3][51]; //interharmonic voltage HR for phase to phase.[0-2]:AB-CA, [0-50]:0-50. unit:%
    uint16_t q;
} PqmMhaiIntr;

typedef struct {
    time_t time;        
    float watt;     //Power. unit:kw
    float amp;      //DC current rms. unit:A
    float vol;      //DC voltage rms. unit:V
    float vdev;     //voltage deviation. unit:%.
    float ampavg;   //DC current average. unit:A
    float volavg;   //DC voltage average. unit:V
    uint16_t q;     //Quality
} PqmMmdc;

typedef struct {
    time_t time;
    time_t hz_time;
    float hz;           //unit:Hz
    float hzdev;        //Frequency deviation. unit:Hz
    float a_mag[3];     //current rms. [0-2]:A-C. unit:A
    float phv_mag[3];   //voltage rms for phase to neutral. [0-2]:A-C. unit:V
    float ppv_mag[3];   //voltage rms for phase to phase. [0-2]:AB-CA. unit:V
    float phvdev[3];    //voltage deviation for phase to neutral. [0-2]:A-C. unit:%
    float ppvdev[3];    //voltage deviation for phase to phase. [0-2]:AB-CA. unit:%
    //power
    float w[4];         //A-C,all. unit:kW
    float var[4];       //A-C,all. unit:kvar
    float va[4];        //A-C,all. unit:kVA
    float pf[4];        //A-C,all
    uint16_t hz_q;      //Quality of frequency
    uint16_t q;         //Quality
} PqmMmxu;

typedef struct {
    time_t time;
    float para[3][2];   //[0-2]:+LM,-LM,+L-L; [0-1]:coefficient,ratio. unit:%    
    uint16_t q;
} PqmMrpl;

typedef struct {
    time_t time;    //MSQI$xxx$t
    float seqa[3];  //MSQI$SeqA$cx$cVal$mag$f. zero-pos-neg, unit:A
    float imbnga;   //MSQI$ImbNgA$mag$f. unit:%
    float imbzroa;  //MSQI$ImbZroA$mag$f. unit:%
    float seqv[3];  //MSQI$SeqV$cx$cVal$mag$f. zero-pos-neg, unit:V. U_b-U_u-ε; unit:V,V,%; for DC
    float imbngv;   //MSQI$ImbNgV$mag$f. unit:%
    float imbzrov;  //MSQI$ImbZroV$mag$f. unit:%
    uint16_t q;
} PqmMsqi;

typedef struct {
    char IVarStr_stVal;     //QITR.VarStr.stVal -- Current event start
    struct timeval IVarStr_time;   //QITR.VarStr.t -- Start time of the current event
    char IVarEnd_stVal;     //QITR.VarEnd.stVal -- Current event finished
    struct timeval IVarEnd_time;   //QITR.VarEnd.t -- Finish time of the current event
    int32_t MaxATrs;            //QITR.MaxATrs.mag -- Maximum current transient value,unit:uA
    int32_t ATrsTm;             //QITR.ATrsTm.mag -- Current transient duration, unit:ms
} PqmQitr;

typedef struct {
    char happen;        //event happened. set 1 by pqm3mn, clear 0 by 61850 svr
    char varstr;        //QRVC.VarStr.stVal -- RVC event start
    struct timeval str_time;   //QRVC.VarStr.t -- Start time of the RVC event
    char varend;        //QRVC.VarEnd.stVal -- RVC event finished
    struct timeval end_time;    //QRVC.VarEnd.t -- Finish time of the RVC event
    int32_t vva;        //QRVC.VVa.mag -- Umax, unit:10mV
    int32_t vvatm;      //QRVC.VVaTm.mag -- RVC duration, unit:ms
    int32_t vvadev;     //QRVC.MaxVVa.mag -- Uss, unit:10mV
    //Event counter histogram
    //int32_t rvtcnt[30];     //QVVR.EvtCnt.hstVal[..]
    //CellType range[30];     //QVVR.EvtCnt.hstRangeC[..]
} PqmQrvc;

typedef struct {
    char UVarStr_stVal;     //QVTR.VarStr.stVal -- Voltage event start
    struct timeval UVarStr_time;   //QVTR.VarStr.t -- Start time of the Voltage event
    char UVarEnd_stVal;     //QVTR.VarEnd.stVal -- Voltage event finished
    struct timeval UVarEnd_time;   //QVTR.VarEnd.t -- Finish time of the Voltage event
    int32_t MaxVTrs;            //QVTR.MaxVTrs.mag -- Maximum voltage transient value,unit:mV
    int32_t VTrsTm;             //QVTR.VTrsTm.mag -- Voltage transient duration, unit:ms
} PqmQvtr;

typedef struct {
    char happen;        //event happened. set 1 by pqm3mn, clear 0 by 61850 svr
    char varstr;        //QVVR.VarStr.stVal -- Voltage event start
    struct timeval str_time;   //QVVR.VarStr.t -- Start time of the Voltage event
    char dipstr;        //QVVR.DipStr.stVal -- Dip event start
    char swlstr;        //QVVR.SwlStr.stVal -- Swell event start
    char intrstr;       //QVVR.IntrStr.stVal -- Interruption event start
    char varend;        //QVVR.VarEnd.stVal -- Voltage event finished
    struct timeval end_time;   //QVVR.VarEnd.t -- Finish time of the Voltage event
    int32_t vva;            //QVVR.VVa.mag -- Maximum voltage transient value(primary),unit:10mV
    int32_t vvatm;          //QVVR.VVaTm.mag -- Voltage transient duration, unit:ms
    //Event counter histogram
    //int32_t rvtcnt[30];     //QVVR.EvtCnt.hstVal[..]
    //CellType range[30];     //QVVR.EvtCnt.hstRangeC[..]
} PqmQvvr;

typedef struct {
    char happen;        //event happened. set 1 by pqm3mn, clear 0 by 61850 svr
    struct timeval time;       //RDRE$RcdMade$t. Complete time
    char rcdmade;       //RDRE$RcdMade$stVal. TRUE = disturbance recording complete
    int32_t  fltnum;        //RDRE$FltNum$stVal. Fault number,此处用于区分不同录波文件的序号
    struct timeval trig_time;  //RDRE$RcdTrg$t. Trigger time
    char rcdtrg;        //RDRE$RcdTrg$stVal. Trigger recorder. External command to trigger recorder(TRUE)
} PqmRdre;

//共享内存数据区
struct Data4LD {
    PqmMmxu mmxu_real;
    PqmMsqi msqi_real;
    PqmMhai mhai_real;
    PqmMhaiIntr mhai_in_real;
    PqmMmdc mmdc_real[3];            //[0-2]:+LM,-LM,+L-L or total for watt.
    PqmMrpl mrpl_real;
    
    PqmMmxu mmxu_stat[4];           //[0-3]:maximum,average,minimum,CP95
    PqmMsqi msqi_stat[4];           //ditto
    PqmMhai mhai_stat[4];           //ditto
    PqmMhaiIntr mhai_in_stat[4];    //ditto
    PqmMrpl mrpl_stat[4];           //ditto
    PqmMmdc mmdc_stat[4][3];        //ditto. [0-2]:+LM,-LM,+L-L;
    
    PqmLphd lphd;
    PqmGgio ggio;
    PqmMflk mflk;
    PqmQitr qitr;
    PqmQrvc qrvc;
    PqmQvtr qvtr;
    PqmQvvr qvvr[4];    //[0-3]:A-C, Whole
    PqmRdre rdre;
    
    //char flicker_ok;
    //char lphd_ok;
    //char statistic_ok;
    char ggio_ok;   //set 1 by pqm3mn, clear 0 by 61850 svr
    char qvvr_ok;   //set 1 by pqm3mn, clear 0 by 61850 svr
    char rdre_ok;   //set 1 by pqm3mn, clear 0 by 61850 svr
    char qrvc_ok;   //set 1 by pqm3mn, clear 0 by 61850 svr
    char res1[4];   //reserve
    char MonitorPoint[68]; //monitor point name
    char LDName[16];
    char brcb_up;   //statistic data has update. increased by pqm3mn, decreased by 61850
    char urcb_up;   //realtime data has update. increased by pqm3mn, decreased by 61850
    uint16_t debug_var; //varialbes for debug
    char pst_up;   //Pst data has update. increased by dpqnet_mn, changed by dpqnet_mn only
    char plt_up;   //Plt data has update. increased by dpqnet_mn, changed by dpqnet_mn only
    char reserve[22];
};

//共享内存数据区
struct ShmemSvr61850 {
    Data4LD ld_data[kChannelTol];
    char StationName[68];  //substation name
    char IEDName[16];
    char ver_inf[24];   //61850 service version information.
    uint16_t quit_cmd;  //31729=quit
    uint8_t ld_stat;    //LDx state. bit0-kChannelTol:LD1-kChannelTol. 0=nil
    char reserve[61];
    
    shm_APICmd request_cmd; //Request command from 61850 Server
    shm_APICmd response_cmd;//Response command from pqm3mn
    int32_t resp_rec_num;   //Response command size in bytes
    uint8_t data[1024];
};

/*!
Initialize share memory
*/
ShmemSvr61850 *InitShmSvr61850();
 
/*!
Set share memory value under semaphore control

    Input:  p -- pointer to share memory
            dataid --
            value --
            idx -- logical device index
    Return: 0=success, <0=failure
*/
int32_t SafeSetShm(ShmemSvr61850 *p, SafeSetDataID dataid, const void *value, int32_t idx);

/*!
Send cmd to share memory in safe mode

    Input:  p -- pointer to share memory
            cmd -- command
            reqst -- request param
            size -- request param size
    Output: resp -- response data
    Return: number of return param. -1=failure
*/
int32_t shm_SendCmd(ShmemSvr61850 *p, shm_APICmd cmd, void *reqst, int32_t size, void *resp);
/* \brief Function shm_SendCmd(...) command list
cmd             reqst                           size            resp                                    return
-------------   ----------------------          ------          -----------------------------------     ------------------
kInitSGCB       NULL                            0               uint8_t[5]. [0]:total number of sg,     5
                                                                            [1]:LD0 active sg,
                                                                            [2]:LD0 edit sg,
                                                                            [3]:LD1-n active sg,
                                                                            [4]:LD1-n edit sg
kSelectActSG &  uint8_t[0]:0=ParamPhD,1=SG4LD   3               ParamPhD or SG4LD                       sizeof()
kSelectEditSG   uint8_t[1]:which sg                  
                uint8_t[2]:index of LD. 
                           0=LD1,1=LD2..
kConfEditSG     uint8_t[0]:0=ParamPhD,1=SG4LD   2+sizeof()      if (act_sg!=edit_sg) NULL;              0
                uint8_t[1]:index of LD.                         else ParamPhD or SG4LD;                 sizeof()
                           0=LD1,1=LD2..
                ParamPhD or SG4LD
kManRcdWave     uint8_t[0]:index of LD.         2               none                                    0
                           0=LD1,1=LD2..
                uint8_t[1]:1=start, 0=end
kPqmnVer        NULL                            0               char *ver_date.                         strlen(ver_date)+1
                                                                    e.g."0.4.20_20191211_0936"
kColdReset      NULL                            0               none                                    0
kLogin61850     uint32_t[4]                     16              0=passed, 1=refused                     1
kIdentify61850  uint32_t[4]                     16              0=passed, 1=refused                     1
kProgUpdate     NULL                            0               none                                    0
*/

#ifdef __cplusplus
}
#endif

#endif  //_SHMEM_SRV61850_H_ 

