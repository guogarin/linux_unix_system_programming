#include<stdio.h>
#include<unistd.h>
#include<errno.h>


static void sysconfPrint(const char * msg, int name)
{
    long lim;

    errno = 0;
    lim = sysconf(name);
    if(lim != -1) // 调用成功
    {
        printf("%s %ld\n ", msg, lim);
    }
    else
    {
        if(errno == 0) // 调用成功，但该限制名未定义
        {
            printf("%s (indeterminate)\n", msg);
        }
        else
        {
            printf("sysconf erro: %s" , msg);
            exit(-1);
        }

    }
}


int main(int argc, char * argv[])
{
    sysconfPrint("_SC_ARG_MAX: ", _SC_ARG_MAX);
    sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
    sysconfPrint("_SC_OPEN_MAX: ", _SC_OPEN_MAX);
    sysconfPrint("_SC_NGROUPS_MAX: ", _SC_NGROUPS_MAX);
    sysconfPrint("_SC_PAGESIZE: ", _SC_PAGESIZE);
    sysconfPrint("_SC_RTSIG_MAX: ", _SC_RTSIG_MAX);
    exit(0); 
}
