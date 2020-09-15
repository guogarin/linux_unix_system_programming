#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
    {
        printf("usageErr: %s pathname\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    execlp(argv[1], argv[1], "hello world", (char *) NULL);
    printf("execlp\n");
    exit(EXIT_FAILURE);

}
