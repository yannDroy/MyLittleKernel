#ifndef __SHELL_C__
#define __SHELL_C__

#define TAILLE_COMMANDE 64
#define MAX_TOKEN 10

/* Processus shell */
void shell ();

/* Decoupe la commande entree par l'utilisateur */
void decouper_commande (char* chaine, char **tokens);

/* Execute une commande shell */
void executer_commande (char **tokens);

/* Nettoie l'ecran */
void clear ();

/* Hello world ! */
void hello ();

/* Aleatoire entre 0 et n */
void rand (void *n);

/* Factorielle de n */
void fact (void *n);

/* n more beers... (n-1) beers... */
void beer (void *n) ;

/* Dors n secondes */
void sleep (void *n);

/* Affiche la liste des utilisateurs */
void users ();

/* Liste des commandes */
void help ();

/* Passer en mode super utilisateur */
void su ();

/* Quitte le mode super utilisateur ou quitte le shell */
void quitter ();

/* Liste des processus */
void jobs ();

/* Conversion d'une chaine en nombre */
int32_t atoi (char* c);

#endif
