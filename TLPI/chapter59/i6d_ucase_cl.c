#include "i6d_ucase.h"

int main(int argc, char *argv[])
{
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        printf("usageErr: %s host-address msg...\n", argv[0]);
    
    struct sockaddr_in6 caddr;
    int cfd, i;

    if( (cfd = socket(AF_INET6, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket\n");
        exit(EXIT_FAILURE);
    }
    
    memset(&caddr, 0, sizeof(struct sockaddr_in6));
    caddr.sin6_family = AF_INET6;
    caddr.sin6_port   = htons(PORT_NUM);
    if(inet_pton(AF_INET6, argv[1], &caddr.sin6_addr) < 0)
        printf("inet_pton failed for address '%s'", argv[1]);

    size_t numRead, msgLen;
    char rsp[BUF_SIZE];
    for(i = 2; i < argc; i++)
    {
        msgLen = strlen(argv[i]);
        if( sendto(cfd, argv[i], msgLen, 0, (struct sockaddr *) &caddr, sizeof(struct sockaddr_in6)) != msgLen)
        {
            printf("sendto\n");
            exit(EXIT_FAILURE);
        }

        numRead = recvfrom(cfd, rsp, BUF_SIZE, 0, NULL, NULL);
        if(numRead == -1)
        {
            printf("recvfrom\n");
            exit(EXIT_FAILURE);
        }

        printf("Response %d: %.*s\n", i - 1, (int) numRead, rsp);
    }

    exit(EXIT_SUCCESS);
}
