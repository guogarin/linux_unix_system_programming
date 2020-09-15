#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>


static void handler(int sig)
{
    printf("PID %ld: caught signal %2d (%s)\n",(long) getpid(), sig, strsignal(sig));
}


int main(int argc, char *argv[])
{
    pid_t parentPid, childPid;
    struct sigaction sa;
    int j;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s {d|s}... [ > sig.log 2>&1 ]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    setbuf(stdout, NULL);

    parentPid = getpid();
    printf("PID of parent process is: %ld\n", (long) parentPid);
    printf("Foreground process group ID is: %ld\n",
            (long) tcgetpgrp(STDIN_FILENO));

    for(j = 1; j < argc; j++)
    {
        childPid = fork();
        
        if(childPid == -1)
        {
            printf("erro fork\n");
            exit(EXIT_FAILURE);
        }
            
        if(childPid == 0)
        {
            if(argv[j][0] == 'd')
            {
                if(setpgid(0,0) == -1)
                {
                    printf("erro setpgid\n");
                    exit(EXIT_FAILURE);
                }
            }

            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = handler;
            if(sigaction(SIGHUP, &sa, NULL) == -1)
            {
                printf("erro sigaction\n");
                exit(EXIT_FAILURE);
            }


            break;
        }
    }

    alarm(60);
    
    printf("PID=%ld PGID=%ld\n", getpid(), getpgrp());

    for(;;)
    {
        pause();
    }

}
