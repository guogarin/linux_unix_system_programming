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
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        printf("usageErr: %s shm-name string\n", argv[0]);

    int fd, flags;
    size_t size;


    fd = shm_open(argv[1], O_RDWR, 0);
    if(fd == -1)
    {
        printf("shm_open\n");
        exit(EXIT_FAILURE);
    }

    size = strlen(argv[2]);
    if(ftruncate(fd, size) == -1)
    {
        printf("ftruncate\n");
        exit(EXIT_FAILURE);
    }
    printf("Resized to %ld bytes\n", (long) size);

    char * addr = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
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

    printf("copying %ld bytes\n", (long) size);
    memcpy(addr, argv[2], size);
    exit(EXIT_SUCCESS);
}
