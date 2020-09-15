#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

static void* threadFunc(void *arg)
{
    char *s = (char *) arg;
    printf("%s\n", s);

    return (void *) strlen(s);
}


int main(int argc, char *argv[])
{
    pthread_t t1;
    void *res;
    int s;

    s = pthread_create(&t1, NULL, threadFunc, "Hello world");
    if(s != 0)
    {
        printf("pthread_create\n");
        exit(EXIT_FAILURE);
    }

    printf("Message from main()\n");
    pthread_join(pthread_self(), NULL);
    s = pthread_join(t1, &res);
    if(s != 0)
    {
        printf("pthread_join\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Thread returned %ld\n", (long) res);
    exit(EXIT_SUCCESS);

}
