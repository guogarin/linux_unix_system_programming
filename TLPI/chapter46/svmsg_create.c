#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

static void usageError(const char * progName, const char *msg)
{
    if(msg != NULL)
        fprintf(stderr, "%s", msg);
    fprintf(stderr, "Usage: %s [-cx] {-f pathname | -k key | -p} "
            "[octal-perms]\n", progName);
    fprintf(stderr, " -c Use IPC_CREAT flag\n");
    fprintf(stderr, " -x Use IPC_EXCL flag\n");
    fprintf(stderr, " -f pathname Generate key using ftok()\n");
    fprintf(stderr, " -k key Use 'key' as key\n");
    fprintf(stderr, " -p Use IPC_PRIVATE key\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int numKeyFlags;
    int flags, msqid, opt;
    unsigned int perms;
    long lkey;
    key_t key;

    while((opt = getopt(argc, argv, "cf:k:px")) != -1)
    {
        switch(opt)
        {
            case 'c':
                flags |= IPC_CREAT;
                break;
            case 'f': /* -f pathname */
                key = ftok(optarg, 1);
                if (key == -1)
                {
                    printf("ftok\n");
                    exit(EXIT_FAILURE);
                }
                numKeyFlags++;
                break;
            case 'k': /* -k key (octal, decimal or hexadecimal) */
                if (sscanf(optarg, "%li", &lkey) != 1)
                {
                    printf("cmdLineErr(-k option requires a numeric argument\n");
                    exit(EXIT_FAILURE);
                }
                key = lkey;
                numKeyFlags++;
                break;
            case 'p':
                key = IPC_PRIVATE;
                numKeyFlags++;
                break;
            case 'x':
                flags |= IPC_EXCL;
                break;
            default:
                usageError(argv[0], "Bad option\n");
        }
    }

    if (numKeyFlags != 1)
        usageError(argv[0], "Exactly one of the options -f, -k, "
                "or -p must be supplied\n");

    perms = (optind == argc) ? (S_IRUSR | S_IWUSR) : atoi(argv[optind]);

    msqid = msgget(key, flags | perms);
    if(msqid == -1)
    {
        printf("msgget\n");
        exit(EXIT_FAILURE);
    }
    printf("%d\n", msqid);
    exit(EXIT_SUCCESS);
}
