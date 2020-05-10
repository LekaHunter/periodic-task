#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 

int main(int argc, char *argv[]){

    if(fork() == 0){

        pid_t lead = setsid();

        if(lead == -1){

            perror("setsid");
            exit(-1);

        }

        if(fork() == 0){

            int changDir = chdir("/");

            if(changDir == -1){

                perror("chdir");
                exit(-1);

            }

            umask(0);

            close(0);
            close(1);
            close(2);

            execlp(argv[1],argv[1],NULL);

        }

        exit(0);

    }

    wait(NULL);

    return 0;
}