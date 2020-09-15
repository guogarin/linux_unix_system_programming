/*
 * 习题4-2 编写一个类似于 cp 命令的程序，当使用该程序复制一个包含空洞（连续的空字节）
 * 的普通文件时，要求目标文件的空洞与源文件保持一致。
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<error.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>

#ifndef BUF_SIZE
#define BUF_SIZE 4096 // 块大小为单位，效率最高
#endif

int main(int argc, char const * argv[])
{
    int inFD, outFd;
    char buf[BUF_SIZE];
    
    if(argc < 3)
    {
        perror("Ussage Error.");
        exit(EXIT_FAILURE);
    }

    inFD = open(argv[1], O_RDONLY);
    if(inFD == -1)
    {
        perror("Open source file failed!");
        exit(EXIT_FAILURE);
    }

    outFd = open(argv[2], O_WRONLY|O_CREAT, S_IWUSR|S_IRUSR|S_IWGRP|S_IRGRP);
    if(outFd == -1)
    {
        perror("Open dst file failed!");
        exit(EXIT_FAILURE);
    }
    
    ssize_t numRead, numWritten;
    // 不完全初始化时，没有被初始化的元素自动为 0,这就意味着zero里面有BUF_SIZE个0 
    char zero[BUF_SIZE] = {0};// 不完全初始化
    while(((numRead = read(inFD, buf, BUF_SIZE)) > 0))
    {
        // 在空洞文件中，空洞里的内容填的都是0
        // 将读到的块和包含块大小（4096字节）的一个全为0的字符串进行比较，相等就意味着是空洞文件。
        if(memcmp(buf, zero, numRead) == 0) // 块大小为单位进行内存比较
        {
            if(lseek(outFd, numRead, SEEK_CUR) == -1)// 偏移指针，生成空洞
            {
                perror("lseek failed!");
                exit(EXIT_FAILURE);
            }
        }
        else // 正常的数据，直接写就好了
        {
           if( ( numWritten =  write(outFd, buf, numRead) ) != numRead )
           {
               perror("Couldn't write whole buffer!");
               exit(EXIT_FAILURE);
           }

        }

    }

    if(numRead == -1)
    {
        perror("Read error!");
    }
    
    off_t len = lseek(outFd, 0, SEEK_CUR); // 获取总偏移量
    ftruncate(outFd, len);

    return 0;

}
