#include "../include/periodicLib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

void affiUsage(){

    printf("usage : ./periodic start period cmd [arg]...\n");
    printf("Usage : ./periodic\n");

}

pid_t procExPeriod(const char *path){
    pid_t pid = 0;

    int fileFifoExist = access(path, F_OK);

    if(fileFifoExist == -1){

        printf("Le fichiet '/tmp/period.pid' n'existe pas!\n");
        exit(1);

    }else{

        FILE *fp = fopen(path,"r");

        if(fp == NULL){

            perror("fopen");
            exit(2);

        }

        size_t r = fread(&pid, sizeof(pid_t), 1, fp);

        if(r != 1){

            perror("fread");
            exit(3);

        }

        int c = fclose(fp);

        if(c == EOF){

            perror("fclose");
            exit(4);

        }

    }

    return pid;
}

void closeTube(int fd){

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(1);

    }

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
            exit(1);

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
            exit(2);

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
            exit(3);

        } 

        time_t dateNow = time(NULL);

        if(date < dateNow){

            printf("La date de départ est antérieur à la date actuel\n");
            exit(4);

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
        exit(5);

    }

    return 0;
}