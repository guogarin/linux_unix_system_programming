#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


static void handler(int arg)
{
}

static void alarm_handler (int arg)
{
    printf(" SIGALARM garin\n");
}

int main(int argc, char *argv[])
{
    struct sigaction sa;
    pid_t childPid;
    
    setbuf(stdout, NULL);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;

    if(sigaction(SIGHUP, &sa, NULL) == -1)
    {
        printf("sigaction\n");
        exit(EXIT_FAILURE);
    }

    if ((childPid = fork()) == -1)
    {
        printf("fork\n");
        exit(EXIT_FAILURE);
    }
    
/*
    if(signal(SIGALRM, alarm_handler))
    {   
        printf("signal\n");
        exit(EXIT_FAILURE);
    }
*/
    if(childPid == 0 && argc > 1)
    {
        if(setpgid(0,0) == -1)
        {
            printf("ssetpgid\n");
            exit(EXIT_FAILURE);
        }
    }
    
    printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n", (long)getpid(), (long)getppid(),
            (long)getpgrp(), (long)getsid(0));

    alarm(30);

    for(;;)
    {
        pause();
        printf("%ld: caught SIGHUP\n", (long)getpid());
    }

}
