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

    //recv string by fd
    printf("Receve the string :\n");

    size_t cap = 0;

    //receve the capacity
    ssize_t readCap = read(fd, &cap, sizeof(size_t));

    if(readCap == -1){

        fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
        exit(-1);

    }

    size_t size = 0;

    //receve the size
    ssize_t readSize = read(fd, &size, sizeof(size_t));

    if(readSize == -1){

        fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
        exit(-1);

    }

    //receve the array of char * it containe the cmd and its arg
    for(size_t i = 0; i < size; i++){

        char **res = recv_argv(fd);

        size_t i = 0;
        while(res[i] != NULL){

            printf("%s ",res[i]);
            i++;

        }

        size_t date = 0;

        //receve the size
        ssize_t readDate = read(fd, &date, sizeof(size_t));

        if(readDate == -1){

            fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
            exit(-1);

        }

        size_t periode = 0;

        //receve the size
        ssize_t readPeriode = read(fd, &periode, sizeof(size_t));

        if(readPeriode == -1){

            fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
            exit(-1);

        }

        printf("date = %ld\n",date);
        printf("periode = %ld\n",periode);
        printf("\n");

        size_t j = 0;
        while(res[j] != NULL){

            free(res[j]);
            j++;

        }

        free(res);

    }

    printf("capacity = %ld\n",cap);
    printf("size = %ld\n",size);

    sleep(3);

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

    return 0;
}