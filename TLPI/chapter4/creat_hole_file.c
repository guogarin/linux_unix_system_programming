/*
 **** 此文件用于创建空洞文件。来自于APUE 3.6 函数lseek
*/


#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/stat.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main()
{
    int fd;

    if ((fd = creat("file.hole", S_IWUSR|S_IRUSR|S_IWGRP|S_IRGRP)) < 0 )
    {
        perror("creat error");
        exit(EXIT_FAILURE);
    }

    if (write(fd, buf1, sizeof(buf1)) != sizeof(buf1))
    {
        perror("write buf1 error");
        exit(EXIT_FAILURE);
    }

    // 制造空洞
    if (lseek(fd, 16384, SEEK_CUR) == -1)
    {
        perror("seek buf1 error");
        exit(EXIT_FAILURE);
    }

    if (write(fd, buf2, sizeof(buf2)) != sizeof(buf2))
    {
        perror("write buf2 error");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
