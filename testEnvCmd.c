#include "message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int main(){

    int fd;
    char *tubenomme = "/tmp/example.fifo";

    int res = mkfifo(tubenomme, S_IRWXU);

    if(res == -1){
        perror("mkfifo");
        exit(1);
    }

    fd = open(tubenomme, O_RDWR);

    if(fd == -1){
        perror("open");
        exit(2);
    }

    //création des cmd un à un
    struct cmd cmdTest;

    cmd_create(&cmdTest, 3);

    cmdTest.nameAndArgs[0] = "ls";
    cmdTest.nameAndArgs[1] = "../";

    cmdTest.date = 1548567868;
    cmdTest.periode = 5;

    struct cmd cmdTest2;

    cmd_create(&cmdTest2, 5);

    cmdTest2.nameAndArgs[0] = "echo";
    cmdTest2.nameAndArgs[1] = "bonjour";
    cmdTest2.nameAndArgs[2] = "ça va";
    cmdTest2.nameAndArgs[3] = "toi";

    cmdTest2.date=10;
    cmdTest2.periode=0;

    //création de la list de cmd
    struct array lcmd;

    array_create(&lcmd);

    //ajout des cmd dans la list das cmd
    array_add(&lcmd, cmdTest);

    printf("capacity list cmd = %ld\n", lcmd.capacity);
    printf("size list cmd =%ld\n",lcmd.size);

    array_add(&lcmd, cmdTest2);

    printf("capacity list cmd = %ld\n", lcmd.capacity);
    printf("size list cmd =%ld\n",lcmd.size);

    //affichage de array cmd
    for(size_t i = 0; i < lcmd.size; i++){

        size_t j = 0;
        while(lcmd.listCmd[i].nameAndArgs[j] != NULL){

            printf("%s ", lcmd.listCmd[i].nameAndArgs[j]);

            j++;
        }

        printf("\n");

        printf("date = %ld\n",lcmd.listCmd[i].date);
        printf("perido = %ld\n",lcmd.listCmd[i].periode);

        printf("\n");
    }

    //send string in the named pipe
    printf("Send cmd :\n");

    //envoyer le size du tableau

    sleep(3);

    //send capacity
    ssize_t writeCap = write(fd, &lcmd.capacity, sizeof(size_t));

    if(writeCap == -1){

        fprintf(stderr,"The date of the cmd can't be write in the pipe\n");
        exit(-1);

    }

    sleep(3);

    //send size
    ssize_t writeSize = write(fd, &lcmd.size, sizeof(size_t));

    if(writeSize == -1){

        fprintf(stderr,"The date of the cmd can't be write in the pipe\n");
        exit(-1);

    }

    sleep(3);

    //send array of char * of the cmd and its arg
    for(size_t i = 0; i < lcmd.size; i++){

        int resSend = send_argv(fd, lcmd.listCmd[i].nameAndArgs);

        ssize_t writeDate = write(fd, &lcmd.listCmd[i].date, sizeof(size_t));

        if(writeDate == -1){

            fprintf(stderr,"The date of the cmd can't be write in the pipe\n");
            exit(-1);

        }

        ssize_t writePeriod = write(fd, &lcmd.listCmd[i].periode, sizeof(size_t));

        if(writePeriod == -1){

            fprintf(stderr,"The date of the cmd can't be write in the pipe\n");
            exit(-1);

        }

    }
    
    sleep(3);

    //libération de la mémoir
    array_destroy(&lcmd);

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

    unlink(tubenomme);

    return 0;
}