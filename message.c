#include "message.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int send_string(int fd, const char *str){
    ssize_t len = strlen(str);

    ssize_t tmp = len;

    len = len+4;

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

    char buff;
    char sizeString[sizeof(int)];
    int i = 0;

    while(read(fd, &buff, 1) > 0){

        if(!isspace(buff)){

            sizeString[i]=buff;
            i++;

        }else{

            sizeString[i]='\0';
            break;

        }

    }

    int sizeTab = atoi(sizeString)+1;

    //à revoir
    char *str = (char *)calloc(sizeTab,sizeof(char));
    lseek(fd, 3, SEEK_SET);

    char tmp;
    int j = 0;

    while(read(fd, &tmp, 1) > 0){

        if(tmp != '\0'){

            str[j] = tmp;
            j++;

        }else{

            str[j] = '\0';
            break;

        }

    }

    return str;
}

int send_argv(int fd, char *argv[]){

    return 0;
}

char **recv_argv(int fd){

    return NULL;
}