#ifndef __SYS_V_SEM__
#define __SYS_V_SEM__
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define SEM_RESOURCE_MAX 1
#define SEMMSL           1

#define SEM_LOCK {0, -1, SEM_UNDO}
#define SEM_UNLOCK {0, 1, SEM_UNDO}

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
};

static inline int
sem_init(int *semid, key_t key) {
    union semun semopts;
    if((*semid = semget(key, SEMMSL, IPC_CREAT|IPC_EXCL|0666)) == -1) {
        return -1;
    }
    semopts.val = SEM_RESOURCE_MAX;
    semctl(*semid, 0, SETVAL, semopts);
    return 0;
}

static inline int
sem_open(int *semid, key_t key) {
   if((*semid = semget(key, 0, 0666)) == -1)  {
       perror("Semaphore doesn't exist");
       return -1;
   }
   return 0;
}

static inline void
sem_wait(int *semid) {
    struct sembuf sem_lock = SEM_LOCK;
    semop(*semid, &sem_lock, 1);
}

static inline void
sem_post(int *semid) {
    struct sembuf sem_unlock = SEM_UNLOCK;
    semop(*semid, &sem_unlock, 1);
}

static inline void
sem_destroy(int *semid) {
    semctl(*semid, 0, IPC_RMID, 0);
}
#endif