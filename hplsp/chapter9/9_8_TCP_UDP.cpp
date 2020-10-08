#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024


int setnonblocking(int fd)
{
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addfd(int epollfd,int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = fd;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    setnonblocking(fd);
}


int main(int argc,char*argv[])
{
    if(argc <= 2){
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return -1;
    }

    const char * ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &saddr.sin_addr);
    saddr.sin_port = htons(port);
    
    // Creating a TCP socket, bind and listen.
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(lfd != -1);
    int ret = bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));
    assert(ret != -1);
    ret = listen(lfd, 5);
    assert(ret != -1);
    
    // Creating a UDP socket and bind.
    int udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    assert(udpfd != -1);
    ret = bind(udpfd, (struct sockaddr *)&saddr, sizeof(saddr));
    assert(ret != -1);

    struct epoll_event evlist[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(5);
    if(epollfd == -1){
        printf("epoll_create failedl.\n");
        exit(EXIT_FAILURE);
    }
    
    addfd(epollfd, lfd);
    addfd(epollfd, udpfd);

    int ready, cfd, num;
    char udpbuf[UDP_BUFFER_SIZE];
    char tcpbuf[TCP_BUFFER_SIZE];
    struct sockaddr_in caddr;
    while(1){
        ready = epoll_wait(epollfd, evlist, MAX_EVENT_NUMBER, -1);
        if(ready == -1){
            printf("epoll_wait failed\n");
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < ready; i++){
            if(evlist[i].data.fd == lfd){
                cfd = accept(lfd, NULL, 0);
                addfd(epollfd, cfd);
            }else if(evlist[i].data.fd == udpfd){
                bzero(&caddr, sizeof(saddr));
                memset(udpbuf,0, UDP_BUFFER_SIZE);
                socklen_t length = sizeof(saddr);
                num = recvfrom(udpfd, udpbuf, UDP_BUFFER_SIZE, 0, (struct sockaddr*)&caddr, &length);
                assert(num > 0);
                printf("UDP read: %s\n", udpbuf);
                num = sendto(udpfd, udpbuf, UDP_BUFFER_SIZE, 0, (struct sockaddr*)&caddr, sizeof(saddr));
                assert(num > 0);
            }else if(evlist[i].events & EPOLLIN){
                while(1){// Because it's ET pattern, so we should try to read the contents as many as possible.
                    memset(tcpbuf, 0, TCP_BUFFER_SIZE);
                    num = recv(evlist[i].data.fd, tcpbuf, TCP_BUFFER_SIZE, 0);
                    if(num < 0){
                        if(errno == EAGAIN | errno == EWOULDBLOCK){
                            break;
                        }
                        close(evlist[i].data.fd);
                        break;
                    }else if(num == 0){
                        close(evlist[i].data.fd);
                    }else{
                        num = send(evlist[i].data.fd, tcpbuf, TCP_BUFFER_SIZE, 0);
                        printf("TCP read: %s\n", tcpbuf);
                    }
                }
            }else{
                printf("Something  happened\n");
            }
        }
    }
    
    close(lfd);
    close(udpfd);
    return 0;
}
