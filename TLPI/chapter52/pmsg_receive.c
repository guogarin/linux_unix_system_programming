#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


static void
usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s [-n] name\n", progName);
    fprintf(stderr, " -n Use O_NONBLOCK flag\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    int flags, opt;
    unsigned int prio;
    mqd_t mqd;
    struct mq_attr  attr;

    flags = O_RDWR;
    while( (opt =getopt(argc, argv, "n")) == -1) 
    {
        switch(opt)
        {
            case 'n':
                flags |= O_NONBLOCK;
                break;
            default:
                usageError(argv[0]);
        }
    }

    if(optind + 1 > argc)
        usageError(argv[0]);

    prio = (argc >= optind + 2) ? (atoi(argv[optind + 2])) : 0;
    
    mqd = mq_open(argv[optind], flags);
    if(mqd == (mqd_t) -1)
    {
        printf("mq_open\n");
        exit(EXIT_FAILURE);
    }

    if(mq_getattr(mqd, &attr) == -1)
    {
        printf("mq_getattr\n");
        exit(EXIT_FAILURE);
    }

    long msgSize = attr.mq_msgsize;
    void * buf = malloc(msgSize);
    if(buf == NULL)
    {
        printf("malloc\n");
        exit(EXIT_FAILURE);
    }

    ssize_t numReaded;
    if( ( numReaded = mq_receive(mqd, buf, msgSize, &prio) ) == -1)
    {
        printf("mq_receive\n");
        exit(EXIT_FAILURE);
    }

    printf("Read %ld bytes; priority = %u\n", (long) numReaded, prio);

    if(write(STDOUT_FILENO, buf, numReaded) == -1)
    {
        printf("write\n");
        exit(EXIT_FAILURE);
    }
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}
