#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <netinet/in.h>


#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024


struct fds{
    int epollfd;
    int sockfd;
};

int set_noblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
    printf(" fd %d has been set as O_NONBLOCK.\n", fd);
    return flags;
}

void add_fd(int epollfd, int fd, bool oneshot)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    if(oneshot){
        ev.events |= EPOLLONESHOT;
    }
    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    if(ret){
        printf(" fd %d has been added to the epoll instance %d\n", fd, epollfd);
    }
    set_noblocking(fd);
}


// reset  event of the fd. To ensure that  the event will be trigered and only be trigered once.
void reset_oneshot(int epollfd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &ev);
    if(ret){
        printf(" fd %d has been reseted as EPOLLONESHOT\n", fd, epollfd);
    }
}


void *worker(void * args)
{
    int epollfd = ( (struct fds*) args) -> epollfd;
    int sockfd  = ( (struct fds*) args) -> sockfd;

    char buf[BUFFER_SIZE];
    memset(buf, 0, BUFFER_SIZE);
    
    int ret;
    while(1){
        ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
        if(ret == 0){
            printf("foreiner closed the connection\n");
            close(sockfd);
        }else if(ret < 0){
            if(errno == EAGAIN){
                printf("Read later.\n");
                reset_oneshot(epollfd, sockfd);
                break;
            }
        }else{
             printf("get content:%s\n",buf);
            /*休眠5s， 模拟数据处理过程*/
             sleep(5);
        }
    }
    
    printf("end thread receiving data on fd:%d\n",sockfd);
}


int main(int argc,char*argv[])
{ 
    if(argc <= 2){ 
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    
    const char * ip = argv[1];
    int port   = atoi(argv[2]);

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &saddr.sin_addr);
    saddr.sin_port = htons(port);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = bind(lfd, (struct sockaddr *) & saddr, sizeof(struct sockaddr_in));
    assert(ret != -1);
    ret = listen(lfd, 5);
    assert(ret != -1);
    
    struct epoll_event evlist[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    
    add_fd(epollfd, lfd, false);
    
    int num, i, readyFd, cfd;
    while(1){
        num = epoll_wait(epollfd, evlist, MAX_EVENT_NUMBER, -1);
        assert(num > 0);
        for(i = 0; i < num; i++){
            readyFd = evlist[i].data.fd;
            if(readyFd == lfd){
                cfd = accept(lfd, NULL, 0);
                // Connection fd should be set as EPOLLONESHOT.
                add_fd(epollfd, cfd, true);
            }else if(evlist[i].events & EPOLLIN){
                pthread_t thread;
                struct fds fds_new;
                fds_new.epollfd = epollfd;
                fds_new.sockfd = readyFd;
                pthread_create(&thread, NULL, worker, (void*) &fds_new);
            }else{
                printf("something else happened\n");
            }
        }
    }
    close(lfd);
    return 0;
}
