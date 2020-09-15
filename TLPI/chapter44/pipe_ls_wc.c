#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, const char * argv[])
{
    int pfd[2];

    if(pipe(pfd) == -1)
    {
        printf("pipe\n");
        exit(EXIT_FAILURE);
    }

    switch(fork())
    {
        case -1:
            printf("fork\n");
            exit(EXIT_FAILURE);

        case 0:
            if(close(pfd[0]) == -1)
            {
                printf("close\n");
                exit(EXIT_FAILURE);
            }

            if(pfd[1] != STDOUT_FILENO)
            {
                if(dup2(pfd[1], STDOUT_FILENO) == -1)
                {
                    printf("dup2\n");
                    exit(EXIT_FAILURE);
                }
                if(close(pfd[1])== -1)
                {
                    printf("close\n");
                    exit(EXIT_FAILURE);
                }
            }
            execlp("ls", "ls", (char *) NULL);
            printf("execlp ls\n");
            exit(EXIT_FAILURE);

        default:
            break;
    }

    switch(fork()) 
    {
        case -1:
            printf("fork\n");
            exit(EXIT_FAILURE);

        case 0:
            if(close(pfd[1]) == -1)
            {
                printf("close\n");
                exit(EXIT_FAILURE);
            }
            if(pfd[0] != STDIN_FILENO)
            {
                if(dup2(pfd[0], STDIN_FILENO) == -1)
                {
                    printf("dup2\n");
                    exit(EXIT_FAILURE);
                }
                if(close(pfd[0])== -1)
                {
                    printf("close\n");
                    exit(EXIT_FAILURE);
                }
            }
            execlp("wc", "wc", "-l", (char *) NULL);
            printf("execlp wc\n");
            exit(EXIT_FAILURE);

        default:
            break;

    }
    
    if (close(pfd[0]) == -1)
    {
        printf("close\n");
        exit(EXIT_FAILURE);
    }
    if (close(pfd[1]) == -1)
    {
        printf("close\n");
        exit(EXIT_FAILURE);
    }
    if (wait(NULL) == -1)
    {
        printf("close\n");
        exit(EXIT_FAILURE);
    }
    if (wait(NULL) == -1)
    {
        printf("close\n");
        exit(EXIT_FAILURE);
    }

    printf("Success.\n");
    exit(EXIT_SUCCESS);
}
