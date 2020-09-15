#include<stdio.h>
#include<libgen.h>
#include<stdlib.h>


int main(int argc , char * argv[])
{
    char *t1, *t2;
    int j;

    for(j = 1; j < argc; j++)
    {
        t1 = strdup(argv[j]);
        if(t1 == NULL)
        {
            printf("strdup\n");
            exit(EXIT_FAILURE);
        }
        t2 = strdup(argv[j]);
        if(t2 == NULL)
        {
            printf("strdup\n");
            exit(EXIT_FAILURE);
        }

        printf("%s ==> %s + %s\n", argv[j],dirname(t1), basename(t2));

        free(t1);
        free(t2);
    }
    exit(EXIT_SUCCESS);

}
