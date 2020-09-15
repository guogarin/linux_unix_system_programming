#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


static void * threadFunc(void *arg)
{
    int j;
    printf("New thread started\n");
    for(j = 1; ; j++)
    {
        printf("Loop %d\n", j);
        sleep(1);
    }

    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t thr;
    int s;
    void *res;

    s = pthread_create(&thr, NULL, threadFunc, NULL);
    if (s != 0)
    {
        printf("pthread_create\n");
        exit(EXIT_FAILURE);
    }

    sleep(3);

    s =pthread_cancel(thr);
    if (s != 0)
    {
        printf("=pthread_cancel\n");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(thr, &res);
    if (s != 0) 
    {
        printf("pthread_join\n");
        exit(EXIT_FAILURE);
    }

    if(res == PTHREAD_CANCELED)
        printf("Thread was canceled\n");
    else
        printf("Thread was not canceled (should not happen!)\n");
    exit(EXIT_SUCCESS);

}
