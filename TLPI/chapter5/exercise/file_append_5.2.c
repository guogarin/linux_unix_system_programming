#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>


int main(int argv, char * argc[])
{
    if(argv < 2 || strcmp(argc[1], "--help") == 0 )
    {
        printf("useage: .//%s filename data2append\n", argc[1]);
    }
    
    int flag = O_RDWR/* | O_APPEND*/;

    int fd = open(argc[1], flag);
    if(fd ==  -1)
    {
        printf("open erro.\n");
        exit(EXIT_FAILURE);
    }

    if(lseek(fd, 0, SEEK_SET) == -1)
    {
        printf("lseek erro.\n");
        exit(EXIT_FAILURE);
    }

    if( write(fd, argc[2], sizeof(argc[2])) == -1)
    {
        printf("write erro.\n");
        exit(EXIT_FAILURE);
    }

    if(close(fd) == -1)
    {
        printf("close erro.\n");
        exit(EXIT_FAILURE);
    }

    exit(0);


}
