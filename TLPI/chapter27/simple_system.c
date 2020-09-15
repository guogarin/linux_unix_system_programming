#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int system(char *command)
{
    int status;
    pid_t childPid;
    
    switch(childPid = fork())
    {
        case -1:
            return -1;
        
        case 0: //子进程
            execl("/bin/sh", "sh", "-c", "command", (char *) NULL);
        
        default://父进程
            /* 注意：wait(&status)的调用与 waitpid(-1, &status, 0)等价，
             * 也就是说 waitpid(childPid, &status, 0) 是阻塞的，因为他没有指定 WNOHANG 标志。*/
            if(waitpid(childPid, status, 0) == -1)
                return -1;
            else
                return status;
    }

}
