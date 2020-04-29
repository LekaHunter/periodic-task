#include "message.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>


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

    ssize_t sizeString;

    ssize_t readSize = read(fd, &sizeString, sizeof(ssize_t));

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

    return str;
}

int send_argv(int fd, char *argv[]){

    size_t i = 0;
    
    while(argv[i] != NULL){

        i++;

    }

    i++; // pour le NULL en fin de tableau

    ssize_t writeSizeArgv = write(fd, &i, sizeof(ssize_t));

    if(writeSizeArgv == -1){

        fprintf(stderr,"The size array of the string can't be write in the pipe\n");
        exit(-1);

    }

    int resSendString;

    size_t j = 0;
    while(argv[j] != NULL){

        resSendString = send_string(fd, argv[j]);

        if(resSendString == -1){

            fprintf(stderr,"Erreur send_string\n");
            exit(3);

        }

        j++;

    }

    return 0;
}

char **recv_argv(int fd){

    size_t sizeArr;

    ssize_t readSizeArr = read(fd, &sizeArr, sizeof(size_t));

    if(readSizeArr == -1){

        fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
        exit(-1);

    }

    char **argv = (char **)calloc(sizeArr,sizeof(char *));
    argv[sizeArr-1] = NULL;
    
    for(size_t i = 0; i < sizeArr-1; i++){

        argv[i] = recv_string(fd);

    }

    return argv;
}

void procExPeriod(const char *path, pid_t *pid){

    FILE *fp = fopen(path,"r");

    if(fp == NULL){

        perror("fopen");
        exit(5);

    }

    size_t r = fread(pid, sizeof(pid_t), 1, fp);

    int c = fclose(fp);

    if(c == EOF){

        perror("fclose");
        exit(6);

    }

}