#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * send_string est une focntion qui envoie une chaine de caractères via un descripteur de fichier
 */

int send_string(int fd, const char *str);

/**
 * recv_string est une fonction qui recoit une chaine de caractère via un descripteur de fichier 
 */

char *recv_string(int fd);

/** 
 * send_argv est une fonction qui envoie un tableau de chaines de caracteres via un descripteur de fichier
 */

int send_argv(int fd, char *argv[]);

/** 
 * recv_argv est une fonction qui recoit un tableau de chaines de caracteres via un descripteur de fichier
 */

char **recv_argv(int fd);

/** 
 * procExPeriod est une focntion qui test si un fichier existe, et lit un pid si le cas echéant
 * @param path Le chemin vers le fichier
*/

void procExPeriod(const char *path, pid_t *pid);

#endif /* MESSAGE_H */