#include "message.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

int send_string(int fd, const char *str){
    ssize_t len = strlen(str);

    ssize_t writeSize = write(fd, &len, sizeof(ssize_t));

    if(writeSize == -1){

        fprintf(stderr,"The size of the string can't be write in the pipe\n");
        exit(-1);

    }

    ssize_t writeString = write(fd, str, len);

    if(writeString == -1){

        fprintf(stderr,"The string can't be write in the pipe\n");
        exit(-1);

    }

    return 0;
}

char *recv_string(int fd){

    ssize_t sizeChar;

    ssize_t readSize = read(fd, &sizeChar, sizeof(ssize_t));

    if(readSize == -1){

        fprintf(stderr,"The size of the string can't be read in the pipe\n");
        exit(-1);

    }

    char *str = (char *)calloc(sizeChar+1,sizeof(char));

    ssize_t readString = read(fd, str, sizeChar);

    if(readString == -1){

        fprintf(stderr,"The string can't be read in the pipe\n");
        exit(-1);

    }

    return str;
}

int send_argv(int fd, char *argv[]){

    return 0;
}

char **recv_argv(int fd){

    return NULL;
}