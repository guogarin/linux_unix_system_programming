#include<limits.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#define BUF_SIZE PATH_MAX

int main(int argc, char * argv[])
{
    struct stat statbuf;
    char buf[BUF_SIZE];
    ssize_t numBytes;

    if(argc != 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s pathname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(lstat(argv[1], &statbuf) == -1)
    {
        printf("lstat");
        exit(EXIT_FAILURE);
    }

    if(!S_ISLNK(statbuf.st_mode))
    {
        printf("%s is not a symbolic link", argv[1]);
        exit(EXIT_FAILURE);
    }

    numBytes = readlink(argv[1], buf, BUF_SIZE - 1);
    if(numBytes == -1)
    {
        printf("readlink\n");
        exit(EXIT_FAILURE);
    }

    buf[numBytes] = '\0';
    printf("readlink: %s --> %s\n", argv[1], buf);

    if(realpath(argv[1], buf) == NULL)
    {
        printf("realpath\n");
        exit(EXIT_FAILURE);
    }
    printf("realpath: %s --> %s\n", argv[1], buf);
    
    exit(EXIT_SUCCESS);

}
