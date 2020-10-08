#include <sys/epoll.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_BUF     1000
#define MAX_EVENTS  5

int main(int argc, char *argv[])
{
    int epfd, ready, fd, s, j numOpenFds;
    struct epoll_event ev;
    struct epoll_event evlist[MAX_EVENTS];
    char buf[MAX_BUF];

    if (argc < 2 || strcmp(argv[1], "--help") == 0){
        printf("usageErr: %s file...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    epfd = epoll_create(argc -1);
    if(epfd == -1){
        printf("epoll_create\n");
        exit(EXIT_FAILURE);
    }

    for(j = 0; j < argc; j++){
        fd = open(argv[j], O_RDONLY);
        if(fd == -1){
            printf("open argv[%d] failed.\n", j);
            exit(EXIT_FAILURE);
        }
        printf("Opened \"%s\" on fd %d\n", argv[j], fd);
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        if(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1){
            printf("EPOLL_CTL_ADD [%d] failed.\n", j);
            exit(EXIT_FAILURE);
        }
    }

    numOpenFds = argc -1;

    while( numOpenFds > 0){
        printf("About to epoll_wait()\n");
        ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
        if(ready == -1){
            if(errno == EINTR){
                continue;
            }else{
                printf("epoll_wait\n");
                exit(EXIT_FAILURE);
            }
        }

        printf("Ready: %d\n", ready);
        
        for(j = 0; j < ready, j++){
            printf(" fd=%d; events: %s%s%s\n", evlist[j].data.fd,
                        (evlist[j].events & EPOLLIN) ? "EPOLLIN " : "",
                        (evlist[j].events & EPOLLHUP) ? "EPOLLHUP " : "",
                        (evlist[j].events & EPOLLERR) ? "EPOLLERR " : "");

            if(evlist[j].events & EPOLLIN){
                s = read(evlist[j].data.fd, buf, MAX_BUF);
                if(s == -1){
                    printf("read evlist[%d]\n", j);
                    exit(EXIT_FAILURE);
                }
                printf(" read %d bytes: %.*s\n", s, s, buf);
            }else if(evlist[j].events & (EPOLLHUP | EPOLLERR)){
                printf(" closing fd %d\n", evlist[j].data.fd);
                if(close(evlist[j].data.fd) == -1){
                    printf("close fd %d failed\n", evlist[j].data.fd);
                    exit(EXIT_FAILURE);
                }
                numOpenFds--;
            }
        }
    }

    printf("All file descriptors closed; bye\n");
    exit(EXIT_SUCCESS);
}
