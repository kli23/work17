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
    char sentence[500];
    int semd, shmd, file;
    int *linesize;
    char pastsent[500];

    shmd = shmget(KEY, 0, 0);
    semd = semget(KEY+1, 1, 0); //get access to sem
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;

    semop(semd, &sb, 1); //attempts to take semaphore
    linesize = shmat(shmd,0,0); //attaches segment to linesize variable

    struct stat st;
    file = open("telephone.txt", O_RDONLY); //checks if file is empty
    stat("telephone.txt", &st);
    if (st.st_size) //if not empty 
    {
        lseek(file, -*linesize, SEEK_END); //set the position in the file to however many bytes from the end of the file
    }
    read(file, pastsent, *linesize);
    printf("Previous line: %s\n", pastsent);


    printf("Next line? "); //prompts user, appends next line to file
    fgets(sentence, 500,stdin);
    file = open("telephone.txt", O_WRONLY | O_APPEND);
    write(file, sentence, strlen(sentence));
    *linesize = strlen(sentence);
    shmdt(linesize); //assign linesize and detach segment from variable




    sb.sem_op = 1; // set operation to up
    semop(semd, &sb, 1);
    return 0;
}