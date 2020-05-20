#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <string.h>

volatile sig_atomic_t usr1_receive = 0;

void handSIGUSR1(int sig){

    usr1_receive = 1;

}

void handSIGUSR2(int sig){

    usr1_receive = 2;

}

int main(int argc,char *argv[]){

    //exo7
    //creation de la structure de la liste des commandes    
    struct array list_cmd;

    array_create(&list_cmd);

    struct cmd newcmd;

    //Test si le fichier period.pid existe deja sinon Ecriture du PID dans period.pid
    int filePidExist = access("/tmp/period.pid", F_OK);

    if(filePidExist == 0){

        fprintf(stderr,"Le fichier '/tmp/period.pid' existe déjà\n");
        exit(1);

    }

    FILE *fp = fopen("/tmp/period.pid","w");

    if(fp==NULL){

        perror("fopen");
        exit(2);

    }

    pid_t pid = getpid();

    printf("%d\n",pid);

    fwrite(&pid, sizeof(pid_t), 1, fp);

    int c = fclose(fp);

    if(c == EOF){

        perror("fclose");
        exit(3);

    }

    //Exo6.2
    //redirection de stdin dans /tmp/period.out
    //redirection de stderr dans /tmp/period.err

    //Ouverture du tube nommé s'il existe déjà sinon le créer
    char *tubeNomme = "/tmp/period.fifo";
    int fd;

    int fileFifoExist = access(tubeNomme, F_OK);

    if(fileFifoExist == -1){

        creationOuvrirTube(tubeNomme,&fd);

    }else{

        fd = open(tubeNomme, O_RDWR);

        if(fd == -1){

            perror("open");
            exit(4);

        } 

    }

    //Ouverture du repertoire period s'il existe sinon le créer
    char *repPeriod = "/tmp/period";
    DIR *dir = opendir(repPeriod);

    if(dir == NULL){

        int creatDir = mkdir(repPeriod, S_IRWXU);

        if(creatDir == -1){

            perror("mkdir");
            exit(-1);

        }

    }

    while(1){

        signal(SIGUSR1,handSIGUSR1);
        signal(SIGUSR2,handSIGUSR2);

        if(usr1_receive == 1){           
            
            char **argv_recv = recv_argv(fd);   

            size_t size = 0;

            while(argv_recv[size] != NULL){

                size++;

            }

            size_t sizeCmd = size - 2;// d'abord - 3 pour (./periodic start period) et +1 pour (NULL) à la fin du tableau de cmd

            cmd_create(&newcmd, sizeCmd);            
            
            for(size_t x = 3; x < size; x++){

                newcmd.nameAndArgs[x-3] = argv_recv[x];

            }

            if(strcmp(argv_recv[1],"now") == 0){

                newcmd.date = time(NULL);

            }else{

                newcmd.date = strtol(argv_recv[1], NULL, 10);

            }

            newcmd.periode = strtol(argv_recv[2], NULL, 10);

            array_add(&list_cmd, newcmd);

            free(argv_recv);
            closeTube(fd);
            unlink(tubeNomme);

            usr1_receive = 0;
            break;

        }
        
        if(usr1_receive == 2){

            printf("Bonjour\n");

            usr1_receive = 0;
            break;

        }
        

    }
    
    array_destroy(&list_cmd);

    return 0;
}