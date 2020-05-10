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

    sleep(2);

    //send string in the named pipe
    printf("Send '%s' with %ld size in the named pipe\n",str,strlen(str));

    resSend = send_string(fd, str);

    if(resSend == -1){
        fprintf(stderr,"Erreur send_string\n");
        exit(3);
    }

    sleep(2);

    //send array of string
    printf("Send an array of string\n");

    char **argvEx = (char **)calloc(5,sizeof(char *));

    argvEx[0] = "salut";
    argvEx[1] = "toi";
    argvEx[2] = "yo";
    argvEx[3] = "ghy";
    argvEx[4] = NULL;    

    for(size_t j = 0; j < 4; j++){

        printf("%s\n",argvEx[j]);

    }

    sleep(2);

    int resSendArgv = send_argv(fd,argvEx);

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

    free(argvEx);

    unlink(tubenomme);

    return 0;
}