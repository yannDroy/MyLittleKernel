#ifndef __TICTACTOE_H__
#define __TICTACTOE_H__

#include <inttypes.h>

typedef struct position {
    int32_t absc;
    int32_t ord;
} position;

/* Affiche le plateau de jeu */
void afficher_matrice ();

/* Recherche de place libre pour l'IA */
void chercher_place_libre ();

/* IA */
void jouer_ia ();

/* Initialisation du tableau */
void initialiser_tab ();

/* Test de position valide */
int32_t est_valide (int32_t choixX , int32_t choixY);

/* Test de fin de partie */
int32_t est_finie ();

/* Jeu de tictactoe */
void tictactoe ();

#endif
