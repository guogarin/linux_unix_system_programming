#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define CLIENT_FIFO_TEMPLATE "/tmp/seqnum_cl.%ld"

#define CLIENT_FIFO_NAME_LEN (sizeof(CLIENT_FIFO_TEMPLATE) + 20  )


#define SERVER_FIFO "/tmp/seqnum_sv"

struct request {
    int seqLen;
    pid_t pid;
};

struct response {
    int seqNum;
};
