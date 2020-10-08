#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>


int timeout_connect(const char*ip,int port,int time)
{
    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(saddr));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &saddr.sin_addr);
    saddr.sin_port = htons(port);

    struct timeval tv;
    tv.tv_sec  = time;
    tv.tv_usec = 0;
    
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(cfd != -1);

    int ret = setsockopt(cfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
    assert(ret != -1);
    
    printf("Starting connect\n");
    ret = connect(cfd, (struct sockaddr *) &saddr, sizeof(saddr));
    // Processing the return value of the connect.
    if(ret < 0){
        if(errno == EINPROGRESS){
            printf("connect timeout!\n");
            return -1;
        }
        printf("error occur when connecting to server\n");
        return-1;
    }
    printf("Connectiong success.\n");
    return cfd;
}

int main(int argc,char*argv[])
{
    if(argc <= 2){
		printf("usage:%s ip_address port_number\n",basename(argv[0]));
		return 1;
	}
	const char*ip=argv[1];
	int port=atoi(argv[2]);
	int sockfd=timeout_connect(ip,port,10);
	if(sockfd < 0){
		return 1;
	}
	return 0;
}


