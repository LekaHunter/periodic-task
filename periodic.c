#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

int main(int argc, char *argv[]){
    pid_t pid;
    int readPid;
    int envoieSignal;

    if(argc == 1){

        //lecture du pid de 'period' par periodic
        readPid = procExPeriod("/tmp/period.pid", &pid);

        if(readPid == -1){

            printf("La lecture du pid dans le '/tmp/period.pid' à echouer\n");
            exit(4);

        }

        //Envoie signal SIGUSER1 à 'period'
        envoieSignal = kill(pid, SIGUSR2);

        if(envoieSignal == -1){

            printf("L'envoie du signal SIGUSER2 à 'period' à echouer\n");
            perror("kill");
            exit(5);

        }

        printf("Liste des commandes dans period :\n");


        exit(1);

    }

    if(argc < 4){

        affiUsage();
        exit(2);

    }

    long date;
    long period;
    
    int res = argvValidite(argc, argv, &date, &period);

    if(res != 0){

        printf("Error argvValidite\n");
        exit(3);

    }

    /*//lecture du pid de 'period' par periodic
    readPid = procExPeriod("/tmp/period.pid", &pid);

    if(readPid == -1){

        printf("La lecture du pid dans le '/tmp/period.pid' à echouer\n");
        exit(4);

    }

    //Envoie signal SIGUSER1 à 'period'
    envoieSignal = kill(pid, SIGUSR1);

    if(envoieSignal == -1){

        printf("L'envoie du signal à 'period' à echouer\n");
        perror("kill");
        exit(5);

    }*/

    //envoyer les informations nécessaires à 'period' via le tube nommé
    char *tubeNomme = "/tmp/period.fifo";
    int fd;

    creationOuvrirTube(tubeNomme,&fd);

    ssize_t writeSize = write(fd, &date, sizeof(long));

    if(writeSize == -1){

        fprintf(stderr,"La date de départ n'a pas pu être envoyé!\n");
        exit(-1);

    }

    sleep(5);

    writeSize = write(fd, &period, sizeof(long));

    if(writeSize == -1){

        fprintf(stderr,"La period n'a pas pu être envoyé!\n");
        exit(-1);

    }

    sleep(5);

    int sendArgv = send_argv(fd, argv);

    if(sendArgv == -1){

        fprintf(stderr,"L'envoie de argv a échouer!\n");
        exit(-1);
        
    }

    closeTube(fd);

    return 0;
}