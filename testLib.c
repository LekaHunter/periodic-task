#include "message.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]){

    int fd;
    char *tubenomme = "/tmp/example.fifo";
    int resSend;

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

    const char *str = "Salut/toi/yo/ghy/yo/ahahahahahahahahaZ";

    resSend = send_string(fd, str);

    if(resSend == -1){
        fprintf(stderr,"Erreur send_string\n");
        exit(3);
    }

    char *resRecv = recv_string(fd);

    printf("%s\n",resRecv);

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

    return 0;
}