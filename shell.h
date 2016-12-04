#ifndef __SHELL_H__
#define __SHELL_H__

#include <inttypes.h>

#define TAILLE_COMMANDE 64
#define MAX_TOKEN 10

/* Processus shell */
void shell ();

/* Decoupe la commande entree par l'utilisateur */
void decouper_commande (char* chaine, char **tokens);

/* Execute une commande shell */
void executer_commande (char **tokens);

/* Conversion d'une chaine en nombre */
int32_t atoi (char* c);

#endif
