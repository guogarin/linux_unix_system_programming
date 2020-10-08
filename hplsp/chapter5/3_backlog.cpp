#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>

static bool stop = false;

static void handle_term(int sig)
{
    stop = true;
}

int main(int argc,char*argv[])
{
    if(argc <= 3){
        printf("usage:%s ip_address port_number backlog\n",basename(argv[0]));
        exit(EXIT_FAILURE);
    }

    int ret;

    const char * ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);
    
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    assert(fd != -1);

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ret = inet_pton(AF_INET, ip, &addr.sin_addr);
    assert(ret == 1);

    ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret != -1);

    ret = listen(fd, backlog);
    assert(ret != -1);

    while(!stop){
        sleep(1);
    }

    close(fd);
    return 0;
}

