#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <erno.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <types.h>
#include <sys/ioctl.h>

#define BUF_SIZE 1023


int set_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL);
}


int unblock_connect(const *ip, int port, int time)
{
    int ret = 0;
    struct sockaddr_in address;
    bzereo(&address, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);




}
