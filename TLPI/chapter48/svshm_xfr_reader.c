#include "svshm_xfr.h"
#include <stdio.h>
    
int main(int argc, char *argv[])
{
    int semid, shmid, bytes, xfrs;
    struct shmseg *shmp;

    semid = semget(SEM_KEY, 2, OBJ_PERMS);
    if (semid == -1)
    {
        printf("semget\n");
        exit(EXIT_FAILURE);
    }

    shmid = shmget(SHM_KEY, sizeof(struct shmseg),  OBJ_PERMS);
    if (shmid == -1)
    {
        printf("shmget\n"); 
        exit(EXIT_FAILURE);
    }

    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1)
    {
        printf("shmp\n");
        exit(EXIT_FAILURE);
    }

    for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->cnt) 
    {
        if (reserveSem(semid, READ_SEM) == -1)
        {
            printf("reserveSem\n"); 
            exit(EXIT_FAILURE);
        }

        if(shmp->cnt == 0)
            break;

        if(shmp->cnt != write(STDOUT_FILENO, shmp->buf,shmp->cnt))
        {
            printf("write\n");
            exit(EXIT_FAILURE);
        }

        if(releaseSem(semid, WRITE_SEM) == -1)
        {
            printf("releaseSem\n"); 
            exit(EXIT_FAILURE);
        }

    }

    if (shmdt(shmp) == -1)
    {
        printf("shmdt\n");
        exit(EXIT_FAILURE);
    }

    if(releaseSem(semid, WRITE_SEM) == -1)
    {
        printf("releaseSem\n");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
    exit(EXIT_SUCCESS);
}
