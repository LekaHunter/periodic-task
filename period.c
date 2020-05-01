#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <assert.h>

int main(int argc,char *argv[]){

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
    
    char **argv_recv = recv_argv(fd);   

    closeTube(fd);
    unlink(tubeNomme);

    //exo7
    //creation de la structure de la liste des commandes    

    //ajout de la cmd dans listCmd

    //suppression de la cmd dans listCmd

    //recherche de la cmd à réaliser dans listCmd

    //envoie de listCmd quand period reçoit SIGUSER2

    ssize_t i = 0;
    while(argv_recv[i] != NULL){

        free(argv_recv[i]);
        i++;

    }

    free(argv_recv);

    return 0;
}