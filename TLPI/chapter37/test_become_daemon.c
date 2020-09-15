#include <stdio.h>
#include <unistd.h>
#include "become_daemon.h"
int main()
{
    if(becomeDaemon(0) == -1)
        printf("Failed in becomeDaemon\n");

    sleep(60);
}
