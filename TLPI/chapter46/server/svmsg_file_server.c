#include "svmsg_file.h"

// SIGCHLD 的处理函数
static void grimReaper(int sig)
{
    int savedErrno = errno;//waitpid()可能会修改 errno 变量
    while(waitpid(-1, NULL, WNOHANG) > 0)
    {
        continue;
    }
    errno = savedErrno;
}

static void serveRequest(struct requestMsg *req)
{
    struct responseMsg resp;
    int fd = open(req->pathname, O_RDONLY);
    if(fd == -1)
    {
        resp.mtype = RESP_MT_FAILURE;
        snprintf(resp.data, RESP_MSG_SIZE, "file %s couldn't open\n", req->pathname);
        if(msgsnd(req->clientId, &resp, REQ_MSG_SIZE, 0) == -1)
        {
            printf("msgsnd 1\n");
            exit(EXIT_FAILURE);
        }
        return;
    }
    
    resp.mtype = RESP_MT_DATA;
    int count = 0;
    ssize_t numRead;
    while((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)
    {
        printf("Send success %d\n", count++);
        if( msgsnd(req->clientId, &resp, numRead, 0) == -1)
        {
            printf("msgsnd 2\n");
            break;
        }
    }

    resp.mtype = RESP_MT_END;
    snprintf(resp.data, RESP_MSG_SIZE, "file %s is completed\n", req->pathname);
    if(msgsnd(req->clientId, &req, REQ_MSG_SIZE, 0) == -1)
    {
        printf("msgsnd 3\n");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
    struct sigaction sa;
    sa.sa_flags   = SA_RESTART;
    sa.sa_handler = grimReaper;
    if(sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        printf("sigaction\n");
        exit(EXIT_FAILURE);
    }

    int serverMSG = msgget(SERVER_KEY, IPC_CREAT | 
            S_IRUSR | S_IWUSR | S_IWGRP);
    if(serverMSG == -1)
    {
        printf("msgget\n");
        exit(EXIT_FAILURE);
    }
    
    struct requestMsg req;
    ssize_t numRead;
    pid_t pid;
    printf("Before for loop\n");
    for(;;)
    {
        numRead = msgrcv(serverMSG, &req, REQ_MSG_SIZE, 0, 0); 
        if(numRead == -1)
        {
            /* msgrcv()在被堵塞(即未指定IPC_NOWAIT标识)时，
             * 若被信号处理器中断，那么msgrcv()调用会失败并返回 EINTR 错误，
             * 不管在建立信号处理器时是否设置了 SA_RESTART 标记。*/
            if(errno == EINTR) 
            {
                continue;
            }
            else
            {
                printf("msgrcv\n");
                break; // 此时终止服务
            }
        }
        printf("Received: mtype: %ld, clientId: %d, pathname: %s", req.mtype, req.clientId, req.pathname);
        pid = fork();
        switch(pid)
        {
            case -1:
                printf("fork erro \n");
                continue;

            case 0:
                printf("Before serveRequest\n");
                serveRequest(&req);
                _exit(EXIT_SUCCESS);
            default:
                continue;
        }
    }

    if(msgctl(serverMSG, IPC_RMID, NULL) == -1)
        printf("msgctl\n");
    exit(EXIT_SUCCESS);
}
