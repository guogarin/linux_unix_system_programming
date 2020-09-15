#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <unistd.h>

static void usageError(const char * progName)
{
    fprintf(stderr, "Usage: %s [-cx] [-m maxmsg] [-s msgsize] mq-name "
            "[octal-perms]\n", progName);
    fprintf(stderr, " -c Create queue (O_CREAT)\n");
    fprintf(stderr, " -m maxmsg Set maximum # of messages\n");
    fprintf(stderr, " -s msgsize Set maximum message size\n");
    fprintf(stderr, " -x Create exclusively (O_EXCL)\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    mqd_t mqd;
    int flags, opt;
    mode_t mode;
    struct mq_attr attr, *attrp;

    flags = O_RDWR;
    attr.mq_maxmsg  = 50;
    attr.mq_msgsize = 2048;
    attrp = &attr;

    while((opt = getopt(argc, argv, "cm:s:x")) != -1)
    {
        switch(opt)
        {
            case 'c':
                flags |= O_CREAT;
                break;

            case 'm':
                attr.mq_maxmsg = atol(optarg);
                attrp = &attr;
                break;

            case 's':
                attr.mq_msgsize = atol(optarg);
                attrp = &attr;
                break;

            case 'x':
                flags |= O_EXCL;
                break;

            default:
                usageError(argv[0]);
        }
    }

    if(optind  >= argc)
        usageError(argv[0]);
    
    mode = (argc <= optind + 1) ? (S_IWUSR | S_IRUSR): atoi(argv[optind + 1]);

    if( ( mqd = mq_open(argv[optind], flags, mode, attrp)) == (mqd_t) -1)
    {
        printf("mq_open\n");
        exit(EXIT_FAILURE);
    }
    

    exit(EXIT_SUCCESS);
}

