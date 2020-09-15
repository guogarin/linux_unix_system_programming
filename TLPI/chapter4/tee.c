/*** 习题4-1 实现 tee 命令 
     1. 实现 -a 选项，实现在文件结尾处追加数据。 
***/

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/stat.h>
//#include<sys/types.h>

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif


int main(int argv, char const *argc[])
{
    int fd;
    if(argv == 3) // 带命令行选项
    {
        fd = open(argc[2], O_WRONLY|O_APPEND); 
        if(fd == -1)
        {
            perror("\nopen erro\n");
            // EXIT_FAILURE定义在头文件 stdlib.h 中
            exit(EXIT_FAILURE);
        }
                
    }
    else if(argv == 2) // 不带命令行选项
    {
        fd = open(argc[1], O_WRONLY|O_CREAT, S_IWUSR|S_IRUSR|S_IWGRP|S_IRGRP|S_IWOTH|S_IROTH);
        if(fd == -1)
        {
            perror("\nopen erro\n");
            exit(EXIT_FAILURE);
        }  
    }
    else
    {   
        /* linux命令行usage格式解释
         * 方括号[]：表示可选参数，也就是说可以忽略，使用时没有方括号。
         * 尖括号<>：表示必选参数，不能忽略，且尖括号里的输入只能是指定的option，使用时没有尖括号。
         * 省略号…：表示重复多次的参数。
         */
        printf("usage: ./%s [-a] <filename>", argc[0]);
        exit(EXIT_FAILURE);
    }
    
    char buf[BUFSIZE];
    ssize_t numRead, numWritten, stdoutWrNum; // 注意read()和write()函数的返回类型。
    // STDIN_FILENO： 接收键盘的输入
    // STDOUT_FILENO：向屏幕输出
    while( (numRead = read(STDIN_FILENO, buf, BUFSIZE)) > 0 )
    {
        numWritten = write(fd, buf, numRead);
        stdoutWrNum = write(STDOUT_FILENO, buf, numRead);
        if(numRead != numWritten || stdoutWrNum != numWritten)
        {
            perror("出现了部分写！\n");
            exit(EXIT_FAILURE);
        }
    }
    
    // 判断是否读取成功(若未读取成功，则read()函数将返回 -1)
    if(numRead == -1)
    {
        perror(strerror(errno));
    }

    // 判断是否成功关闭文件
    if(close(fd) == -1)
    {
        perror("\nclose erro.\n");
    }

    return 0;

}



















