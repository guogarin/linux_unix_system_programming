#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define BUF_SIZE 10

int main(int argc, const char *argv[])
{
    int pfd[2];
    int numRead;

    if( (pipe(pfd)) == -1)
    {
        printf("pipe\n");
        exit(EXIT_FAILURE);
    }

    switch(fork())
    {
        case -1:
            printf("fork\n");
            exit(EXIT_FAILURE);
            break;
        case 0:
            if(close(pfd[1]) == -1) //资金从关闭写入端fd
            {
                printf("close\n");
                exit(EXIT_FAILURE);
            }
            
            char buf[10];
            for(;;)
            {
                if( (numRead = read(pfd[0], buf, BUF_SIZE)) == -1)
                {
                    printf("read\n");
                    exit(EXIT_FAILURE);
                }

                if(numRead == 0)
                    break;
                write(STDOUT_FILENO, buf,numRead);
            }
            
            if(close(pfd[0]) == -1)
            {
                printf("close\n");
                exit(EXIT_FAILURE);
            }

            write(STDOUT_FILENO,"\n", 1);

            _exit(EXIT_SUCCESS);

            break;

        default:
            if(close(pfd[0]) == -1)
            {
                printf("close\n");
                exit(EXIT_FAILURE);
            }
            if((numRead = write(pfd[1], argv[1], strlen(argv[1]))) == -1)
            {
                printf("write\n");
                exit(EXIT_FAILURE);
            }
            if(numRead < strlen(argv[1]))
            {
                printf("Part write.\n");
            }
            
            if(close(pfd[1]) == -1)
            {
                printf("close\n");
                exit(EXIT_FAILURE);
            }
            wait();
            break;
               
    }

    exit(EXIT_SUCCESS);
}
