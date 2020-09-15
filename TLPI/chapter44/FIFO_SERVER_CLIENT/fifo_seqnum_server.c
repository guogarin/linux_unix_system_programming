#include <signal.h>
#include "fifo_seqnum.h"

static void handler(int sig)
{
   if(unlink(SERVER_FIFO)== 0)
       printf("SERVER_FIFO is deleted.\n");
   else
       printf("Delete SERVER_FIFO failed\n");
   exit(0);
}

int main(int argc, char *argv[])
{
    struct request req;
    struct response rsp;
    
    char clientFifo[CLIENT_FIFO_NAME_LEN];

    int seqNum = 0;

    int serverFd, clientFd, dummyFd;
    
    struct sigaction sa;
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if( sigaction(SIGINT, &sa, NULL) == -1)
    {
        perror("sigaction SIGINT");
        exit(EXIT_FAILURE);
    }

    if(signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        perror("sigaction SIGPIPE");
        exit(EXIT_FAILURE);
    }

    umask(0);// 为下面创建FIFO时可以得到想要的权限
    if(mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno == EEXIST)
    {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    
    printf("SERVER_FIFO is successfully made\n");

    serverFd = open(SERVER_FIFO, O_RDONLY);
    if(serverFd == -1)
    {
        perror("open SERVER_FIFO");
        exit(EXIT_FAILURE);
    }

    printf("open SERVER_FIFO success, mode: O_RDONLY\n");

    dummyFd = open(SERVER_FIFO, O_WRONLY);
    if(dummyFd== -1)
    {
        perror("open SERVER_FIFO dummyFd");
        exit(EXIT_FAILURE);
    }
    
    printf("open SERVER_FIFO success, it is dummyFd\n");

    for(;;)
    {
        if(read(serverFd, &req, sizeof(struct request)) != sizeof(struct request))
        {
            fprintf(stderr, "Error reading request; discarding\n");
            continue;
        }
        printf("Got message from PID: %ld, seqNum: %d\n", req.pid, req.seqLen);

        snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE, req.pid);
        printf("clientFifo catenated success: %s\n", clientFifo);

        clientFd = open(clientFifo,O_WRONLY);
        if(clientFd == -1) 
        {
            fprintf(stderr, "open clientFd\n");
            continue;
        }
        printf("open clientFifo success\n");

        seqNum += req.seqLen;
        rsp.seqNum = seqNum;
        if(write(clientFd, &rsp, sizeof(struct response)) != sizeof(struct response))
        {
            fprintf(stderr, "write clientFifo error\n");
            continue;
        }
        printf("write clientFd success\n");

    }




}
