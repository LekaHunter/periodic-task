#include "message.h"

#include <stdio.h>
#include <stdlib.h>

struct cmd {

    char *nameAndArgs;
    long date;
    long periode;

};

int main(){

    struct cmd cmd1;
    cmd1.nameAndArgs = "ls -la ../";
    cmd1.date = 10;
    cmd1.periode = 5;

    struct cmd cmd2;
    cmd2.nameAndArgs = "echo bonjour";
    cmd2.date = 154875123154;
    cmd2.periode = 10;

    struct array listCmd;

    array_create(&listCmd);

    printf("%ld\n",listCmd.size);

    struct cmd *ptr = array_get(&listCmd, 0);

    if(ptr == NULL){

        printf("coulé\n");

    }

    printf("cp avant add = %ld\n",listCmd.capacity);
    printf("size avant add = %ld\n",array_size(&listCmd));
    printf("\n");

    array_add(&listCmd, cmd1);
    array_add(&listCmd, cmd2);

    printf("cp après add = %ld\n",listCmd.capacity);
    printf("size avant add = %ld\n",array_size(&listCmd));
    printf("\n");

    for(size_t i = 0; i < listCmd.size; i++){

        printf("index de recherche de cmd = %ld\n",i);
        printf("%s\n",listCmd.listCmd[i].nameAndArgs);
        printf("%ld\n",listCmd.listCmd[i].date);
        printf("%ld\n",listCmd.listCmd[i].periode);
        printf("\n");

    }

    size_t t = array_search(&listCmd,cmd2);

    printf("index de recherche de cmd1 = %ld\n",t);

    ptr = array_get(&listCmd, 1);

    if(ptr != NULL){

        printf("coulé\n");

    }

    printf("%s\n",(*ptr).nameAndArgs);
    printf("%ld\n",(*ptr).date);
    printf("%ld\n",(*ptr).periode);
    printf("\n");

    array_remove(&listCmd, 1);

    for(size_t i = 0; i < listCmd.size; i++){

        printf("%s\n",listCmd.listCmd[i].nameAndArgs);
        printf("%ld\n",listCmd.listCmd[i].date);
        printf("%ld\n",listCmd.listCmd[i].periode);
        printf("\n");

    }

    array_destroy(&listCmd);

    return 0;
}