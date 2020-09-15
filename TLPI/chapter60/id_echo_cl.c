#include <syslog.h>
#include <stdio.h>
#include "id_echo.h"

int main(int argc, char *argv[])
{
    int fd, i;
    size_t len;
    ssize_t numRead;
    char BUF[BUF_SIZE];
    if (argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("usageErr: %s: host msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if( (fd = inetConnect(argv[1], SERVICE, SOCK_DGRAM) == -1) )
    {
        printf("inetConnect\n");
        exit(EXIT_FAILURE);
    }
    
    for(i = 2; i < argc; i++)
    {
        len = strlen(argv[i]);
        if(write(fd, argv[i], len) != len)
        {
            printf("Partial write\n");
            exit(EXIT_FAILURE);
        }

        if(read(fd, BUF, BUF_SIZE) == -1)
        {
            printf("read\n");
            exit(EXIT_FAILURE);
        }
        printf("[%ld bytes] %.*s\n", (long) numRead, (int) numRead, BUF);
    }

    exit(EXIT_SUCCESS);
}

