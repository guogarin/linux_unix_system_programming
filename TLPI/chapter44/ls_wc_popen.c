#include <unistd.h>
#include <stdio.h>



int main(int argc, char *argv[])
{
    FILE * fpr;

    fpr = popen("ls -al", "w");

    char buf[4096];
    fgets(buf, 4096, fpr);
    printf("result:\n%s", buf);
    pclose(fpr);
}
