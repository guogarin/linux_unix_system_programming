#include<stdio.h>
#include<dirent.h>
#include<stdlib.h>
#include<errno.h>


static void listFiles(const char *dirpath)
{
    DIR *dirp;
    struct dirent *dp;
    size_t isCurrent;

    isCurrent = strcmp(".", dirpath);

    dirp = opendir(dirpath);
    if(dirp == NULL)
    {
        printf("opendir failed on '%s'", dirpath);
        return;
    }

    // 对于该目录下的每一项

    for(;;)
    {
        errno = 0;
        dp = readdir(dirp);
        if(dp == NULL)
            break;
        // 过滤 . 和 ..
        if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;
        if(!isCurrent)
            printf("%s/", dirpath);
        printf("%s\n"   , dp->d_name);

    }
}

int main(int argc, char* argv[])
{
    if (argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        printf("%s [dir...]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(argc == 1) // 无参数，则读取当前目录
        listFiles(".");
    else
        while(++argv != NULL)
            listFiles(*argv);

    exit(EXIT_SUCCESS);

}




