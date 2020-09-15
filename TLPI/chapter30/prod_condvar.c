#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

static pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx  = PTHREAD_MUTEX_INITIALIZER;

static avail = 0;


static void * threadFunc(void * arg)
{
    int cnt = atoi(*(char* arg));// 注意 atoi() 函数的形参为 char*
    int s, j;
    
    // 生产者代码
    for(j = 0; j < cnt; j++)
    {
        s = pthread_mutex_lock(&mtx );
        if(s != 0)
        {
            printf("pthread_mutex_lock\n");
            exit(EXIT_FAILURE);
        }

        avail++;

        s = pthread_mutex_unlock(&mtx);
        if(s != 0)
        {
            printf("pthread_mutex_unlock\n\n");
            exit(EXIT_FAILURE);
        }

        s = pthread_cond_signal(&cond);
        if(s != 0)
        {
            printf("pthread_cond_signal\n");
            exit(EXIT_FAILURE);
        }

    }
    return NULL;
}


int main(int argc, char *argv[])
{
    pthread_t tid;
    int s, j;
    int toRequired;

    int numConsumed;
    int done;
    time_t t;

    t = time(NULL);

    toRequired = 0;
    for(j = 1; j < argc; j++)
    {
        toRequired += atoi(argv[j]);

        s = pthread_create(&tid, NULL, threadFunc, argv[j]);
        if(s != 0)
        {
            printf("pthread_create\n");
            exit(EXIT_FAILURE);
        }

        //等待生产者生产
        while(avail == 0)
        {
            s = pthread_cond_wait(&cond, &mtx);
            if(s != 0)
            {
                printf("pthread_cond_wait\n");
                exit(EXIT_FAILURE);
            }
        }
        
        //开始消费
        while(avail > 0)
        {
            numConsumed++;
            avail--;
            printf("T=%ld: numConsumed=%d\n", (long) (time(NULL) - t),numConsumed);
            done = numConsumed >= totRequired;
        }

        s = pthread_mutex_unlock(&mtx);
        if(s != 0)
        {
            printf("pthread_mutex_unlock\n");
            exit(EXIT_FAILURE);
        }

        if(done)
            break;
    }

    exit(EXIT_SUCCESS);


    }
}
