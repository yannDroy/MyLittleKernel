#ifndef __SHELL_C__
#define __SHELL_C__

#define TAILLE_COMMANDE 64
#define MAX_TOKEN 16

/* Processus shell basique */
void shell ();

/* Decoupe la commande entree par l'utilisateur */
void decouper_commande (char* chaine, char tokens[MAX_TOKEN][TAILLE_COMMANDE]);

/* Execute une commande shell */
void executer_commande (char tokens[MAX_TOKEN][TAILLE_COMMANDE]);

/* Nettoie l'ecran */
void clear ();

/* Hello world! */
void hello ();

/* Aleatoire */
void rand ();

/* Liste des commandes */
void help ();

/* Liste des processus */
void jobs ();

/* Conversion d'une chaine en nombre */
int32_t atoi (char* c);

#endif
