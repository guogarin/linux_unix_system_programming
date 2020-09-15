#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>


int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        printf("usageErr %s shm-name\n", argv[0]);
    
    int fd = shm_open(argv[1], O_RDWR, 0);
    if(fd == -1)
    {
        printf("shm_open\n");
        exit(EXIT_FAILURE);
    }
    struct stat sb;
    fstat(fd, &sb);
    char * addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(addr == MAP_FAILED)
    {
        printf("mmap\n");  
        exit(EXIT_FAILURE);   
    }
    
    if(close(fd) == -1)
    {
        printf("close\n");
        exit(EXIT_FAILURE);
    }
/*
    int len = sb.st_size;
    char str[len + 1];
    memcpy(str, addr, len);
    printf("%s", str);
*/
    write(STDOUT_FILENO, addr, sb.st_size);
    printf("\n");
    exit(EXIT_SUCCESS);
}

