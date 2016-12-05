#ifndef __RUBIKS_H__
#define __RUBIKS_H__

#include <inttypes.h>

/* Initialise le cube */
void init_cube ();

/* Test de resolution du cube */
int32_t est_resolu ();

/* Tourne la face R (idem L, U, ...) de n quarts de tour */
void turn_R_face (int32_t n);
void turn_L_face (int32_t n);
void turn_U_face (int32_t n);
void turn_D_face (int32_t n);
void turn_F_face (int32_t n);
void turn_B_face (int32_t n);

/* Affiche le Rubik's cube */
void afficher_cube ();

/* Melange le Rubik's Cube avec n mouvements */
void melanger (int32_t n);

/* Jeu de Rubik's Cube */
void rubiks ();

#endif
