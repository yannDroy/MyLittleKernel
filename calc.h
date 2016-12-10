#ifndef __CALC_H__
#define __CALC_H__

#include <inttypes.h>

#define MAX_OPERANDES 32

#define TAILLE_OPERANDE 16

/* Calculatrice en ligne de commandes (notation post-fixee) */
void calc ();

/* Calcule l'expression entree */
void calculer_expression (liste p, int32_t n, char **tokens);

/* Decoupe l'expression entree */
void decouper_expression (char* chaine, char **tokens, int32_t *n);

/* Recupere les deux operandes */
void recuperer_operandes (liste p, int32_t *x, int32_t *y);

#endif
