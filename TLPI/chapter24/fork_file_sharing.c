#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int fd, flags;
    char template[] = "/tmp/testXXXXXX";

    fd = mkstemp(template);// mkstemp()函数生成一个唯一文件名并打开该文件
    if(fd == -1)
    {
        printf("errExit: mkstemp\n");
        exit(EXIT_FAILURE);
    }

    printf("File offset before fork(): %lld\n",(long long) lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if(flags == -1)
    {
        printf("errExit: fcntl - F_GETFL\n");
        exit(EXIT_FAILURE);
    }

    printf("O_APPEND flag before fork() is: %s\n",(flags & O_APPEND) ? "on" : "off");

    switch(fork())
    {
        case -1:
            printf("errExit: fork\n");
            exit(EXIT_FAILURE);

        case 0:
            if(lseek(fd, 1000, SEEK_SET) == -1)
            {
                printf("errExit: lseek\n");
                exit(EXIT_FAILURE);
            }

            flags = fcntl(fd,F_GETFL);
            if(flags == -1)
            {
                printf("errExit: fcntl - F_GETFL\n");
                exit(EXIT_FAILURE);
            }
            flags |= O_APPEND;
            if(fcntl(fd, F_SETFL, flags) == -1)
            {
                printf("errExit: fcntl - F_SETFL");
                exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);

        default:
            if(wait(NULL) == -1)
            {
                printf("errExit: wait\n");
                exit(EXIT_FAILURE);
            }
            printf("Child has exited\n");

            printf("File offset in parent: %lld\n",
                    (long long) lseek(fd, 0, SEEK_CUR));
            flags = fcntl(fd, F_GETFL);
            if (flags == -1)
            {
                printf("errExit: fcntl - F_GETFL\n");
                exit(EXIT_FAILURE);
            }

            printf("O_APPEND flag in parent is: %s\n", (flags & O_APPEND) ? "on" : "off");
            exit(EXIT_SUCCESS);
    }
}
