#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if(argc != 2){

        fprintf(stderr,"Usage : ./when [N]\n");
        exit(1);

    }

    time_t sec = atoi(argv[1]);

    char *res = ctime(&sec);

    printf("%s\n",res);

    return 0;
}