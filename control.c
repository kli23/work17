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

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main( int argc, char *argv[] ) {

    int semd,shmd,file,r;


    if (argc != 2)
        printf("Provide one argument\n");


    else if (!strcmp(argv[1],"create")) {

        file = open("telephone.txt", O_TRUNC | O_CREAT, 0644); //creates new file
        if (file == -1) {
            printf("file error %d: %s\n", errno, strerror(errno));
            return 0;
        }
        shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0640); //creates shared memory segment
        semd = semget(KEY + 1, 1, IPC_CREAT | IPC_EXCL | 0644); //creates semaphore

        union semun us; //initializes semaphore to 1
        us.val = 1;
        r = semctl(semd, 0, SETVAL, us);
        return 0;

    }
    

    else if (!strcmp(argv[1],"remove")) {

        shmd = shmget(KEY, 0, 0); //allows us to remove semaphore and shared memory at the end
        semd = semget(KEY+1, 1, 0);


        struct stat s; //prints out story from the file
        file = open("telephone.txt", O_RDONLY);
        if (file == -1) {
            printf("file error %d: %s\n", errno, strerror(errno));
            return 0;
        }
        stat("telephone.txt", &s);
        char* story = malloc(s.st_size);
        read(file, story, s.st_size);
        printf("The story: \n%s\n", story);
        free(story);


        shmctl(shmd, IPC_RMID, 0);
        semctl(semd, IPC_RMID, 0);

        
        return 0;
    }

    else   
        printf("Provide a correct argument, create or remove\n");

    return 0;
}