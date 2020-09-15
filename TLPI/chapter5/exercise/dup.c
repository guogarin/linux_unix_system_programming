// 5 - 4



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>


// 实现dup() 系统调用
int my_dup(int oldFd)
{
    return fcntl(oldFd, F_DUPFD, 3);
}

int is_file_descripter(int fd)
{
    // 检查 fd 是否有效，测试 fcntl(fd， F_GETFL)是否成功就能达到这一目的。
    // 若 fd 无效，则 dup2()将返回-1， 并将 errno 置为 EBADF。
    if((fcntl(fd, F_GETFL, 0) == -1) && errno == EBADF)
    {
        printf("未占用的fd: %d.\n", fd);
        return -1;
    }
    return 0;
}

// 实现dup2() 系统调用
int my_dup2(int oldFd, int newFd)
{
    // sysconf()系统调用允许应用程序在运行时获得系统限制值
    if( newFd < 0 ||newFd > sysconf(_SC_OPEN_MAX)  || (is_file_descripter(newFd) == 0) )
    {
        printf("newFd out of range.\n"); 
        return -1;
    }
    
    // oldFd若和newFd相同，则返回newFd
    if(oldFd == newFd)
    {
        printf("newFd is equal to oldFd\n");
        return newFd;
    }
    
    // 若newFd被占用，先将其关闭
    if(is_file_descripter(newFd))
    {
        close(newFd);
    }
    
    // 检查oldFd的有效性
    if(is_file_descripter(oldFd) == -1)
    {
        printf("Bad oldFd");
        return -1;
    }
    
    int fd = fcntl(oldFd, F_DUPFD, newFd);
    
    // dup2函数是要复制到指定数字的，若没有复制到指定数字则返回失败
    return fd == newFd ? fd : -1;


}



int main(int argv, char * argc[])
{
    /*
    if(argv < 2 || strcmp(argc[1], "--help") == 0 )
    {
        printf("useage: .//%s filename numByte [x] \n", argc[1]);
    }
    */


    int fd = open("test.txt",O_RDWR | O_CREAT, S_IWUSR|S_IRUSR);
    printf("fd: %d: \n", fd);
    if(fd == -1)
    {
        printf("open test.txt error.\n");
        exit(EXIT_FAILURE);
    }

    int fd_dup = my_dup(fd);
    printf("fd_dup: %d\n", fd_dup);
    if(fd_dup == -1)
    {
        printf("my_dup error.\n");
        exit(EXIT_FAILURE);
    }

    int fd_dup2 = my_dup2(fd, 66);
    if(fd_dup2 == -1)
    {
        printf("my_dup2 error.\n");
        exit(EXIT_FAILURE);
    }
    printf("fd_dup2: %d\n", fd_dup2);
    char str_fd[] = "Hello I'm fd!\n";
    write(fd, str_fd, sizeof(str_fd));
    char tmp_dup[] = "Hello, I'm dup! my file descriptor is : ";
    char tmp_dup2[] = "Hello, I'm dup2! my file descriptor is : ";
    char * str_fd_dup = ( char * )malloc(sizeof(tmp_dup) + sizeof(int) + 1) ;
    char * str_fd_dup2 = ( char * )malloc(sizeof(tmp_dup2) + sizeof(int) + 1);

    sprintf(str_fd_dup, "%s%d\n", tmp_dup, fd_dup);
    sprintf(str_fd_dup2, "%s%d\n", tmp_dup2, fd_dup2);

    write(fd_dup, str_fd_dup, sizeof(tmp_dup) + sizeof(int));
    write(fd_dup2, str_fd_dup2, sizeof(tmp_dup2) + sizeof(int));
    if(str_fd_dup)
        free(str_fd_dup);
    if(str_fd_dup2)
        free(str_fd_dup2);
    printf("Success.\n");
    return 0;
}




