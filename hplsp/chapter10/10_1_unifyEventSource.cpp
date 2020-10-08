#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024

static int pipefd[2];

int setnonblocking(int fd)
{
    int old_flags = fcntl(fd, F_GETFL);
    int new_flags = old_flags | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_flags);
    return old_flags;
}


void add_fd(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN |EPOLLET;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, &ev);
    assert(ret != -1);
    setnonblocking(fd);
}


void sig_handler(int sig)
{
    /*  Saving the erro number in the beginning of the signal handler function 
     *  to ensure the reentrance of the function.*/
    int saved_errno = errno;
    int msg = sig;
    /* Writing the signal number into the pipe. */
    int num = write(pipefd[1], (char *) &msg, 1);
    assert(num == 1);
    /* Restoring the errno in the end of the function*/
    errno = saved_errno;
}


void add_sig(int sig)
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    int ret = sigaction(sig, &sa, NULL);
    assert(ret != -1);
}


int main(int argc,char*argv[])
{ 
    if(argc <= 2){ 
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    
}
