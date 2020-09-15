#include "fifo_seqnum.h"


static char clientFifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo()
{
    if(unlink(clientFifo) == 0)
        printf("clientFifo is deleted.\n");
    else
        printf("Delete clientFifo failed\n");

}

int
main(int argc, char *argv[])
{
    struct request req;
    struct response rsp;

    int serverFd, clientFd;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        printf("%s [seq-len...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, (long) getpid());
    printf("clientFifo catenated success: %s\n", clientFifo);

    int seqLen = atoi(argv[1]);
    req.pid = getpid();
    req.seqLen = seqLen;

    serverFd = open(SERVER_FIFO, O_WRONLY);
    if(serverFd == -1)
    {
        perror("open SERVER_FIFO");
        exit(EXIT_FAILURE); 
    }
    printf("open SERVER_FIFO success, mode: O_WRONLY\n");

    if(mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno == EEXIST)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE); 
    }
    printf("clientFifo is successfully created\n");
    
    if(atexit(removeFifo) != 0)
    {
        perror("atexit");
        exit(EXIT_FAILURE);
    }

    if(write(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
    {
        perror("write req");
        exit(EXIT_FAILURE);
    }
    printf("write serverFd success\n");

    clientFd = open(clientFifo, O_RDONLY);
    if(clientFd == -1)
    {
        perror("open clientFd");
        exit(EXIT_FAILURE);
    }
    printf("open clientFifo success, mode: O_RDONLY\n");

    if(read(clientFd, &rsp, sizeof(struct response)) != sizeof(struct request))
    {
        perror("read rsp");
        exit(EXIT_FAILURE);
    }
    printf("read clientFd success\n");
    
    printf("received: %l\n", &rsp.seqNum);

    //exit(EXIT_SUCCESS);

}
