#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(int argc, char *argv[])
{
    int numDead;
    pid_t childPid;
    int j;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {    
        printf("usageErr: %s sleep-time...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    setbuf(stdout, NULL);// 关闭 stdout, 的缓冲

    for(j = 0; j < argc; j++)
    {
        switch(fork())
        {
            case -1:
                printf("errExit: fork\n");
                exit(EXIT_FAILURE);

            case 0:
                printf("[] child %d started with PID %ld, sleeping %s "
                        "seconds\n", j, (long) getpid(), argv[j]);
                sleep(atoi(argv[j]));
            default:
                break;
        }
    }

    numDead = 0 ;
    for(;;)
    {
        childPid = wait(NULL);
        if(childPid == -1)
        {
            if(errno == ECHILD)
            {
                printf("No more children - bye!\n");
                exit(EXIT_SUCCESS);
            }
            else
            {
                printf("errExit: wait\n");
                exit(EXIT_FAILURE);
            }
        }

        numDead++;
        printf("[s] wait() returned child PID %ld (numDead=%d)\n",
                (long) childPid, numDead);
    }
    


}
