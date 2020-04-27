#include "message.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int send_string(int fd, const char *str){
    ssize_t len = strlen(str);

    ssize_t tmp = len;

    len = len*2;

    char str1[len];

    snprintf(str1, len, "%ld %s",tmp,str);

    ssize_t valwrite = write(fd, str1, len);

    if(valwrite == -1){

        fprintf(stderr,"The string can't be write in the pipe\n");
        exit(-1);

    }

    return 0;
}

char *recv_string(int fd){

    char buff[2];
    char *res;
    size_t n;

    while((n = read(fd, buff, 1)) == -1){

        if( n > 0){

            if(!isspace(buff)){

                *res = buff;
                ++res;

            }else{

                break;

            }

        }

    }

    int len = atoi(res);

    printf("%d",len);

    return NULL;
}

int send_argv(int fd, char *argv[]){

    return 0;
}

char **recv_argv(int fd){

    return NULL;
}