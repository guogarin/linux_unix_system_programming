#include <syslog.h>
#include <stdio.h>
#include "id_echo.h"
#include <errno.h>
int main(int argc, char *argv[])
{
    int fd;
    socklen_t addrLen, len;
    ssize_t numRead;
    struct sockaddr_storage addr;
    char BUF[BUF_SIZE];
    char addrStr[IS_ADDR_STR_LEN];
    
    if(becomeDaemon(0) == -1)
    {
        printf("becomeDaemon\n");
        exit(EXIT_FAILURE);
    }
    
    if( (fd = inetBind(SERVICE, SOCK_DGRAM, &addrLen)) == -1)
    {
        syslog(LOG_ERR, "Could not create server socket (%s)", strerror(errno));
        exit(EXIT_FAILURE);
    }

    len = sizeof(struct sockaddr_storage);
    for(;;)
    {
        if( (numRead = recvfrom(fd, BUF, BUF_SIZE, 0, (struct sockaddr *) & addr, &len) == -1) )
        {
            syslog(LOG_ERR, "recvfrom failed");
            continue;
        }
        
        if( sendto(fd, BUF,  numRead, 0, (struct sockaddr*) & addr, len) != numRead)
        {
            syslog(LOG_WARNING, "Error echoing response to %s (%s)",
                    inetAddressStr((struct sockaddr *) &addr, len, addrStr, IS_ADDR_STR_LEN),
                    strerror(errno));
            continue;
        }
    }
}
