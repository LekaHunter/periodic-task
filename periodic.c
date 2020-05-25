#include "include/message.h"
#include "include/periodicLib.h"

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

#define MAX_ARG_STRLEN 131072

int main(int argc, char *argv[]){

    int envoieSignal;

    //lecture du pid de 'period' par periodic (exo 3.1)
    pid_t pid = procExPeriod("/tmp/period.pid");

    //Ouverture du tube nommé s'il existe déjà sinon le créer (mettre en fonction)
    char *tubeNomme = "/tmp/period.fifo";
    int fd;

    int fileFifoExist = access(tubeNomme, F_OK);

    if(fileFifoExist == -1){

        printf("Le tube nommé '/tmp/period.fifo' n'existe pas\n");
        exit(1);

    }

    //Envoie de SIGUSR1 à period et recevoir la liste des commandes et afficher
    if(argc == 1){

        printf("pid de period : %ld\n",pid);

        //Envoie signal SIGUSER1 à 'period' (exo 5.1)
        envoieSignal = kill(pid, SIGUSR2);

        if(envoieSignal == -1){

            printf("L'envoie du signal SIGUSER2 à 'period' à echouer\n");
            perror("kill");
            exit(3);

        }

        //A REFAIRE ON RECOIT ET ON AFFICHE

        fd = open(tubeNomme, O_RDONLY);

        if(fd == -1){

            perror("open");
            exit(2);

        }

        size_t size = 0;
        ssize_t readSize = read(fd, &size, sizeof(size_t));

        printf("La liste des commandes :\n\n");

        if(readSize == -1){

            fprintf(stderr,"The size of the array of the string can't be read in the pipe\n");
            exit(4);

        }

        //receve the array of char * it containe the cmd and its arg
        for(size_t i = 0; i < size; i++){

            char **res = recv_argv(fd);

            printf("La %ld-ème commande : \n",i+1);

            size_t k = 0;
            while(res[k] != NULL){
                
                printf("%s ",res[k]);
                k++;

            }

            printf("\n");

            size_t date = 0;

            //receve the size
            ssize_t readDate = read(fd, &date, sizeof(size_t));

            if(readDate == -1){

                fprintf(stderr,"The date of the command can't be read in the pipe\n");
                exit(5);

            }

            size_t periode = 0;

            //receve the size
            ssize_t readPeriode = read(fd, &periode, sizeof(size_t));

            if(readPeriode == -1){

                fprintf(stderr,"The periode of the command can't be read in the pipe\n");
                exit(6);

            }

            char *dateStr = ctime(&date);

            printf("date le : %s",dateStr);
            printf("periode = %ld\n",periode);
            printf("\n");

            size_t j = 0;
            while(res[j] != NULL){

                free(res[j]);
                j++;

            }

            free(res);

        }

        closeTube(fd);

        return 0; //est ce qu'après ce if le programme s'arrete?

    }

    if(argc < 4){

        affiUsage();
        exit(2);

    }

    printf("pid de period : %ld\n",pid);

    //Teste de validation des arguments (Exo 4.1)
    int res = argvValidite(argc, argv);

    if(res != 0){

        printf("Error argvValidite\n");
        exit(3);

    }

    //Envoie signal SIGUSER1 à 'period' (exo 4.2)
    envoieSignal = kill(pid, SIGUSR1);

    if(envoieSignal == -1){

        printf("L'envoie du signal à 'period' à echouer\n");
        perror("kill");
        exit(4);

    }

    fd = open(tubeNomme, O_WRONLY);

    if(fd == -1){

        perror("open");
        exit(2);

    }

    //conversion de start si il à un '+' en long qui corespond à un time(NULL)
    if(argv[1][0] == '+'){

        char start[MAX_ARG_STRLEN];
        sprintf(start,"%ld",atoi(argv[1]+1) + time(NULL));
        argv[1] = start;

    }

    //envoyer les informations nécessaires à 'period' via le tube nommé (exo 4.3)
    int sendArgv = send_argv(fd, argv);

    if(sendArgv == -1){

        fprintf(stderr,"L'envoie de argv a échouer!\n");
        exit(5);
        
    }

    closeTube(fd);

    return 0;
}