#ifndef MESSAGE_H
#define MESSAGE_H

/**
 * send_string est une focntion qui envoie une chaine de caractères via un descripteur de fichier
 * @param int fd    Le descripteur de fichier ou l'on veut envoyer le string
 * @param char *str     Le string que l'on veut envoyer
 * @result int      La fonction renvoies 1 ou 2 s'il y a une erreur sinon 0 
 */

int send_string(int fd, const char *str);

/**
 * recv_string est une fonction qui recoit une chaine de caractère via un descripteur de fichier 
 * @param int fd    Le descripteur de fichier ou l'on reçoit le string
 * @result char *   Un pointeur qui pointe sur le 1er caractère du string reçus
 */

char *recv_string(int fd);

/** 
 * send_argv est une fonction qui envoie un tableau de chaines de caracteres via un descripteur de fichier
 * @param int fd    Le descripteur de fichier ou l'on veut envoyer le tableau de string
 * @param char *argv[]  Le tableau de String que l'on veut envoyer
 * @result int      La fonction renvoies 1 ou 2 s'il y a une erreur sinon 0
 */

int send_argv(int fd, char *argv[]);

/** 
 * recv_argv est une fonction qui recoit un tableau de chaines de caracteres via un descripteur de fichier
 * @param int fd    Le descripteur de fichier ou l'on reçoit le tableau de string
 * @result char **  Retourn un tableau de string
 */

char **recv_argv(int fd);

#endif /* MESSAGE_H */