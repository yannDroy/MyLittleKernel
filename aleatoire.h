#ifndef __ALEATOIRE_H__
#define __ALEATOIRE_H__

#include <inttypes.h>

/* Initialise la graine de la suite pseudo-aleatoire */
void srand (int32_t n);

/* Renvoie un nombre aleatoire */
int32_t crand48 ();

#endif
