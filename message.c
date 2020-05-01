#include "message.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

void creationOuvrirTube(char *path, int *fd){

    int res = mkfifo(path, S_IRWXU);

    if(res == -1){

        perror("mkfifo");
        exit(1);

    }

    *fd = open(path, O_RDWR);

    if(*fd == -1){

        perror("open");
        exit(2);

    }

}

void closeTube(int fd){

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

}

int send_string(int fd, const char *str){
    ssize_t len = strlen(str);

    ssize_t writeSize = write(fd, &len, sizeof(ssize_t));

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

    ssize_t sizeString;

    ssize_t readSize = read(fd, &sizeString, sizeof(ssize_t));

    if(readSize == -1){

        fprintf(stderr,"The size of the string can't be read in the pipe\n");
        exit(-1);

    }

    char *str = (char *)calloc(sizeString+1,sizeof(char));

    assert(str != NULL);

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
            exit(-1);

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

    assert(argv != NULL);

    argv[sizeArr-1] = NULL;
    
    for(size_t i = 0; i < sizeArr-1; i++){

        argv[i] = recv_string(fd);

    }

    return argv;
}

int procExPeriod(const char *path, pid_t *pid){

    FILE *fp = fopen(path,"r");

    if(fp == NULL){

        perror("fopen");
        exit(-1);

    }

    size_t r = fread(pid, sizeof(pid_t), 1, fp);

    if(r != 1){

        perror("fread");
        exit(-1);

    }

    int c = fclose(fp);

    if(c == EOF){

        perror("fclose");
        exit(-1);

    }


    return 0;
}

void affiUsage(){

    printf("usage : ./periodic start period cmd [arg]...\n");
    printf("Usage : ./periodic\n");

}

int argvValidite(int argc, char *argv[]){

    char *endptr, *endptr2;
    char *now = "now";

    long date = strtol(argv[1],&endptr,10);

    long period = strtol(argv[2],&endptr2,10);

    if(argv[1] == endptr && strcmp(endptr,now) == 0){ 

        if(*endptr2 != '\0'){

            printf("invalid period\n");
            affiUsage();
            exit(-1);

        }       

        if(period == 0){

            printf("On souhaite exécuter '");

            size_t i = 3;

            while(i < argc){

                if(i == argc-1){

                    printf("%s",argv[i]);

                }else{

                    printf("%s ",argv[i]);

                }

                ++i;

            }

            printf("' en commençant maintenant\n");
            printf("La commande ne sera exécutée qu'une seule fois\n");

        }else{

            printf("On souhaite exécuter '");
            
            size_t i = 3;

            while(i < argc){

                if(i == argc-1){

                    printf("%s",argv[i]);

                }else{

                    printf("%s ",argv[i]);

                }

                ++i;

            }
            
            printf("' toutes les %ld secondes à partir de maintenant\n", period);

        }        

    }else if(argv[1][0] == '+' && *endptr == '\0'){

        if(*endptr2 != '\0'){

            printf("invalid period\n");
            affiUsage();
            exit(-1);

        } 

        if(period == 0){

            printf("On souhaite exécuter '");
            
            size_t i = 3;

            while(i < argc){

                if(i == argc-1){

                    printf("%s",argv[i]);

                }else{

                    printf("%s ",argv[i]);

                }

                ++i;

            }
            
            printf("' en commençant dans %ld secondes\n", date);
            printf("La commande ne sera exécutée qu'une seule fois\n");

        }else{

            printf("On souhaite exécuter '");
            
            size_t i = 3;

            while(i < argc){

                if(i == argc-1){

                    printf("%s",argv[i]);

                }else{

                    printf("%s ",argv[i]);

                }

                ++i;

            }
            
            printf("' toutes les %ld secondes en commençant dans %ld secondes\n", period, date);

        }

    }else if(argv[1][0] != '+' && errno == 0 && *endptr == '\0'){

        if(*endptr2 != '\0'){

            printf("invalid period\n");
            affiUsage();
            exit(-1);

        } 

        time_t dateNow = time(NULL);

        if(date < dateNow){

            printf("La date de départ est antérieur à la date actuel\n");
            exit(-1);

        }else{

            char *res = ctime(&date);

            if(period == 0){

                printf("On souhaite exécuter '");
            
                size_t i = 3;

                while(i < argc){

                    if(i == argc-1){

                        printf("%s",argv[i]);

                    }else{

                        printf("%s ",argv[i]);

                    }

                    ++i;

                }
                
                printf("' le %s", res);
                printf("La commande ne sera exécutée qu'une seule fois\n");

            }else{

                printf("On souhaite exécuter '");
            
                size_t i = 3;

                while(i < argc){

                    if(i == argc-1){

                        printf("%s",argv[i]);

                    }else{

                        printf("%s ",argv[i]);

                    }

                    ++i;

                }
                
                printf("' toutes les %ld secondes à partir du %s", period, res);

            }

        }

    }else{

        printf("invalid start\n");
        affiUsage();
        exit(-1);

    }

    return 0;
}