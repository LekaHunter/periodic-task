#include "include/message.h"
#include "include/periodLib.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define MAX_ARG_STRLEN 131072

/** 
 * Déclaration de variables globales servant de drapeaux
*/

volatile sig_atomic_t usr1_receive = 0;

/** 
 * Handler quand period.c reçoit SIGUSER1
*/

void handSIGUSR1(int sig){

    usr1_receive = 1;

}

/** 
 * Handler quand period.c reçoit SIGUSER2
*/

void handSIGUSR2(int sig){

    usr1_receive = 2;

}

/** 
 * Handler quand period.c reçoit SIGCHLD
 * 
*/

void handEndChild(int sig){

    usr1_receive = 3;

}

/** 
 * Handler quand period.c reçoit SIGALRM
 * 
*/

void handSigAlrm(int sig){

    usr1_receive = 4;

}

/** 
 * Handler quand period.c reçoit SIGINT ou SIGQUIT ou SIGTERM
 * 
*/

void handFinProc(int sig){

    usr1_receive = 5;

}

int main(int argc,char *argv[]){

    //redirection de stdout et de stderr (exo 6.2)
    //open de period.out et redirection
    size_t fdout = open("/tmp/period.out", O_WRONLY | O_CREAT ,0644);

    if(fdout == -1){

        fprintf(stderr,"Erreur lors de l'ouverture de '/tmp/period.out'\n");
        perror("open");
        exit(1);

    }

    int resfdout = dup2(fdout,STDOUT_FILENO);

    if(resfdout == -1 ){

        fprintf(stderr,"Je suis l'ouverture de period.out\n");
        perror("dup2");
        closeTube(fdout);
        exit(1);

    }

    closeTube(fdout);

    //open de period.err et redirection
    size_t fderr = open("/tmp/period.err", O_WRONLY | O_CREAT ,0644);

    if(fderr == -1){

        fprintf(stderr,"Erreur lors de l'ouverture de '/tmp/period.err'\n");
        perror("open");
        exit(1);

    }

    int resfderr = dup2(fderr,STDERR_FILENO);

    if(resfderr == -1 ){
        
        fprintf(stderr,"Je suis l'ouverture de period.err\n");
        perror("dup2");
        closeTube(fderr);
        exit(2);

    }
    
    closeTube(fderr);

    //Mettre en place sigaction pour gérer les signaux
    struct sigaction sigP_user1, sigP_user2, sigP_stop, sigP_chld, sigP_alrm;

    //recevoir SIGUSR1 ou SIGUSR2
    sigP_user1.sa_handler = handSIGUSR1;
    sigemptyset(&sigP_user1.sa_mask);
    sigP_user1.sa_flags = 0;

    sigP_user2.sa_handler = handSIGUSR2;
    sigemptyset(&sigP_user2.sa_mask);
    sigP_user2.sa_flags = 0;

    sigaction(SIGUSR1, &sigP_user1, NULL);
    sigaction(SIGUSR2, &sigP_user2, NULL);

    //recevoir SIGINT ou SIGQUIT ou SIGTERM
    sigP_stop.sa_handler = handFinProc;
    sigemptyset(&sigP_stop.sa_mask);
    sigP_stop.sa_flags = 0;

    sigaction(SIGINT, &sigP_stop, NULL);
    sigaction(SIGQUIT, &sigP_stop, NULL);
    sigaction(SIGTERM, &sigP_stop, NULL);

    //recevoir SIGCHLD
    sigP_chld.sa_handler = handEndChild;
    sigemptyset(&sigP_chld.sa_mask);
    sigP_chld.sa_flags = 0;

    sigaction(SIGCHLD, &sigP_chld, NULL);

    //recevoir SIGALRM
    sigP_alrm.sa_handler = handSigAlrm;
    sigemptyset(&sigP_alrm.sa_mask);
    sigP_alrm.sa_flags = 0;

    sigaction(SIGALRM, &sigP_alrm, NULL);

    //exo7
    //creation de la structure de la liste des commandes    
    struct array list_cmd;

    array_create(&list_cmd);

    struct cmd newcmd;

    //Test si le fichier period.pid existe deja sinon Ecriture du PID dans period.pid (exo 6.1)
    int filePidExist = access("/tmp/period.pid", F_OK);

    if(filePidExist == 0){

        fprintf(stderr,"Le fichier '/tmp/period.pid' existe déjà\n");
        array_destroy(&list_cmd);
        exit(1);

    }

    FILE *fp = fopen("/tmp/period.pid","w");

    if(fp==NULL){

        fprintf(stderr,"Erreur lors de l'ouverture du fichier '/tmp/period.pid'\n");
        perror("fopen");
        exit(2);

    }

    pid_t pid = getpid();

    fprintf(stdout,"Pid de period :%d\n",pid);

    fwrite(&pid, sizeof(pid_t), 1, fp);

    int c = fclose(fp);

    if(c == EOF){

        fprintf(stderr,"Erreur lors de la fermeture de '/tmp/period.pid'\n");
        perror("fclose");
        exit(3);

    }

    //Ouverture du tube nommé s'il existe déjà sinon le créer (exo 6.3)    
    int fileFifoExist = access("/tmp/period.fifo", F_OK);

    if(fileFifoExist == -1){

        creationOuvrirTube("/tmp/period.fifo");

    }

    //Ouverture du repertoire period s'il existe sinon le créer (6.4)
    char *repPeriod = "/tmp/period";
    DIR *dir = opendir(repPeriod);

    if(dir == NULL){

        int creatDir = mkdir(repPeriod, 0744);

        if(creatDir == -1){

            fprintf(stderr,"Erreur lors de la création de '/tmp/period'\n");
            perror("mkdir");
            exit(5);

        }

        dir = opendir(repPeriod);

    }

    char out[MAX_ARG_STRLEN];
    char err[MAX_ARG_STRLEN];
    char *devnull = "/dev/null";

    struct cmd tmp;    
    
    size_t i = 0;

    //exo 8 >
    while(1){ 

        if(list_cmd.size > 0){

            size_t h = 0;
            while(list_cmd.listCmd[0].nameAndArgs[h] != NULL){

                h++;

            }      

            cmd_create(&tmp, h);           
            
            size_t k = 0;
            while(list_cmd.listCmd[0].nameAndArgs[k] != NULL){
                
                size_t sizeStr = strlen(list_cmd.listCmd[0].nameAndArgs[k]);
                tmp.nameAndArgs[k] = (char *)calloc(sizeStr+1,sizeof(char));

                size_t x = 0;
                while(list_cmd.listCmd[0].nameAndArgs[k][x] != '\0'){
                    
                    tmp.nameAndArgs[k][x] = list_cmd.listCmd[0].nameAndArgs[k][x];
                    x++;

                }                

                tmp.nameAndArgs[k][sizeStr] = '\0';

                k++;

            }

            tmp.date = list_cmd.listCmd[0].date;
            tmp.periode = list_cmd.listCmd[0].periode;            

            for(size_t j = 1; j < list_cmd.size; j++){


                if(tmp.date > list_cmd.listCmd[j].date){

                    tmp.date = list_cmd.listCmd[j].date;
                    tmp.periode = list_cmd.listCmd[j].periode;
                    
                    size_t k = 0;
                    while(list_cmd.listCmd[j].nameAndArgs[k] != NULL){

                        tmp.nameAndArgs[k] = list_cmd.listCmd[j].nameAndArgs[k];
                        k++;

                    }

                }

            }

            if(tmp.date == 0){

                if(tmp.periode == 0){

                    sprintf(out,"/tmp/period/%ld.out",i+1);

                    sprintf(err,"/tmp/period/%ld.err",i+1);

                    size_t fdDevNull = open(devnull, O_WRONLY, 0);

                    if(fdDevNull == -1){

                        fprintf(stderr,"Error lors de l'ouverture de '/dev/null' du fils\n");
                        perror("open");
                        exit(1);

                    }

                    size_t fdoutFils = open(out, O_WRONLY | O_CREAT, 0644);

                    if(fdoutFils == -1){

                        fprintf(stderr,"Error lors de l'ouverture du fils\n");
                        perror("open");
                        exit(1);

                    }

                    size_t fderrFils = open(err, O_WRONLY | O_CREAT, 0644);

                    if(fderrFils == -1){
                        
                        fprintf(stderr,"Error lors de l'ouverture du fils\n");
                        perror("open");
                        exit(1);
                        
                    }

                    if(fork() == 0){                

                        int rfdDevNull = dup2(fdDevNull, 0);

                        if(rfdDevNull == -1 ){

                            fprintf(stderr,"redirection de stdin du fils\n");
                            perror("dup2");
                            exit(1);

                        }

                        close(fdDevNull);

                        int rfdoutFils = dup2(fdoutFils, 1);

                        if(rfdoutFils == -1 ){

                            fprintf(stderr,"redirection de stdout du fils\n");
                            perror("dup2");
                            exit(1);

                        }

                        close(fdoutFils);

                        int rfderrFils = dup2(fderrFils, 2);

                        if(rfderrFils == -1 ){
                            
                            fprintf(stderr,"redirection de stderr du fils\n");
                            perror("dup2");
                            exit(2);

                        }
                        
                        close(fderrFils);

                        int n = execvp(tmp.nameAndArgs[0],tmp.nameAndArgs);

                        if(n == -1){

                            fprintf(stderr,"Errer : %d\n",errno);

                        }

                    }

                    cmd_destroy(&tmp);

                    close(fdoutFils);
                    close(fderrFils);

                }else{

                    sprintf(out,"/tmp/period/%ld.out",i+1);

                    sprintf(err,"/tmp/period/%ld.err",i+1);

                    size_t fdDevNull = open(devnull, O_WRONLY, 0);

                    if(fdDevNull == -1){

                        fprintf(stderr,"Error lors de l'ouverture de '/dev/null' du fils\n");
                        perror("open");
                        exit(1);

                    }

                    size_t fdoutFils = open(out, O_WRONLY | O_CREAT, 0644);

                    if(fdoutFils == -1){

                        fprintf(stderr,"Error lors de l'ouverture du fils\n");
                        perror("open");
                        exit(1);

                    }

                    size_t fderrFils = open(err, O_WRONLY | O_CREAT, 0644);

                    if(fderrFils == -1){
                        
                        fprintf(stderr,"Error lors de l'ouverture du fils\n");
                        perror("open");
                        exit(1);
                        
                    }

                    if(fork() == 0){                

                        int rfdDevNull = dup2(fdDevNull, 0);

                        if(rfdDevNull == -1 ){

                            fprintf(stderr,"redirection de stdin du fils\n");
                            perror("dup2");
                            exit(1);

                        }

                        close(fdDevNull);

                        int rfdoutFils = dup2(fdoutFils, 1);

                        if(rfdoutFils == -1 ){

                            fprintf(stderr,"redirection de stdout du fils\n");
                            perror("dup2");
                            exit(1);

                        }

                        close(fdoutFils);

                        int rfderrFils = dup2(fderrFils, 2);

                        if(rfderrFils == -1 ){
                            
                            fprintf(stderr,"redirection de stderr du fils\n");
                            perror("dup2");
                            exit(2);

                        }
                        
                        close(fderrFils);

                        int n = execvp(tmp.nameAndArgs[0],tmp.nameAndArgs);

                        if(n == -1){

                            fprintf(stderr,"Errer : %d\n",errno);

                        }

                    }

                    size_t search = array_search(&list_cmd,tmp);

                    array_remove(&list_cmd, search);

                    tmp.date = tmp.date + tmp.periode;

                    array_add(&list_cmd, tmp);

                    cmd_destroy(&tmp);

                    close(fdoutFils);
                    close(fderrFils);                

                }

            }else{

                long start = time(NULL) - tmp.date;

                alarm(start);
                pause();

                if(tmp.periode != 0){

                    size_t search = array_search(&list_cmd,tmp);

                    array_remove(&list_cmd, search);

                    tmp.date = tmp.date + tmp.periode;

                    array_add(&list_cmd, tmp);

                    cmd_destroy(&tmp);

                }

            }

        }

        if(usr1_receive == 4){

            sprintf(out,"/tmp/period/%ld.out",i+1);

            sprintf(err,"/tmp/period/%ld.err",i+1);

            size_t fdDevNull = open(devnull, O_WRONLY, 0);

            if(fdDevNull == -1){

                fprintf(stderr,"Error lors de l'ouverture de '/dev/null' du fils\n");
                perror("open");
                exit(1);

            }

            size_t fdoutFils = open(out, O_WRONLY | O_CREAT, 0644);

            if(fdoutFils == -1){

                fprintf(stderr,"Error lors de l'ouverture du fils\n");
                perror("open");
                exit(1);

            }

            size_t fderrFils = open(err, O_WRONLY | O_CREAT, 0644);

            if(fderrFils == -1){
                
                fprintf(stderr,"Error lors de l'ouverture du fils\n");
                perror("open");
                exit(1);
                
            }

            if(fork() == 0){                

                int rfdDevNull = dup2(fdDevNull, 0);

                if(rfdDevNull == -1 ){

                    fprintf(stderr,"redirection de stdin du fils\n");
                    perror("dup2");
                    exit(1);

                }

                close(fdDevNull);

                int rfdoutFils = dup2(fdoutFils, 1);

                if(rfdoutFils == -1 ){

                    fprintf(stderr,"redirection de stdout du fils\n");
                    perror("dup2");
                    exit(1);

                }

                close(fdoutFils);

                int rfderrFils = dup2(fderrFils, 2);

                if(rfderrFils == -1 ){
                    
                    fprintf(stderr,"redirection de stderr du fils\n");
                    perror("dup2");
                    exit(2);

                }
                
                close(fderrFils);

                execvp(tmp.nameAndArgs[0],tmp.nameAndArgs);

            }

            cmd_destroy(&tmp);

            close(fdoutFils);
            close(fderrFils);

            usr1_receive = 0;
        }

        if(usr1_receive == 1){

            int fd = open("/tmp/period.fifo", O_RDONLY);

            if(fd == -1){

                fprintf(stderr,"Erreur l'hors de l'ouverture de '/tmp/period.fifo'\n");
                perror("open");
                exit(1);

            }         
            
            //recevoir le signal SIGUSER1 (exo 8.1)
            //ajouts de la cmd dans le liste de cmd
            char **argv_recv = recv_argv(fd);

            size_t size = 0;
            while(argv_recv[size] != NULL){

                size++;

            }

            size_t a = 0;
            while(argv_recv[a] != NULL){

                a++;

            }

            size_t sizeCmd = size-3;// d'abord - 3 pour (./periodic start period) et +1 pour (NULL) à la fin du tableau de cmd

            cmd_create(&newcmd, sizeCmd);

            //vue que j'ai pas calloc dans le cmd_create la place mémoire pour les string que le tableau de string
            //allait recevoir j'ai u des problème de mémoire        
            
            for(size_t x = 3; x < size; x++){
                
                //alors ici on calcule le size d'un string 
                //on alloue la mémoire suffisant dans le tableau de string 
                //on ajout char par char dans le tableau les char des string 
                //on ajout le char \0 à la fin du string 
                //à ameliorer si il y a mieux
                size_t sizeStr = strlen(argv_recv[x]);
                newcmd.nameAndArgs[x-3] = (char *)calloc(sizeStr+1,sizeof(char));

                size_t j = 0;
                while(argv_recv[x][j] != '\0'){
                    
                    newcmd.nameAndArgs[x-3][j] = argv_recv[x][j];
                    j++;

                }                

                newcmd.nameAndArgs[x-3][sizeStr] = '\0';

            }

            if(strcmp(argv_recv[1],"now") == 0){

                newcmd.date = time(NULL);

            }else{

                newcmd.date = strtol(argv_recv[1], NULL, 10);

            }

            newcmd.periode = strtol(argv_recv[2], NULL, 10);

            array_add(&list_cmd, newcmd);

            //libérer la mémoire pour le tableau de string
            size_t k = 0;
            while(argv_recv[k] != NULL){
                
                free(argv_recv[k]);
                k++;

            }

            free(argv_recv);

            closeTube(fd);

            usr1_receive = 0;

        }
        
        if(usr1_receive == 2){

            //recevoir le signal SIGUSER2 (exo 9.1)
            //envoie de la liste de cmd

            int fd = open("/tmp/period.fifo", O_WRONLY);

            if(fd == -1){
                
                fprintf(stderr,"Erreur lors de l'ouverture de '/tmp/period.fifo'\n");
                perror("open");
                exit(1);

            }

            //send size
            ssize_t writeSize = write(fd, &list_cmd.size, sizeof(size_t));

            if(writeSize == -1){

                fprintf(stderr,"The size of the liste of cmd can't be write in the pipe\n");
                exit(1);

            }

            //send array of char * of the cmd and its arg
            for(size_t i = 0; i < list_cmd.size; i++){

                int resSend = send_argv(fd, list_cmd.listCmd[i].nameAndArgs);

                ssize_t writeDate = write(fd, &list_cmd.listCmd[i].date, sizeof(size_t));

                if(writeDate == -1){

                    fprintf(stderr,"The date of the cmd can't be write in the pipe\n");
                    exit(2);

                }

                ssize_t writePeriod = write(fd, &list_cmd.listCmd[i].periode, sizeof(size_t));

                if(writePeriod == -1){

                    fprintf(stderr,"The date of the cmd can't be write in the pipe\n");
                    exit(3);

                }

            }

            closeTube(fd);

            usr1_receive = 0;

        }

        if(usr1_receive == 3){

            //attendre et terminé tous les fils potentiels (exo 11)
            //affichage de la terminaison de ses fils
            while(1){

                int statusFils = 0;
                pid_t fils = wait(&statusFils);

                if(fils == -1){

                    break;

                }else{

                    if(WIFEXITED(statusFils)){

                        if(WEXITSTATUS(statusFils) == 0){

                            fprintf(stdout,"Le fils s'est arrété avec le statu de terminaison : %d\n",WEXITSTATUS(statusFils));
                        
                        }else{

                            fprintf(stderr,"Le fils s'est terminé avec une anomalie de statu : %d\n",WEXITSTATUS(statusFils));
                        
                        } 

                    }

                }

            }
            
        }

        if(usr1_receive == 5){

            //recevoir SIGINT SIGQUIT SIGTERM (exo 12.1)

            array_destroy(&list_cmd);
            unlink("/tmp/period.pid");           
            closedir(dir);
            /*--------------- terminer et éliminer tous les processus créés restant ----------*/
            
            //attendre et terminé tous les fils potentiels (exo 11)
            //affichage de la terminaison de ses fils
            while(1){

                int statusFils = 0;
                pid_t fils = wait(&statusFils);

                if(fils == -1){

                    break;

                }else{

                    if(WIFEXITED(statusFils)){

                        if(WEXITSTATUS(statusFils) == 0){

                            fprintf(stdout,"Le fils s'est arrété avec le statu de terminaison : %d\n",WEXITSTATUS(statusFils));
                        
                        }else{

                            fprintf(stderr,"Le fils s'est terminé avec une anomalie de statu : %d\n",WEXITSTATUS(statusFils));
                        
                        } 

                    }

                }

            }

            usr1_receive = 0;
            break;

        }

              

    }  

    return 0;
}