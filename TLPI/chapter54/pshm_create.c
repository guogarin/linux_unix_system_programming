#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>




static void usageError(const char *progName)
{
    fprintf(stderr, "Usage: %s [-cx] name size [octal-perms]\n", progName);
    fprintf(stderr, " -c Create shared memory (O_CREAT)\n");
    fprintf(stderr, " -x Create exclusively (O_EXCL)\n");
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    int fd, prot, flags, opt, perms;
    void * addr;
    mode_t mode;
    size_t size;

    flags = 0;
    while( (opt = getopt(argc ,argv, "cx") ) != -1)
    {
        switch(opt)
        {
            case 'c':
                flags |= O_CREAT;
                break;

            case 'x':
                flags |= O_EXCL;
                break;

            default:
                usageError(argv[0]);
        }
    }
    
    printf("After while loop\n");
    if(optind + 1 >= argc)
        usageError(argv[0]);
    
    size = atoi(argv[optind + 1]);
   
    printf("Before perms\n");
    perms = (argc > optind + 2) ? atoi(argv[optind + 2]) : (S_IRUSR | S_IWUSR);
    printf("Afterperms\n");

    fd = shm_open(argv[optind], flags | O_RDWR, perms);

    if(ftruncate(fd, size) == -1)
    {
        printf("ftruncate\n");
        exit(EXIT_FAILURE);
    }

    if( (addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0) == MAP_FAILED) )
    {
        printf("mmap\n");
        exit(EXIT_FAILURE);     
    }

    exit(EXIT_SUCCESS);
}
