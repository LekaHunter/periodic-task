#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/types.h>
#include <unistd.h>

/** 
 * creationOuvrirTube est une fonction qui cree un tube nommé puis l'ouvrir
 * @param path Le chemin vers le tube nommé
*/

void creationOuvrirTube(char *path, int *fd);

/** 
 * closeTube est une fonction qui ferme un tube nommé
 * @param fd Le descripteur du tube nommé
*/

void closeTube(int fd);

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

int procExPeriod(const char *path, pid_t *pid);

/** 
 * affiUsage est une fonction qui affiche l'usage du programme periodic
*/

void affiUsage();

/** 
 * argvValidite est une fonction qui test la validitée des argumens passer à periodic
 * @param argv La liste des arguments passer à periodic
 * @return La fonction return un int 0 si tous se passe bien sinon il y a une erreur
*/

int argvValidite(int argc, char *argv[], long *date, long *period);

#endif /* MESSAGE_H */