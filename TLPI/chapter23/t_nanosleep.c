#define _POSIX_C_SOURCE 199309
#include<sys/time.h>
#include<time.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>

static void sigintHandler(int sig)
{
    return;
}


int main(int argc, char *argv[])
{
    struct timeval start, finish;
    struct timespec request, remain;
    struct sigaction sa;
    int s;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        printf("usageErr: %s secs nanosecs\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    request.tv_sec  = atol(argv[1]);
    request.tv_nsec = atol(argv[1]);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;
    if(sigaction(SIGINT, &sa, NULL) == -1)
    {
        printf("errExit:sigaction\n");
        exit(EXIT_FAILURE);
    }

    if(gettimeofday(&start, NULL) == -1)
    {
        printf("errExit:gettimeofday\n");
        exit(EXIT_FAILURE);
    }

    for(;;)
    {
        s = nanosleep(&request, &remain);
        if(s == -1 && errno != EINTR)
        {
            printf("errExit:nanosleep\n");
            exit(EXIT_FAILURE);
        }

        if(gettimeofday(&finish, NULL) == -1)
        {
            printf("errExit:gettimeofday\n");
            exit(EXIT_FAILURE);
        }

        printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec +
                (finish.tv_usec - start.tv_usec) / 1000000.0);

        if(s == 0)
            break; // 休眠完成！

        printf("Remaining: %2ld.%09ld\n", (long) remain.tv_sec,remain.tv_nsec);

        request = remain; // 若有未完成的时间，则重新开始！
    }

    printf("Sleep complete\n");
    exit(EXIT_SUCCESS);

}
