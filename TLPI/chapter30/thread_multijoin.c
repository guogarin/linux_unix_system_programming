#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


static pthread_cond_t  threadDied  = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;


static int totThreads = 0; // 线程总数
static int numLive = 0;    // 还存活的线程


static int numUnjoined = 0; //尚未join的线程总数


// 线程状态
enum tstate{
    TS_ALIVE,
    TS_TERMINATED,
    TS_JOINED,
};

// 线程的相关信息
static struct {
    pthread_t tid;
    enum tstate state;
    int sleepTime;
} *thread; 


static void * threadFunc(void * arg)
{
    int idx = *((int *) arg);
    int s;

    sleep(thread[idx].sleepTime);
    printf("Thread %d terminating\n", idx);

    s = pthread_mutex_lock(&threadMutex);
    if(s != 0)
    {
        printf("pthread_mutex_lock\n");
        exit(EXIT_FAILURE);
    }

    numUnjoined++;
    thread[idx].state = TS_TERMINATED;

    s = pthread_mutex_unlock(&threadMutex);
    if(s != 0)
    {
        printf("pthread_mutex_unlock\n"); 
        exit(EXIT_FAILURE);
    }

    s = pthread_cond_signal(&threadDied);
    if(s != 0)
    {
        printf("pthread_cond_signal\n");
        exit(EXIT_FAILURE);
    }

    return NULL;
    
}


int main(int argc, char *argv[]) 
{
    int s, idx; 

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s num-secs...\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // calloc 和 malloc的区别
    // 1. 它们请求内存的方式不一样；
    // 2. calloc() 在返回指向内存的指针之前将其初始化为0；
    thread = calloc(argc - 1, sizeof(*thread));
    if(thread == NULL)
    {
        printf("calloc\n");
        exit(EXIT_FAILURE);
    }

    // 新建所有线程
    for(idx = 0; idx < argc -1; idx++)
    {
        thread[idx].sleepTime = atoi(argv[idx + 1]);
        thread[idx].state = TS_ALIVE;
        s = pthread_create(&thread[idx].tid, NULL, threadFunc, &idx);
        if(s != 0) 
        {
            printf("pthread_create\n");
            exit(EXIT_FAILURE);
        }
    }

    totThreads = argc -1;
    numLive    = totThreads;


    // join 状态为 terminated 的线程
    while(numLive > 0)
    {
        s = pthread_mutex_lock(&threadMutex);
        if(s != 0)
        {
            printf("pthread_mutex_lock\n");
            exit(EXIT_FAILURE);
        }

        while(numLive == 0)
        {
            s = pthread_cond_wait(&threadDied, &threadMutex);
            if(s != 0) 
            {
                printf("pthread_cond_wait\n");
                exit(EXIT_FAILURE);
            }
        }
        for(idx = 0; idx < totThreads; idx++){
            if(thread[idx].state == TS_TERMINATED){
                s = pthread_join(thread[idx].tid, NULL);
                if(s != 0) {
                    printf("pthread_join\n");
                    exit(EXIT_FAILURE);
                }
            }
            
            thread[idx].state = TS_JOINED;
            numLive--;
            numUnjoined--;
            printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
        }
    }

    s = pthread_mutex_unlock(&threadMutex);
    if(s != 0) {
        printf("pthread_mutex_unlock\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}
