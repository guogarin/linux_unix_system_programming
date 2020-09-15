#include "svmsg_file.h"

static int clientId;

static void removeQueue(void)
{
    if(msgctl(clientId, IPC_RMID, NULL) == -1)
        printf("msgctl - IPC_RMID\n");
}


int main(int argc, char *argv[])
{
    struct requestMsg  req;
    struct responseMsg resp;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("%s pathname\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (strlen(argv[1]) > sizeof(req.pathname) - 1)
    {
        printf("pathname too long (max: %ld bytes)\n",
                (long) sizeof(req.pathname) - 1);
        exit(EXIT_FAILURE);
    }
    
    int serverId;

    //snprintf(req.pathname, sizeof(req.pathname), "%s", argv[1]);
    strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
    req.pathname[sizeof(req.pathname) - 1] = '\0';

    serverId = msgget(SERVER_KEY, S_IWUSR);
    if(serverId == -1)
    {
        printf("serverId\n");
        exit(EXIT_FAILURE);
    }

    clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
    if(clientId == -1)
    {
        printf("serverId\n");
        exit(EXIT_FAILURE);
    }
    
    if(atexit(removeQueue) != 0)
    {
        printf("atexit\n");
        exit(EXIT_FAILURE);
    }

    req.mtype = 1;
    req.clientId = clientId;
    if(msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
    {
        printf("msgsnd failed when send message to the serverId for the first time \n");
        exit(EXIT_FAILURE); // TODO：第一次就发送失败要退出？
    }

    ssize_t numRead, numTotal;
    if( ( numRead = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0) ) == -1)
    {
        printf("msgrcv 1\n");
        exit(EXIT_FAILURE);
    }

    if(resp.mtype == RESP_MT_FAILURE)
    {
        printf("recevied: RESP_MT_FAILURE\n");
        msgctl(clientId, IPC_RMID, NULL);
        exit(EXIT_FAILURE);
    }

    numTotal = numRead;
    int numMsgs = 0;
    for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs++) 
    {
        numRead = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
        if(numRead == -1)
        {
            printf("msgrcv 2\n");
            exit(EXIT_FAILURE);
        }
        numTotal += numRead;
    }
    
    printf("Received %ld bytes (%d messages)\n", (long) numTotal, numMsgs);

    exit(EXIT_SUCCESS);
}
