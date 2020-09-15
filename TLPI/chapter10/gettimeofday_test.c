#include<sys/time.h>
#include<stdio.h>


int main(int argc, char argv[])
{
    struct timeval tv;
    if(gettimeofday(tv, NULL) == -1)
    {
        printf("gettimeofday erro\n");
        exit(0);
    }
    printf("after gettimeofday.\n");
    printf("time_t: %ld , suseconds_t: %ld\n", tv.tv_sec, tv.tv_usec);
    return 0;
}
