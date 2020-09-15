#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


static volatile int numLiveChildren = 0;


static void sigchldHandler(int sig)
{
   int status, savedErrno;
   pid_t childPid;

   savedErrno = errno;

   printf("%s handler: Caught SIGCHLD\n", currTime("%T"));

   while((childPid = waitpid(-1, &status, WNOHANG)) > 0)
   {
        printf("%s handler: Reaped child %ld - ", currTime("%T"),
                (long) childPid);
        printWaitStatus(NULL, status);
        numLiveChildren--;
   }

   if (childPid == -1 && errno != ECHILD)
       errMsg("waitpid");

   sleep(5); /* Artificially lengthen execution of handler */

   printf("%s handler: returning\n", currTime("%T"));
   errno = savedErrno;

}

int main(int argc, char *argv[])
{
    
}
