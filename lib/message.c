#include "../include/message.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

int send_string(int fd, const char *str){

    size_t len = strlen(str);//longueur de str sans '\0'    

    ssize_t writeSize = write(fd, &len, sizeof(size_t));

    if(writeSize == -1){

        fprintf(stderr,"The size of the string can't be write in the pipe\n");
        return -1;

    }

    ssize_t writeString = write(fd, str, len);

    if(writeString == -1){

        fprintf(stderr,"The string can't be write in the pipe\n");
        return -1;

    }

    return 0;
}

char *recv_string(int fd){

    size_t sizeString = 0;

    ssize_t readSize = read(fd, &sizeString, sizeof(size_t));

    if(readSize == -1){

        fprintf(stderr,"The size of the string can't be read in the pipe\n");
        exit(-1);

    }

    char *str = (char *)calloc(sizeString+1,sizeof(char));

    ssize_t readString = read(fd, str, sizeString);

    if(readString == -1){

        fprintf(stderr,"The string can't be read in the pipe\n");
        exit(-1);

    }

    str[sizeString] = '\0';

    return str;
}

int send_argv(int fd, char *argv[]){

    size_t size = 0;
    
    while(argv[size] != NULL){

        size++;

    }

    //size without NULL
    ssize_t writeSizeArgv = write(fd, &size, sizeof(size_t));

    if(writeSizeArgv == -1){

        fprintf(stderr,"The size array of the string can't be write in the pipe\n");
        exit(-1);

    }

    int resSendString = 0;

    size_t j = 0;
    while(argv[j] != NULL){

        resSendString = send_string(fd, argv[j]);

        if(resSendString == -1){

            fprintf(stderr,"Erreur send_string\n");
            exit(-1);

        }
        
        j++;

    }

    return 0;
}

char **recv_argv(int fd){

    size_t size = 0;

    ssize_t readSizeArr = read(fd, &size, sizeof(size_t));

    if(readSizeArr == -1){

        fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
        exit(-1);

    }

    char **arr = (char **)calloc(size+1,sizeof(char *));

    for(size_t j = 0; j < size; j++){

        arr[j] = recv_string(fd);

   }

    arr[size] = NULL;

    return arr;
}