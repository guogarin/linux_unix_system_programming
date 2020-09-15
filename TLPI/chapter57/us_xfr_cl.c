#include "us_xfr.h"


int main()
{
    struct sockaddr_un addr;
    int sfd;
    size_t numRead;
    char BUF[BUF_SIZE];

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(struct sockaddr_un) - 1);

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        printf("socket\n");
        exit(EXIT_FAILURE);
    }

    if(connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("connect\n");
        exit(EXIT_FAILURE);
    }

    while( (numRead = read(STDIN_FILENO, BUF, BUF_SIZE)) > 0)
    {
        if(write(sfd, BUF, numRead) != numRead)
        {
            printf("write\n");
            exit(EXIT_FAILURE);
        }
    }

    if(numRead == -1)
    {
        printf("read\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);


}
