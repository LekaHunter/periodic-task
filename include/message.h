#ifndef MESSAGE_H
#define MESSAGE_H

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

#endif /* MESSAGE_H */