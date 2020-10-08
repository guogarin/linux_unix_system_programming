#define _GNU_SOURCE 1

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
#include <poll.h>
#include <fcntl.h>



int main(int argc,char*argv[])
{
    if(argc <= 2){
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return -1;
    }

    const char * ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);

    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &saddr.sin_addr);
    saddr.sin_port = htons(port);

    int con_ret = connect(sockfd, (struct sockaddr *)  &saddr, sizeof(saddr));
    assert(con_ret != -1);

    struct pollfd fds[2];

    // Adding the STDIN_FILENO into the fds;
    fds[0].fd    = STDIN_FILENO;
    fds[0].events = POLLIN;
    // Adding the connect socket into the fds;
    fds[1].fd    = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;
    
    int poll_ret, num;
    int pipeFds[2];
    int pipe_ret = pipe(pipeFds);
    assert(pipe_ret != -1);
    char buf[1024];
    while(1){
        poll_ret = poll(fds, 2, -1);
        assert(poll_ret != -1);
        if(fds[1].revents & POLLIN){
            memset(buf, 1024, 0);
            num = recv(sockfd, buf, 1024, 0);
            printf("%d bytes received: %s\n", num, buf);
        }else if(fds[0].revents & POLLIN){
            num = splice(STDIN_FILENO, NULL, pipeFds[1], NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
            num = splice(pipeFds[0], NULL,   sockfd,     NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE); 
        }else if (fds[1].revents & POLLRDHUP){
            printf("server close the connection\n");
            break;
        }

    }
    close(sockfd);
    return 0;
}


