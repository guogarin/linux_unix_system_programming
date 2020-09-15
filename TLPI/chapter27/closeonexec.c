#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>


int
main(int argc, char *argv[])
{
    int flags;

    if(argc > 1)
    {
        flags = fcntl(STDOUT_FILENO, F_GETFD);
        if( flags == -1)
        {
            printf("errExit: fcntl - F_GETFD\n");
            exit(EXIT_FAILURE);
        }
            flags |= FD_CLOEXEC;

            if(fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)\
            {
                printf("errExit: fcntl - F_SETFD\n");
                exit(EXIT_FAILURE);
            }
    }

    execlp("ls", "ls", "-l", argv[0], (char *) NULL);
    printf("errExit: execlp\n");
    exit(EXIT_FAILURE);
}
