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

    int fd = open("/tmp/example.fifo", O_RDWR);

    if(fd == -1){
        perror("open");
        exit(2);
    }

    //recv string by fd
    printf("Receve the string :\n");

    char *resRecv = recv_string(fd);

    printf("%s\n",resRecv);

    //recv the array of string
    printf("Receve the array of string\n");

    char **test = recv_argv(fd);

    ssize_t j = 0;
    while(test[j] != NULL){

        printf("%s\n",test[j]);
        j++;

    }

    //memory clean

    free(resRecv);

    ssize_t i = 0;
    while(test[i] != NULL){

        free(test[i]);
        i++;

    }

    free(test);

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

    return 0;
}