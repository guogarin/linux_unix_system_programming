#include <sys/un.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#define SV_SOCK_PATH "/tmp/us_xfr"

#define BUF_SIZE 100
