#include<stdio.h>
#include<unistd.h>
#include<errno.h>




static void fpathconfPrint(const char * msg, int fd, int name)
{
    long lim;

    errno = 0;
    lim = fpathconf(fd, name);
    if(lim != -1) // 调用成功
    {
        printf("%s %ld\n ", msg, lim);
    }
    else
    {
        if(errno == 0) // 调用成功，但该限制名未定义
        {
            printf("%s (indeterminate)\n", msg);
        }
        else
        {
            printf("sysconf erro: %s" , msg);
            exit(-1);
        }

    }
}







int main()
{
    fpathconfPrint("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);
    fpathconfPrint("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);
    fpathconfPrint("_PC_PIPE_BUF: ", STDIN_FILENO, _PC_PIPE_BUF);
    exit(0);
}
