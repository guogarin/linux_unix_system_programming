#include<unistd.h>
#include<sys/uio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<stdarg.h>
#include<stdlib.h>
#include<stdio.h>


int main(int argc, char *argv[])
{
    int fd;
    struct iovec iov[3];
    struct stat  mystruct;
    int x;
#define STR_SIZE 100
    char str[STR_SIZE];
    ssize_t numRead, totRequired;

    if(argc != 2 || strcmp(argv[0], "--help") == 0)
    {
        printf("usageErr! \n");
        exit(EXIT_FAILURE);
    }

    fd =open(argv[1], O_RDONLY);
    if(fd == -1)
    {
       exit(EXIT_FAILURE);
    }

    totRequired = 0;

    iov[0].iov_base = &mystruct;
    iov[0].iov_len  = sizeof(struct stat);
    totRequired += iov[0].iov_len;

    iov[1].iov_base = &x;
    iov[1].iov_len = sizeof(x);
    totRequired += iov[1].iov_len;

    iov[2].iov_base = str;
    iov[2].iov_len = sizeof(str);
    totRequired += iov[2].iov_len;
    
    numRead = readv(fd, iov, 3);
    if(numRead == -1)
    {
        printf("readv error.\n");
        exit(EXIT_FAILURE);
    }

    if (numRead < totRequired)
    {
        printf("Read fewer than required!\n");
    }

    printf(" success\n");
    exit(EXIT_SUCCESS);
}
