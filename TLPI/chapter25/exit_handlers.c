#define _BSD_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>


static void atexitFunc1(void)
{
    printf("atexit function 1 called\n");
}

static void
atexitFunc2(void)
{
    printf("atexit function 2 called\n");
}

static void
onexitFunc(int exitStatus, void *arg)
{
    printf("on_exit function called: status=%d, arg=%ld\n",
            exitStatus, (long) arg);
}

int main(int argc, char *argv[])
{
    if(on_exit(onexitFunc, (void *) 10) != 0)
    {
        printf("fatal: on_exit 1\n");
        exit(EXIT_FAILURE);
    }

    if (atexit(atexitFunc1) != 0)
    {
        printf("fatal: atexit \n");
        exit(EXIT_FAILURE);
    }

    if (atexit(atexitFunc2) != 0)
    {
        printf("fatal: atexit 2 \n");
        exit(EXIT_FAILURE);
    }

    if (on_exit(onexitFunc, (void *) 20) != 0)
    {
        printf("fatal: on_exit 2\n");
        exit(EXIT_FAILURE);
    }

    exit(2);

}


