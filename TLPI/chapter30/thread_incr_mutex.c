#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static int glob = 0; // 链接属性为internal，只能被本文件所访问，存储于静态区

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void * threadFunc(void * arg)
{
    int loops = *((int *) arg);
    int loc , j;

    for(j = 0; j < loops; j++)
    {
        pthread_mutex_lock(&mtx);
        loc = glob;
        loc++;
        glob = loc;
        pthread_mutex_unlock(&mtx);
    }

    return NULL;

}



int main(int argc, char *argv[])
{
    pthread_t t1, t2;

    int loops , s;


    loops = (argc > 1) ? atoi(argv[2]) : 10000000;

    s = pthread_create(&t1, NULL, threadFunc, &loops);
    if(s != 0)
    {
        printf("pthread_create\n");
        exit(EXIT_FAILURE);
    }

    s = pthread_create(&t2, NULL, threadFunc, &loops);
    if(s != 0)
    {
        printf("pthread_create2\n");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t1, NULL);
    if(s != 0)
    {
        printf("pthread_join\n");
        exit(EXIT_FAILURE);
    }

    s = pthread_join(t2, NULL);
    if(s != 0)
    {
        printf("pthread_join2\n");
        exit(EXIT_FAILURE);
    }

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);

}
