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
#define BUF_SIZE 10


int set_nonblocking(int fd)
{
    int option = fcntl(fd, F_GETFL);
    option |= O_NONBLOCK;
    return option;
}


void add_fd(int epollfd, int fd, bool enable_et)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if(enable_et){
        ev.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    set_nonblocking(fd);
}

// the working process of LT pattern
void lt(struct epoll_event * evs, int num, int epollfd, int lfd)
{
    char buf[BUF_SIZE];
    int i;
    int ret, cfd;
    
    for(i = 0; i < num; i++){
        cfd = evs[i].data.fd;
        if(cfd == lfd){
            cfd = accept(lfd, NULL, 0);
            add_fd(epollfd, cfd, false);
        }else if(evs[i].events & EPOLLIN){
            printf("event trigger once\n");
            memset(buf, 0, BUF_SIZE);
            ret = recv(cfd, buf, BUF_SIZE - 1, 0);
            if(ret){
                printf("get %d bytes of content:%s\n",ret,buf);
            }else{
                close(cfd);
                continue;
            }
        }else{
            printf("Something went wrong!\n");
        }
    }
}

//The working process of ET pattern.
void et(epoll_event*evs, int num, int epollfd, int lfd)
{
    char buf[BUF_SIZE]; 
    int i, ret, cfd;
    for(i = 0; i < num; i++){
        cfd = evs[i].data.fd;
        if(cfd == lfd){
            cfd = accept(lfd, NULL, 0);
            add_fd(epollfd, cfd, true);
        }else if(evs[i].events & EPOLLIN){
            printf("event will only be triggered once.\n");
            memset(buf, 0, BUF_SIZE);
            while(1){
                ret = recv(cfd, buf, BUF_SIZE - 1, 0);
                if(ret <= 0){
                    if(errno == EAGAIN || errno == EWOULDBLOCK){
                        printf("All read\n");
                        break;
                    }
                }else if(ret = 0){
                    close(cfd);
                }else{
                    printf("get %d bytes of content:%s\n",ret,buf);
                }
            }
        }else{
            printf("something else happened\n");
        }
    }
}


int main(int argc,char*argv[])
{
    if(argc <= 2){
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return 1;
    }   
    
    const char * ip = argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &saddr.sin_addr);
    saddr.sin_port = htons(port);

    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    int ret = bind(lfd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_in));
    assert(ret != -1);
    ret = listen(lfd, 5);
    assert(ret != -1);
    
    // The following is epoll
    struct epoll_event evs[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    add_fd(epollfd, lfd, true);
    while(1){
        ret = epoll_wait(epollfd, evs, MAX_EVENT_NUMBER, -1);
        if(ret == -1){
            printf("epoll failure\n");
            break;
        }
        lt(evs, ret, epollfd, lfd);
        //et(evs, ret, epollfd, lfd);
    }
    
    close(lfd);
    return 0;
}
