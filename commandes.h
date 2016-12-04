#ifndef __COMMANDES_H__
#define __COMMANDES_H__

/* Nettoie l'ecran */
void clear ();

/* Hello world ! */
void hello ();

/* Aleatoire entre 0 et n */
void rand (void *n);

/* Initialise la suite aleatoire */
void init_rand (void *n);

/* Processus infini */
void infinity ();

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

/* Tue un processus */
void kill (void *n);

/* Quitte le mode super utilisateur ou quitte le shell */
void quitter ();

/* Liste des processus */
void jobs ();

#endif
