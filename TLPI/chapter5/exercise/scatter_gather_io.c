/* 5-7． 使用 read()、 write()以及 malloc 函数包（见 7.1.2 节）中的必要函数以实现 readv()
 * 和 writev()功能。
 */

#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include<sys/uio.h>
#include<sys/stat.h>

/*
ssize_t my_readv(int fd, const iovec *iov, int iovcnt)
{

}
*/


ssize_t my_writev(int fd, const struct iovec *iov, int iovcnt)
{
    //fd的有效性
    if(fcntl(fd, F_GETFL, 0) == -1&&errno==EBADF)
    {
        printf("无效的fd");
        return -1;
    }

    if(iov == NULL)
    {
        printf("pointer iov is NULL!\n");
        return -1;
    }

    if(iovcnt<1)
    {
        printf("iovcnt 小于1");
        return -1;
    }
    
    int i, length;
    length = 0;
    for(i = 0; i < iovcnt; i++)
    {
        length += iov[i].iov_len;
    }
    char data2write[length];
    printf("Before strcat.\n");
    for(i = 0; i < iovcnt; i++)
    {
        printf("%s\n", iov[i].iov_base);
        strcat(data2write, iov[i].iov_base);
    }
    printf("%s\n", data2write);
    
    ssize_t numWritten = write(fd, data2write, length);
    if(numWritten == -1)
    {
        printf("write erro!\n");
        return 0;
    }
    
    if(-1 == close(fd))
    {
        printf("close erro\n");
        return -1;
    }
    return 0;
    

}

int main(int argv, int argc[])
{
    char str1[] = "Hello, I'm str1, who is there?\n";
    char str2[] = "Hi, This is str2,How are you?\n";
    char str3[] = "你好，我是str3";
    
    int fd = open("test_scatter_gather.txt", O_CREAT|O_RDWR, S_IWUSR|S_IRUSR);
    if(fd == -1)
    {
        printf("open test_scatter_gather.txt failed.\n");
        exit(EXIT_FAILURE);
    }
    
    struct iovec iov[3];
    iov[0].iov_base = str1;
    iov[0].iov_len = strlen(str1);
    iov[1].iov_base = str2;
    iov[1].iov_len = strlen(str2);
    iov[2].iov_base = str3;
    iov[2].iov_len = strlen(str3);
    printf("before call my_writev\n");
    if(my_writev(fd, iov, 3) == 0)
    {
        printf("Success!\n");
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("Failed!\n");
        exit(EXIT_FAILURE);
    }

}
