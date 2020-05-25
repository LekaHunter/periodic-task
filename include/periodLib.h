#ifndef PERIODLIB_H
#define PERIODLIB_H

#include <stddef.h>
#include <stdbool.h>

/** 
 * Structure d'une commande
*/

struct cmd {

    char **nameAndArgs;
    long date;
    long periode;

};

/** 
 * Structure d'une la liste de commande
*/

struct array {

    struct cmd *listCmd;
    size_t capacity;
    size_t size;

};

/** 
 * Fonction pour creer et ouvrir un tube
 * @param path  Le chemin vers le fichier
*/

void creationOuvrirTube(char *path);

/** 
 * Fonction qui ferme un descripteur de fichier
 * @param fd    Le descripteur de fichier à fermer
*/

void closeTube(int fd);

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

void array_add(struct array *self, struct cmd newAdd);

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

#endif /* PERIODLIB_H */