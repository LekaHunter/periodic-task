#include "message.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char *argv[]){

    //ouvrir un fichier qui se nomme myself.pid si le fichier n'existe pas il est créé
    FILE *fp = fopen("/tmp/period.pid","w");

    if(fp==NULL){

        perror("fopen");
        exit(1);

    }

    pid_t pid = getpid();

    printf("%d\n",pid);

    //écrir le pid du programme dans le fichier ouvert
    fwrite(&pid, sizeof(pid_t), 1, fp);

    //fermer le fichier
    int c = fclose(fp);

    if(c == EOF){

        perror("fclose");
        exit(2);

    }

    return 0;
}