#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

struct cmd {

    char **nameAndArgs;
    long date;
    long periode;

};

struct array {

    struct cmd *listCmd;
    size_t capacity;
    size_t size;

};

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
 * send_cmd est une foction qui envoie une struct cmd via un descripteur de fichier
*/

int send_cmd(int fd, struct cmd *self);

/** 
 * recv_cmd est une fonction qui recoit une struct cmd via un descripteur de fichier
*/

struct cmd *recv_cmd(int fd);

/** 
 * send_arrayCmd est une fonction qui envoie une liste de commande via un descripteur de fichier
*/

int send_arrayCmd(int fd, struct array *self);

/** 
 * recv_arrayCmd est une focntion qui recoit une liste de commande via un descripteur de fichier
*/

//struct array *recv_arrayCmd(int fd);

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

int argvValidite(int argc, char *argv[]);

/*
 * Create a cmd with a argv
 */
void cmd_create(struct cmd *self, size_t size);

/*
 * Destroy a cmd
 */
void cmd_destroy(struct cmd *self);

/*
 * Create an empty array
 */
void array_create(struct array *self);

/*
 * Destroy an array
 */
void array_destroy(struct array *self);

/*
 * Add a cmd at the end of the array
 */
void array_add(struct array *self, struct cmd cmd);

/*
 * Remove a cmd in the array (preserving the order)
 */
void array_remove(struct array *self, size_t index);

/*
 * Get a pointer to the cmd at the specified index in the array
 */
struct cmd *array_get(const struct array *self, size_t index);

/*
 * Get the size of the array
 */
size_t array_size(const struct array *self);

/* 
 *  Compare two cmd
*/

bool compare_cmd(struct cmd cmd1, struct cmd cmd2);

/*
 * Search for a cmd in the array.
 */
size_t array_search(const struct array *self, struct cmd command);

#endif /* MESSAGE_H */