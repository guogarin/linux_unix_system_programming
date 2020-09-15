#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "become_daemon.h"

int becomeDaemon(int flags)
{
    int maxfd, fd;

    switch(fork())
    {
        case -1: return -1;
        case 0 : break;
        default: _exit(EXIT_SUCCESS);
    }

    if(setsid() == -1)
        return -1;

    switch (fork()) /* Ensure we are not session leader */ 
    {
        case -1: return -1;
        case 0: break;
        default: _exit(EXIT_SUCCESS);
    }

    if (!(flags & BD_NO_UMASK0))
        umask(0); /* Clear file mode creation mask */

    if (!(flags & BD_NO_CHDIR))
        chdir("/"); /* Change to root directory */

    if (!(flags & BD_NO_CLOSE_FILES)) { /* Close all open files */
        maxfd = sysconf(_SC_OPEN_MAX);
        if (maxfd == -1) /* Limit is indeterminate... */
            maxfd = BD_MAX_CLOSE; /* so take a guess */
        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if(!(flags & BD_NO_REOPEN_STD_FDS)) 
    {
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);

        if(fd != dup2(STDIN_FILENO, fd))
            return -1;
        if(fd != dup2(STDOUT_FILENO, STDIN_FILENO))
            return -1;
        if(fd != dup2(STDERR_FILENO, STDIN_FILENO))
            return -1;
    }

    return 0;
}
