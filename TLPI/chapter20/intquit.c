#include<signal.h>
#include<stdio.h>
#include<stdlib.h>

static void sigHandler(int sig)
{
    static int count = 0;

    if(sig == SIGINT)
    {
        count++;
        printf("Caught SIGINT (%d)\n", count);
        return;
    }
    
    printf("Caught SIGQUIT - that's all folks!\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    if(signal(SIGINT, sigHandler) == SIG_ERR)
    {
        printf("signal\n");
        exit(EXIT_FAILURE);
    }
    if(signal(SIGQUIT, sigHandler) == SIG_ERR)
    {
        printf("signal\n");
        exit(EXIT_FAILURE);
    }

    for(;;)
        pause();
}
