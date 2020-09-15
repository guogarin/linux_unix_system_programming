#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "inet_sockets.h" /* Declares functions defined here */


int inetConnect(const char *host, const char *service, int type)
{
    struct addrinfo hints;
    struct addrinfo * result, * rp;
    int cfd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;
    hints.ai_addr = NULL;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = type;

    if( getaddrinfo(host, service, &hints, &result) != 0)
    {
        errno = ENOSYS;
        printf("getaddrinfo\n");
    }

    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        if( (cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 )
        {
            printf("socket in for loop\n");
            continue; 
        }
    
        if(connect(cfd, rp->ai_addr, rp->ai_addrlen) == -1)
        {
            printf("Connect failed, try another address.\n");
            close(cfd);
            continue; 
        }
        else
            break;
    }
    
    freeaddrinfo(result);

    if(rp == NULL)
    {
        printf("Cannot connect any address till the end of the resluts returned by getaddrinfo function\n");
        return -1;
    }

    return cfd;
}


static int /* Public interfaces: inetBind() and inetListen() */
inetPassiveSocket(const char *service, int type, socklen_t *addrlen, Boolean doListen, int backlog)
{
    struct addrinfo hints;
    struct addrinfo * result, * rp;
    int sfd, optval;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_next = NULL;
    hints.ai_addr = NULL;
    hints.ai_family   = AF_UNSPEC;
    hints.ai_socktype = type;
    hints.ai_flags    = AI_PASSIVE;

    if( getaddrinfo(NULL, service, &hints, &result) != 0)
    {
        printf("getaddrinfo\n");
        return -1;
    }

    optval = 1;
    for(rp = result; rp != NULL; rp = rp->ai_next)
    {
        if( (sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 )
        {
            printf("socket in for loop\n");
            continue;
        }

        if(doListen)
        {
            if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) 
            {
                close(sfd);
                freeaddrinfo(result);
                return -1;
            }
        } 
        if(bind(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;
        close(sfd);
    }

    if(rp == NULL)
    {
        freeaddrinfo(result);
        return -1;
    }

    if(doListen)
    {
        if(listen(sfd, backlog) == -1)
        {
            freeaddrinfo(result);
            return -1;
        }
    }

    if(addrlen != NULL)
        *addrlen = rp->ai_addrlen; // Retuen address length.

    freeaddrinfo(result);
    return sfd;
}


int inetListen(const char *service, int backlog, socklen_t *addrlen)
{
    return inetPassiveSocket(service, SOCK_STREAM, addrlen, TRUE, backlog);
}


int inetBind(const char *service, int type, socklen_t *addrlen)
{
    return inetPassiveSocket(service, type, addrlen, FALSE, 0);
}


char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen)
{
    char host[NI_MAXHOST], service[NI_MAXSERV];
    if(getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV) == 0)
        snprintf(addrStr, addrStrLen, "(%s, %s)", host, service);
    else
        snprintf(addrStr, addrStrLen, "%s", "(?UNKNOWN?)");
    
    addrStr[addrStrLen - 1] = '\0';

    return addrStr;
}
