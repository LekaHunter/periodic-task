#ifndef PERIODICLIB_H
#define PERIODICLIB_H

#include <sys/types.h>

 /** 
 * Affichage de l'usage de periodic.c
*/

void affiUsage();

/** 
 * procExPeriod est une focntion qui test si un fichier existe, et lit un pid si le cas echéant
 * @param path  Le chemin vers le fichier
 * @result pid_t    La fonction renvoit le pid écrit dans '/tmp/period.pid' sinon il s'arrete avec les code 1, 2, 3, 4  
*/

pid_t procExPeriod(const char *path);

/** 
 * Fonction qui ferme un descripteur de fichier
 * @param fd    Le descripteur de fichier à fermer
*/

void closeTube(int fd);

/** 
 * argvValidite est une fonction qui test la validitée des argumens passer à periodic
 * @param argv La liste des arguments passer à periodic
 * @result int La fonction return un int 0 si tous se passe bien sinon il y a une erreur
*/

int argvValidite(int argc, char *argv[]);

#endif /* PERIODICLIB_H */