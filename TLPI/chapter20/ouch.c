#include<stdio.h>
#include<signal.h>
#include<stdlib.h>

static void sigHandler(int sig)
{
    printf("Outch!\n");
}


int main(int argc, char *argv[])
{
    int j;

    if(signal(SIGINT, sigHandler) == SIG_ERR)
    {
        printf("signal\n");
        exit(EXIT_FAILURE);
    }

    for(j = 0; ; j++)
    {
        printf("%d\n", j);
        sleep(3);
    }
}
