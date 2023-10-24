/*! \file parse_option_shmem.cpp
    \brief Parse command line option.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse_optn_shmem.h"
//#include "config.h"

#define MAIN_PROG "tst_shmem"
const int kVersion[3] = {1, 0, 0};
const char *kCmdName[kCmdTypeEnd] = {
    MAIN_PROG };

//main command
static const char * main_sopts = "hV";
static const option main_lopts[] = {
    { "help",    0, 0, 'h' },
    { "version", 0, 0, 'V' },
    { "read",    2, 0, 'r' },
    { "set",     2, 0, 's' },
    { "select",  1, 0, 0x100+'s' },
    { NULL, 0, 0, 0 },
};

static const char * main_help =
    "Usage: "MAIN_PROG" [option] <dev> [type]\n"
    "The option are:\n"
    "       -h, --help      Print help information\n"
    "       -V, --version   main program version\n"
    "       --read=n <dev>  read shmem. n is LD index, 0=LD1,1=LD2...\n"
    "       --set=n <dev>   set shmem. n is LD index, 0=LD1,1=LD2...\n"
    "       --select n <dev> <type>   select sg, n=1,2,... \n"
    "\nThe dev is: phy or ld\n"
    "The type is: act or edt\n";

ParseOptnShmem::ParseOptnShmem()
{
    InitParam();

    dev_ = 0;
    ldx_ = 0;
    sgn_ = 1;
    sgt_ = 0;
}

void ParseOptnShmem::InitParam()
{
    cmd_tol_ = kCmdTypeEnd;
    cmd_name_ = kCmdName;
    
    short_opts_[kMainProg] = main_sopts;
    long_opts_[kMainProg] = main_lopts;
    help_info_[kMainProg] = main_help;
}

/*!
Handle main command

    Input:  opt -- option character 
            arg -- option argument
*/
int ParseOptnShmem::HandleMain(int opt, char *arg)
{
    switch (opt) {
        case 'V':
            printf(MAIN_PROG" version %d.%d.%d\n", kVersion[0], kVersion[1], kVersion[2]);
            return -1;
        case 'r':
            shm_cmd_ = 0;
            if (arg) ldx_ = atoi(arg);
            break;
        case 's':
            shm_cmd_ = 1;
            if (arg) ldx_ = atoi(arg);
            break;
        case 0x100+'s':
            shm_cmd_ = 2;
            sgn_ = atoi(arg);
            break;
        default:
            return PrintHelp();
    }
    return 0;
}

/*!
not -x or -xxx argument

    Input:  arg
            argc -- arg count
*/
int ParseOptnShmem::HandleOther(char *arg[], int argc)
{
    char stri[4];
    for (int i=0; i<argc; i++) {
        sscanf(arg[0], "%s", stri);
        switch (i) {
            case 0:
                if (strcmp(stri, "phy") == 0) dev_ = 0;
                else if (strcmp(stri, "ld") == 0) dev_ = 1;
                break;
            case 1:
                if (strcmp(stri, "act") == 0) sgt_ = 0;
                else if (strcmp(stri, "edt") == 0) sgt_ = 1;
        }
        arg++;
    }
    return 0;
}

/*!
Handle subcommand

    Input:  opt -- option character 
*/

int ParseOptnShmem::HandleSubcmd(int opt, char *arg)
{
    int ret;
    switch (cmd_) {
        default:
           ret = PrintHelp();
    }
    return ret;
}


