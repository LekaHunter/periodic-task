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
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

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

int send_cmd(int fd, struct cmd *self){

    int resSend = send_argv(fd, self->nameAndArgs);

    if(resSend != 0){

        fprintf(stderr,"The array of string can't be send!\n");
        exit(-1);

    }

    ssize_t writeDate = write(fd, &self->date, sizeof(size_t));

    if(writeDate == -1){

        fprintf(stderr,"The date of the cmd can't be write in the pipe\n");
        exit(-1);

    }

    ssize_t writePeriod = write(fd, &self->periode, sizeof(size_t));

    if(writePeriod == -1){

        fprintf(stderr,"The period of the cmd can't be write in the pipe\n");
        exit(-1);

    }

    return 0;
}

int send_arrayCmd(int fd, struct array *self){

    //size without NULL
    ssize_t writeSizeSelf = write(fd, &(self->size), sizeof(size_t));

    if(writeSizeSelf == -1){

        fprintf(stderr,"The size array of the string can't be write in the pipe\n");
        exit(-1);

    }

    for(size_t i = 0; i < self->size; i++){

        int resSendCmd = send_cmd(fd, &(self->listCmd[i]));

        if(resSendCmd == -1){

            fprintf(stderr,"Erreur send_cmd\n");
            exit(3);

        }

    }

    return 0;
}

//envoie d'une command via fd
struct cmd *recv_cmd(int fd){

    size_t sizeCmdTab;

    //envoie de la taille du tableau char **sizeCmdTab
    ssize_t readSizeCmdTab = read(fd, &sizeCmdTab, sizeof(size_t));

    if(readSizeCmdTab == -1){

        fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
        exit(-1);

    }

    struct cmd *recv_cmd;

    recv_cmd = (struct cmd *)calloc(1,sizeof(struct cmd));

    //creation d'une cmd avec la taille reçu
    cmd_create(recv_cmd, sizeCmdTab+1);

    //remplir le tableau char **sizeCmdTab du nouveau cmd créer par les string que l'on reçoit 
    for(size_t j = 0; j < sizeCmdTab; j++){

        recv_cmd->nameAndArgs[j] = recv_string(fd);

    }

    //lire/sauvegarder la date de lancement de la cmd
    ssize_t readDate = read(fd, &recv_cmd->date, sizeof(size_t));

    if(readDate == -1){

        fprintf(stderr,"The date can't be read in the pipe\n");
        exit(-1);

    }

    //lire/sauvegarder la periode de lancement de la cmd
    ssize_t readPeriod = read(fd, &recv_cmd->periode, sizeof(size_t));

    if(readPeriod == -1){

        fprintf(stderr,"The period can't be read in the pipe\n");
        exit(-1);

    }

    //renvoyé l'adresse de la cmd ainsi créer
    return recv_cmd;
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

void cmd_create(struct cmd *self, size_t size){

    assert(self != NULL);

    self->date = 0;
    self->periode = 0;

    self->nameAndArgs = (char **)calloc(size, sizeof(char *));
    self->nameAndArgs[size-1] = NULL;

}

void cmd_destroy(struct cmd *self){

    assert(self != NULL);

    free(self->nameAndArgs);

    self->date = 0;
    self->periode = 0;

}

void array_create(struct array *self){

    assert(self != NULL);

    self->capacity = 100;
    self->size = 0;
    self->listCmd = (struct cmd *)calloc(self->capacity,sizeof(struct cmd));

}

void array_destroy(struct array *self){

    assert(self != NULL);

    for(size_t i = 0; i < self->size; i++){

        cmd_destroy(&(self->listCmd[i]));

    }

    free(self->listCmd);
    self->capacity = 0;
    self->size = 0;

}

void array_add(struct array *self, struct cmd newAdd){

    assert(self != NULL);

    if(self->size == self->capacity){

        self->capacity = self->capacity * 2;

        struct cmd *data = (struct cmd *)calloc(self->capacity,sizeof(struct cmd));

        memcpy(data,self->listCmd,self->size*sizeof(struct cmd));

        free(self->listCmd);

        self->listCmd = data;

    }

    self->listCmd[self->size] = newAdd;
    self->size = self->size + 1;

}

void array_remove(struct array *self, size_t index){

    assert(self != NULL);

    for(size_t i = index + 1; i < self->size; ++i){

        self->listCmd[i - 1] = self->listCmd[i];

    }

    self->size = self->size - 1;

}

struct cmd *array_get(const struct array *self, size_t index){

    assert(self != NULL);

    if(index >= self->size){

        return NULL;
        
    }

    struct cmd *getCmd = &(self->listCmd[index]);
    
    return getCmd;
}

size_t array_size(const struct array *self){

    assert(self != NULL);

    return self->size;

}

bool compare_cmd(struct cmd cmd1, struct cmd cmd2){

    assert(&cmd1 != NULL && &cmd2 != NULL);

    bool testTab = true;
    size_t i = 0;
    while(cmd1.nameAndArgs[i] != NULL && cmd2.nameAndArgs[i] != NULL){

        if(strcmp(cmd1.nameAndArgs[i],cmd2.nameAndArgs[i]) != 0){

            testTab = false;
            break;

        }

        i++;
    }

    if(testTab == true && cmd1.date == cmd2.date && cmd1.periode == cmd2.periode){

        return true;

    }

    return false;
}

size_t array_search(const struct array *self, struct cmd command){

    assert(self != NULL);
  
    size_t index = 0; 

    while(index < self->size && compare_cmd(self->listCmd[index],command) == false){

        index = index + 1;

    }    

    return index;

}