#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc,char*argv[])
{
    if(argc <= 2){
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return 1;
    }

    const char * ip = argv[1];
    int port        = atoi(argv[2]);

    int ret;

    struct sockaddr_in saddr;
    bzero(&saddr, sizeof(struct sockaddr_in));
    saddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &saddr.sin_addr);
    saddr.sin_port = htons(port);

    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(lfd != -1);
    
    ret = bind(lfd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_in));
    assert(ret != -1);

    ret = listen(lfd, 5);
    assert(ret != -1);

    int cfd = accept(lfd, NULL, 0);
    if(cfd != -1){
        int fds[2];
        ret = pipe(fds);
        assert(lfd != -1);

        ret=splice(cfd,NULL,fds[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret != -1);
        ret=splice(fds[0],NULL,cfd,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
        assert(ret != -1);
    }else{
        printf("accept failed\n");
        close(lfd);
        exit(EXIT_FAILURE);
    }

    close(cfd);
    close(lfd);
    return 0;
}
