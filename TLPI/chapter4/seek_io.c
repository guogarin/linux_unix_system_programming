#include<sys/stat.h>
#include<fcntl.h>
#include<ctypes.h>
#include<stdio.h>
#include<unistd.h>
//#include<



int main(int argc, cont char * argv[])
{
    size_t len;
    off_t offset; //  SUSv3 规定 off_t 数据类型为有符号整型数。 
    int fd, ap, j;
    ssize_t numRead, numWritten;

    if(argc < 3 || strcmp(argv[1], "--help") == 0)
    {
        printf("\nUsageErro.\n");
        exit();
    }

    fd = open(argv[1], O_RDWR|O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IWOTH |S_IROTH );
    if(fd == -1)
    {
        printf("openErro\n");
        exit()
    }

    for(ap = 2; ap < argc;ap++)
    {
        switch(argv[ap][0])
        {
            case 'r':
            case 'R':
        }

    }
}
