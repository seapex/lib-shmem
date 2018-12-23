#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <poll.h>

#include "config.h"
#include "share_mem.h"

union semun {
    int val;        // value for SETVAL
    struct semid_ds *buf;   // buffer for IPC_STAT, IPC_SET
    unsigned short *array;  // array for GETALL, SETALL
    struct seminfo *__buf;  // buffer for IPC_INFO
};

const int kSemTol = 16;   //total number of semaphore

/*!
Initialize share memory

    Input:  shm_sz -- share memory size in bytes
            filename, subid -- used to generate IPC key
    Output: semid -- semaphore set identifier
    Return: pointer to share memory
*/
void *InitShareMem(int *semid, int shm_sz, const char *filename, int subid)
{
    key_t key = ftok(filename, subid); //generate a System V IPC key
    if(key == -1) {
        perror("ftok error \n");
        return NULL;
    }
    if((shm_sz % 4) != 0)  shm_sz += (shm_sz%4);
    int shm_id = shmget(key, shm_sz, IPC_CREAT); //allocates a shared memory & return id of shared memory. if exist, only return id.
    printf("share_mem v%d.%d.%d key=%d size=%d\n", _VERSION_MAJOR, _VERSION_MINOR, _VERSION_PATCH, key, shm_sz);
    if(shm_id == -1) {
        perror("shmget error!\n");
        return NULL;
    }
    void *pshmem = shmat(shm_id, NULL, 0); //attaches the shared memory & return the segment's start address
    if (pshmem == NULL) perror("shmat error!\n");

    int sid = semget(key, 0, 0); //Get semaphores set.
    if (sid == -1) {
        printf("creat semaphore set\n");
        sid = semget(key, kSemTol, IPC_CREAT | IPC_EXCL | 0600); //Create n semaphores. n=kSemTol
        if (sid == -1)   {
            perror("create semget error");
            return NULL;
        }
        //Initialize semaphores
        unsigned short buf[kSemTol];
        for (int i=0; i < kSemTol; i++) buf[i] = 1;
        semun arg;
        arg.array = buf;
        int ret = semctl(sid, 0, SETALL, arg);
        if (ret < 0 )   {
            perror("ctl sem error");
            semctl(sid, 0, IPC_RMID);
            return NULL ;
        }
    }
    *semid = sid;
    return pshmem;
}

void FreeShareMem(void *pshmem)
{
    if(shmdt(pshmem) == -1) {
        perror("detach error");
    }
}

/*!
对信号量数组semnum编号的信号量做P操作 
*/
int Poperate(int semid, int semnum)
{
    sembuf sops = {semnum, -1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}

/*!
对信号量数组semnum编号的信号量做V操作
*/
int Voperate(int semid, int semnum)
{
    struct sembuf sops = {semnum, +1, SEM_UNDO};
    return (semop(semid, &sops, 1));
}

