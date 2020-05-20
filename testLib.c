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

    //char *tubenomme = "/tmp/example.fifo";
    //int resSend;

    int res = mkfifo("/tmp/example.fifo", 0644);

    if(res == -1){
        perror("mkfifo");
        exit(1);
    }

    int fd = open("/tmp/example.fifo", O_WRONLY);

    if(fd == -1){
        perror("open");
        exit(2);
    }

    /*const char *str = "echo";

    //send string in the named pipe
    printf("Send '%s' with %ld size in the named pipe\n",str,strlen(str));

    resSend = send_string(fd, str);

    if(resSend == -1){
        fprintf(stderr,"Erreur send_string\n");
        exit(3);
    }

    sleep(2);*/

    //send array of string
    printf("Send an array of string\n");

    char **argvEx = (char **)calloc(5,sizeof(char *));

    argvEx[0] = "salut";
    argvEx[1] = "toi";
    argvEx[2] = "yo";
    argvEx[3] = "ghy";
    argvEx[4] = NULL;

    int resSendArgv = send_argv(fd,argvEx);

    if(resSendArgv != 0){

        fprintf(stderr,"Send argv ne fonction pas!\n");
        exit(1);

    }

    free(argvEx);

    sleep(5);

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

    unlink("/tmp/example.fifo");

    return 0;
}