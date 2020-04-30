#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){

    if(argc == 1){

        printf("Liste des commandes dans period\n");
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

    pid_t pid;

    int readPid = procExPeriod("/tmp/period.pid", &pid);

    if(readPid == -1){

        printf("La lecture du pid dans le '/tmp/period.pid' à echouer\n");
        exit(4);

    }

    

    return 0;
}