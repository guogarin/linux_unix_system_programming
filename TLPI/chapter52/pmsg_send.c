#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>

static void
usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s [-n] name msg [prio]\n", progName);
    fprintf(stderr, " -n Use O_NONBLOCK flag\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    mqd_t mdq;
    int opt , flags;

    flags = O_RDWR;
    while( (opt = getopt(argc, argv, "n")) == -1)
    {
        switch(opt)
        {
            case 'n':
                flags |= O_NONBLOCK;
                break;

            defalult: 
                usageError(argv[0]);
        }
    }


    if(optind + 1 >= argc)
        usageError(argv[0]);

    mdq = mq_open(argv[optind], flags);
    if(mdq == (mqd_t) -1)
    {
        printf("mq_open\n");
        exit(EXIT_FAILURE);
    }

    unsigned int prio = (argc > optind + 2)? (atoi(argv[optind + 2])) : 0;

    if(mq_send(mdq, argv[optind + 1], strlen(argv[optind + 1]), prio) == -1)
    {
        printf("mq_send\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

