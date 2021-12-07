#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define KEY 24602

int main() {
    char s[100];




    printf("Next line? ");
    fgets(s,100,stdin);

    return 0;
}