#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>


int main(int argv, char * argc[])
{
    if(argv < 2 || strcmp(argc[1], "--help") == 0 )
    {
        printf("useage: .//%s filename numByte [x] \n", argc[1]);
    }
    
    int fd, flags;
    // 若没有第三个参数
    if(argv == 2)
    {   
        flags = O_RDWR | O_APPEND;
    }
    else
    {
        flags = O_RDWR;
    }

    if((fd = open(argc[1], flags)) == -1)
    {
        printf("open erro.\n");     
        exit(EXIT_FAILURE);
    }
    
#define num2write  1000000
    
    // 因为O_APPEND打开i文件时，即使lseek到三么位置，结果都是在未端插入
    // 所以不区分命令行输入的是几个参数了
    if(lseek(fd, 0, SEEK_END) == -1)
    {
        printf("lseek erro.\n");
        exit(EXIT_FAILURE);
    }
    
    int i;
    char str[num2write + 1];
    for(i = 0; i<num2write; ++i)
    {
        str[i] = 1;
    }
    str[num2write] = '\0';

    int numWritten = write(fd, str, num2write);
    if(numWritten == -1)
    {
        printf("write erro.\n");
        exit(EXIT_FAILURE);
    }


    if(close(fd) == -1)
    {
        printf("close erro.\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}

