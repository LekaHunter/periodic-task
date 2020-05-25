#include "../include/periodLib.h"

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
#include <stdbool.h>
#include <stddef.h>
#include <stdbool.h>

void creationOuvrirTube(char *path){

    int res = mkfifo(path, 0644);

    if(res == -1){

        perror("mkfifo");
        exit(1);

    }

}

void closeTube(int fd){

    int closeFd = close(fd);

    if(closeFd == -1){

        perror("close");
        exit(4);

    }

}

void cmd_create(struct cmd *self, size_t size){

    assert(self != NULL);

    self->date = 0;
    self->periode = 0;

    self->nameAndArgs = (char **)calloc(size+1, sizeof(char *));
    self->nameAndArgs[size] = NULL;

}

void cmd_destroy(struct cmd *self){

    assert(self != NULL);

    size_t i = 0;
    while(self->nameAndArgs[i] != NULL){

        free(self->nameAndArgs[i]);
        i++;

    }

    free(self->nameAndArgs);

    self->date = 0;
    self->periode = 0;

}

void array_create(struct array *self){

    assert(self != NULL);

    self->capacity = 100;
    self->size = 0;
    self->listCmd = (struct cmd *)calloc(self->capacity,sizeof(struct cmd));

}

void array_destroy(struct array *self){

    assert(self != NULL);

    for(size_t i = 0; i < self->size; i++){

        cmd_destroy(&(self->listCmd[i]));

    }

    free(self->listCmd);
    self->capacity = 0;
    self->size = 0;

}

void array_add(struct array *self, struct cmd newAdd){

    assert(self != NULL);

    if(self->size == self->capacity){

        self->capacity = self->capacity * 2;

        struct cmd *data = (struct cmd *)calloc(self->capacity,sizeof(struct cmd));

        memcpy(data,self->listCmd,self->size*sizeof(struct cmd));

        free(self->listCmd);

        self->listCmd = data;

    }

    self->listCmd[self->size] = newAdd;
    self->size = self->size + 1;

}

void array_remove(struct array *self, size_t index){

    assert(self != NULL);

    for(size_t i = index + 1; i < self->size; ++i){

        self->listCmd[i - 1] = self->listCmd[i];

    }

    self->size = self->size - 1;

}

struct cmd *array_get(const struct array *self, size_t index){

    assert(self != NULL);

    if(index >= self->size){

        return NULL;
        
    }

    struct cmd *getCmd = &(self->listCmd[index]);
    
    return getCmd;
}

size_t array_size(const struct array *self){

    assert(self != NULL);

    return self->size;

}

bool compare_cmd(struct cmd cmd1, struct cmd cmd2){

    assert(&cmd1 != NULL && &cmd2 != NULL);

    bool testTab = true;
    size_t i = 0;
    while(cmd1.nameAndArgs[i] != NULL && cmd2.nameAndArgs[i] != NULL){

        if(strcmp(cmd1.nameAndArgs[i],cmd2.nameAndArgs[i]) != 0){

            testTab = false;
            break;

        }

        i++;
    }

    if(testTab == true && cmd1.date == cmd2.date && cmd1.periode == cmd2.periode){

        return true;

    }

    return false;
}

size_t array_search(const struct array *self, struct cmd command){

    assert(self != NULL);
  
    size_t index = 0; 

    while(index < self->size && compare_cmd(self->listCmd[index],command) == false){

        index = index + 1;

    }    

    return index;

}