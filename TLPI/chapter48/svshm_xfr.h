#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "../chapter47/binary_sems.h"
#include "../chapter47/semun.h"


#define SHM_KEY 0x1234  /* Key for shared memory segment */
#define SEM_KEY 0x5678          /* Key for semaphore set */

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)

#define WRITE_SEM  0
#define READ_SEM   1

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

// Defines struture of shared memory segment
struct shmseg
{
    int cnt;            // Number of bytes used in 'buf'
    char buf[BUF_SIZE]; // Data being transferred
};
