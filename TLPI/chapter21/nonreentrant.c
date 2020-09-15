#define _XOPEN_SOURCE 600
#include<unistd.h>
#include<signal.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

static char *str2;
static int handled = 0;

static void handler(int sig)
{
    crypt(str2, "xx");
    handled++ ;
}

int main(int argc, char *argv[])
{
    char *cr1;
    int calNum, mismatch;
    struct sigaction sa;

    if(argc != 3)
    {
        printf("usageErr: %s str1 str2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    str2 = argv[2];
    cr1= strdup(crypt(argv[1], "xx"));/* strdup可以直接把要复制的内容复制给没有初始化的指针，                                        因为它会自动分配空间给目的指针。*/
    if(cr1 == NULL)
    {
        printf("strdup\n");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if(sigaction(SIGINT,&sa, NULL) == -1)
    {
        printf("sigaction\n");
        exit(EXIT_FAILURE);
    }

    for(calNum = 1, mismatch = 0;;calNum++)
    {
        if(strcmp(crypt(argv[1], "xx"), cr1) != 0)
        {
            mismatch++;
            printf("Mismatch on call %d (mismatch=%d handled=%d)\n",calNum, mismatch, handled);
        }
    }
}
