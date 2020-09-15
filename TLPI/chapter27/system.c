#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>



int system(const char *command)
{
    sigset_t blokMask, origMask;
    struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
    pit_t childPid;
    int status, savedErrno;

    if(command == NULL)
        return system(":") == 0;
    sigemptyset(&blokMask);
    sigaddset(&blokMask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &blokMask, origMask);//阻塞SIGCHLD


    saIgnore.sa_handler = SIG_IGN;// 忽略SIGINT 和 SIGQUIT
    saIgnore.sa_flags   = 0;
    sigemptyset(&saIgnore.sa_mask); /*仅当 sa_handler 是信号处理程序的地址时，亦即 
                                      sa_handler的取值在 SIG_IGN 和 SIG_DFL 之外，
                                      才会对 sa_mask 和 sa_flags 字段（稍后讨论）加以处理。*/
    sigaction(SIGINT,  &saIgnore, &saOrigInt);
    sigaction(SIGQUIT, &saIgnore, &saOrigQuit);


    switch(childPid = fork())
    {
        case -1:
            status = -1;
            break;

        case 0: // 子进程执行
            saDefault.sa_handler = SIG_DFL;
            saDefault.sa_flags = 0;
            sigemptyset(&saDefault.sa_mask);

            if(saOrigInt.sa_handler != SIG_IGN)
                sigaction(SIGINT, &saDefault, NULL);
            if(saOrigQuit.sa_handler != SIG_IGN)
                sigaction(SIGQUIT, &saDefault, NULL);
            
            sigprocmask(SIG_SETMASK, &origMask, NULL);

            excel("bin/sh","sh", "-c", command, (char*) NULL);
            _exit(127);

        default: //父进程执行
            while(waitpid(childPid, &status, 0) == -1)
            {
                if(errno == EINTR)
                {
                    status = -1;
                    break;
                }
            }
            break;
    }

    savedErrno = errno;

    sigprocmask(SIG_SETMASK, &origMask, NULL);
    sigaction(SIGINT,  &saOrigInt,  NULL);
    sigaction(SIGQUIT, &saOrigQuit, NULL);

    errno = savedErrno;
    
    return status;
    
}
