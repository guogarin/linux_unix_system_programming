#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int numPipes, j, ready, randPipe, numWrites;
    int (*pfds)[2];
    struct pollfd *pollFd;

    if (argc < 2 || strcmp(argv[1], "--help") == 0){
        printf("usageErr: %s num-pipes [num-writes]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numPipes = atoi(argv[1]);
    numWrites = (argc > 2) ? atoi(argv[2]) : 1;
    
    pfds = calloc(numPipes, sizeof(int [2]) );
    if(pfds == NULL){
        printf("calloc pfds\n");
        exit(EXIT_FAILURE);
    }

    pollFd = calloc(numPipes, sizeof(struct pollfd));
    if(pollFd == NULL){
        printf("calloc pollFd\n");
        exit(EXIT_FAILURE);
    }

    for(j = 0; j < numPipes; j++){
        if(pipe(pfds[j]) == -1){
            printf("pipe\n");
            exit(EXIT_FAILURE);
        }
    }   

    srandom( (int) time(NULL));
    for(j = 0; j < numWrites; j++){
        randPipe = random() % numPipes;
        printf("Writing to fd: %3d (read fd: %3d)\n",pfds[randPipe][1], pfds[randPipe][0]);
        if( write(pfds[randPipe][1], "a", 1) == -1){
            printf("write\n");
            exit(EXIT_FAILURE);
        }
    }

    for(j = 0; j < numPipes; j++){
        pollFd[j].fd = pfds[j][0];
        pollFd[j].events = POLLIN;
    }

    ready = poll(pollFd, numPipes, -1);
    if (ready == -1){
        printf("poll\n");
        exit(EXIT_FAILURE);
    }

    printf("poll() returned: %d\n", ready);

    for (j = 0; j < numPipes; j++)
        if (pollFd[j].revents & POLLIN)
            printf("Readable: %d %3d\n", j, pollFd[j].fd);

    exit(EXIT_SUCCESS);
    
}
