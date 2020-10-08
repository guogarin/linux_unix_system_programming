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


int set_nonblock(int fd)
{
    int old_flags = fcntl(fd, F_GETFL);
    int new_flags = old_flags | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_flags);
    return old_flags;
}


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

    int ret = bind(sockfd, (struct sockaddr *) &saddr sizeof(saddr));
    assert(ret != -1);

    ret = listen(sockfd, 5);
    assert(ret != -1);

    // Using nonblocking accept()
    set_nonblock(sockfd);

}

