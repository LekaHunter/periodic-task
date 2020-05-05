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

struct cmd {

    char **nameAndArgs;
    long date;
    long periode;

};

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

            free(argv_recv);
            closeTube(fd);
            unlink(tubeNomme);

            usr1_receive = 0;
            break;

        }


    }

    

    /*for(size_t i = 0; i < list_cmd.size; i++){

        size_t j = 0;

        while(list_cmd.listCmd[i].nameAndArgs[j] != NULL){

            printf("%s\n",list_cmd.listCmd[i].nameAndArgs[j]);
            j++;

        }
        
        printf("date = %ld\n",list_cmd.listCmd[i].date);
        printf("periode = %ld\n",list_cmd.listCmd[i].periode);

    }*/   
    
    array_destroy(&list_cmd);

    return 0;
}