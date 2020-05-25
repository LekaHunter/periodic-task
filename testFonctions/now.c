#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main(){

    time_t now = time(NULL);

    printf("%ld\n",now);

    return 0;
}