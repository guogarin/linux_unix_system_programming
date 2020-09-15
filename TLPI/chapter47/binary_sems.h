#ifndef BINARY_SEMS_H
#define BINARY_SEMS_H

#include <sys/types.h>  // for portablity
#include <sys/sem.h>    // 信号量相关函数所在头文件
#include <errno.h>


// Definition of the bool type
typedef int bool;
#define False 0
#define True  1


extern bool bsUseSemUndo; /* Use SEM_UNDO during semop()? */
extern bool bsRetryOnEintr; /* Retry if semop() interrupted by
                                  signal handler? */

int initSemAvailable(int semId, int semNum);
int initSemInUse(int semId, int semNum);
int reserveSem(int semId, int semNum);
int releaseSem(int semId, int semNum);
#endif
