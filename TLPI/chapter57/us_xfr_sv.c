#include "us_xfr.h"


#define BACKLOG 5


int main()
{
    struct sockaddr_un addr;
    if( remove(SV_SOCK_PATH) == -1 && errno != ENOENT )
    {
        printf("remove\n");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(struct sockaddr_un) -1);

    int sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sfd == -1)
    {
        printf("socket\n");
        exit(EXIT_FAILURE);
    }

    if( bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
    {
        printf("bind\n");
        exit(EXIT_FAILURE);
    }

    if(listen(sfd, BACKLOG) == -1)
    {
        printf("listen\n");
        exit(EXIT_FAILURE);
    }
    
    int afd;
    size_t numRead;
    char BUF[BUF_SIZE];
    for(;;)
    {
        afd = accept(sfd, 0, 0);
        if(afd == -1)
        {
            printf("accept\n");
            exit(EXIT_FAILURE);
        }
        while( ( numRead = read(afd, BUF, BUF_SIZE) ) > 0 )
        {
            if( write(STDOUT_FILENO, BUF, numRead) != numRead)
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

        if(close(afd) == -1)
        {
            printf("close\n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);


}
