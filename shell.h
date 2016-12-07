#ifndef __SHELL_H__
#define __SHELL_H__

#include <inttypes.h>

#define TAILLE_COMMANDE 16
#define MAX_TOKEN 3

/* Prompt de l'invite de commandes */
void prompt_shell ();

/* Processus shell */
void shell ();

/* Decoupe la commande entree par l'utilisateur */
void decouper_commande (char* chaine, char **tokens);

/* Execute une commande shell */
void executer_commande (char **tokens, int32_t *arret);

#endif
