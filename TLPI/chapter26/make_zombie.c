#include <signal.h>
#include <libgen.h> /* For basename() declaration */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


#define CMD_SIZE 200

int main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;

    setbuf(stdout, NULL); /* Disable buffering of stdout */

    switch(childPid = fork())
    {
        case -1:
            printf("errExit: fork\n");
            exit(EXIT_FAILURE);
        
        case 0:
            printf("Child (PID=%ld) exiting\n", (long) getpid());
            _exit(EXIT_SUCCESS);
        
        default:
            sleep(3);
            snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
            cmd[CMD_SIZE - 1] = '\0'; /* Ensure string is null-terminated */
            system(cmd); /* View zombie child */

            if(kill(childPid, SIGKILL) == -1)
            {
                printf("errMsg: kill\n");
                exit(EXIT_FAILURE);
            }

            sleep(3);
            printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
            system(cmd); /* View zombie child again */

            exit(EXIT_SUCCESS);

    }


}
