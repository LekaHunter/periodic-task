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

    int fd = open("/tmp/example.fifo", O_RDONLY);

    if(fd == -1){
        perror("open");
        exit(2);
    }

    /*//recv string by fd
    printf("Receve the string :\n");

    char *resRecv = recv_string(fd);

    size_t i = 0;
    while (i < strlen(resRecv)+1){

        if(resRecv[i] == '\0'){

            printf("Je suis a la fin dans Lib2\n");

        }

        printf("%c\n",resRecv[i]);
        i++;

    }   

    printf("%s\n",resRecv);
    
    free(resRecv);
    sleep(2);*/

    //recv the array of string
    printf("Receve the array of string\n");

    char **test = recv_argv(fd);

    ssize_t i = 0;
    while(test[i] != NULL){

        printf("String reçu : %s\n",test[i]);
        i++;

    }

    ssize_t k = 0;
    while(test[k] != NULL){

        free(test[k]);
        k++;

    }

    free(test);
    sleep(2);

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

    return 0;
}