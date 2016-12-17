#ifndef __COMMANDES_H__
#define __COMMANDES_H__

/* Processus pour montrer des tests */
void test (void *n);

/* Nettoie l'ecran */
void clear ();

/* Nettoie l'ecran et supprime l'historique */
void reset ();

/* Hello world ! */
void hello ();

/* Aleatoire entre 0 et n */
void rand (void *n);

/* Initialise la suite aleatoire */
void init_rand (void *n);

/* Temps apres lequel on lance l'ecran de veille */
void set_veille (void *n);

/* Processus infini */
void infinity ();

/* Factorielle de n */
void fact (void *n);

/* n more beers... (n-1) beers... */
void beer (void *n) ;

/* Dors n secondes */
void sleep (void *n);

/* Donne le temps d'allumage du systeme */
void time ();

/* Affiche la liste des utilisateurs */
void users ();

/* Liste des commandes */
void help ();

/* Passer en mode super utilisateur */
void su ();

/* Tue un processus */
void kill (void *n);

/* Quitte le mode super utilisateur ou quitte le shell */
void quitter (void* arret);

/* Deconnexion */
void deconnexion (void* arret);

/* Liste des processus */
void jobs ();

/* Jeu de devine le nombre (+ ou -) */
void devine (void *n);

/* Triangle de Pascal de profondeur n */
void pascal (void *n);

/* Suite de Fibonacci jusqu'au rang n */
void fibonacci (void *n);

/* Dessine un joli triangle de n lignes */
void triangle (void *n);

#endif
