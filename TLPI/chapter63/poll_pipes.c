#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <poll.h>


int main(int argc, char *argv[])
{
    int numPipes, j, ready, randPipes, numWrites;
    int (*pfds)[2];
    struct pollfd *pollFd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("usageErr: %s num-pipes [num-writes]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numPipes = atol(argv[1]);

    

}
