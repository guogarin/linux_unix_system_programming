#include "i6d_ucase.h"

int main(int argc, char *argv[])
{
    struct sockaddr_in6 saddr, caddr;
    int sfd;
    char buf[BUF_SIZE];
    size_t numRead;

    if( (sfd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1 )
    {
        printf("socket\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&saddr, 0, sizeof(struct sockaddr_in6));
    saddr.sin6_family = AF_INET6;
    saddr.sin6_addr   = in6addr_any;
    saddr.sin6_port   = htons(PORT_NUM);

    if(bind(sfd, (struct sockaddr *) &saddr, sizeof(struct sockaddr_in6)) == -1)
    {
        printf("bind\n");
        exit(EXIT_FAILURE);
    }
    
    int j;
    socklen_t len = sizeof(struct sockaddr_in6);
    char caddrStr[INET6_ADDRSTRLEN];
    for(;;)
    {
        numRead = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &caddr, &len);
        
        if(numRead == -1)
        {
            printf("recvfrom\n");
            exit(EXIT_FAILURE);
        }

        if(inet_ntop(AF_INET6, &caddr.sin6_addr, caddrStr, INET6_ADDRSTRLEN) == NULL)
            printf("Couldn't convert client address to string\n");
        else
            printf("Server received %ld bytes from (%s, %u)\n",
                    (long) numRead, caddrStr, ntohs(caddr.sin6_port));

        for(j = 0; j < numRead; j++)
            buf[j] = toupper( (unsigned char) buf[j]);

        if(sendto(sfd, buf, numRead, 0, (struct sockaddr *) &caddr, len) != numRead)
        {
            printf("sendto\n");   
            exit(EXIT_FAILURE);   
        }

    }

}
