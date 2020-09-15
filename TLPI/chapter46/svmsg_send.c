#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>

#define MAX_MTEXT 1024

struct mbuf 
{
    long mtype;             // 报文类型
    char mtext[MAX_MTEXT];  // 报文体
};

static void usageError(const char *progName, const char * msg)
{
    if (msg != NULL)
        fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "Usage: %s [-n] msqid msg-type [msg-text]\n", progName);
    fprintf(stderr, " -n Use IPC_NOWAIT flag\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int msqid, flags, msgLen;
    struct mbuf msg; /* Message buffer for msgsnd() */
    int opt; /* Option character from getopt() */

    flags = 0;

    while((opt = getopt(argc, argv, "n")) != -1)
    {
         switch(opt)
         {
             case 'n':
                 flags |= IPC_NOWAIT;
                 break;

             default:
                 usageError(argv[0], NULL);
         }
    }

    if(argc < optind + 2 || argc > optind + 3)
    {
        printf("Wrong para number(s)\n");
        exit(EXIT_FAILURE);
    }

    msqid = atoi(argv[optind]);
    msg.mtype = atol(argv[optind + 1]);

    if(argc > optind + 2)
    {
        msgLen = strlen(argv[optind + 2]) + 1;
        if(msgLen > MAX_MTEXT)
        {
            printf("Message is too long!\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            memcpy(msg.mtext, argv[optind + 2], msgLen);
        }
    }
    else
    {
        msgLen = 0;
    }

    if(msgsnd(msqid, &msg, msgLen, flags) == -1)
    {
        printf("msgsnd\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
    
