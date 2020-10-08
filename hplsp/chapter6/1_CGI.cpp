#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
    if(argc <=2){
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return 1;
    }

    const char * ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET,ip, &saddr.sin_addr);
    saddr.sin_port   = htons(port);

    int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock_fd != -1);

    int ret = bind(sock_fd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_in));
    assert(ret != -1);

    ret = listen(sock_fd, 5);
    assert(ret != -1);
   
    struct sockaddr_in cli_addr;
    socklen_t cli_addr_len;
    int cli_fd = accept(sock_fd, (struct sockaddr *) &cli_addr, &cli_addr_len);
    if(cli_fd == -1){
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
    
    if(dup2(cli_fd, STDOUT_FILENO) == -1){
        close(sock_fd);
        close(cli_fd);
        exit(EXIT_FAILURE);
    }

    printf("Hello, I am CGI server\n");
    
    close(sock_fd);
    close(cli_fd);
    return 0;
}
