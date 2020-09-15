//#define _LARGEFILE64_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>


int main(int argc, char * argv[])
{
    int fd;
    off_t off;
    if(argc !=3 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s pathname offset\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR |O_CREAT, S_IRUSR|S_IWUSR);
    if(fd == -1)
    {
        printf("Erro: open.\n");
        exit(EXIT_FAILURE);
    }

    off = atoll(argv[2]);
    if(lseek(fd, off, SEEK_SET) == -1)
    {
        printf("Erro: lseek.\n");
        exit(EXIT_FAILURE);
    }

    if(write(fd, "test", 4) == -1)
    {
        printf("Erro: write.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}
