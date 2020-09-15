#include<unistd.h>
#include<sys/stat.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#define CMD_SIZE 200
#define BUF_SIZE 1024


int main(int argc, char *argv[])
{
    int fd, j, numBlocks;
    char shellCMD[CMD_SIZE];
    char buf[BUF_SIZE];

    if(argc < 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("usage: %s temp-file [num-1kB-blocks] \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    numBlocks = (argc > 2)?atoi(argv[2]):100000;

    fd=open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if(fd == -1)
    {
        printf("open wrro\n");
        exit(EXIT_FAILURE);
    }

    if(unlink(argv[1]) == -1)
    {
        printf("unlink\n");
        exit(EXIT_FAILURE);
    }

    for(j = 0; j < numBlocks; j++)
    {
        if(write(fd, buf, BUF_SIZE) != BUF_SIZE)
        {
            printf("write error!\n");
            exit(EXIT_FAILURE);
        }
    }

    snprintf(shellCMD, CMD_SIZE, "df ", argv[1]);
    system(shellCMD);

    if(close(fd) == -1)
    {
        printf("close error\n");
        exit(EXIT_FAILURE);
    }

    printf("********** Closed file descriptor\n");

    system(shellCMD);
    exit(EXIT_SUCCESS);

}
