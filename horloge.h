#ifndef __HORLOGE_H__
#define __HORLOGE_H__

#include <inttypes.h>

#define CANAL_CLOCK 0
#define INTERRUPTION_CLOCK 32

#define PORT_OUT_PIT_CLOCK 0x20
#define PORT_COM_CLOCK     0x43
#define PORT_DATA_CLOCK    0x40

#define QUARTZ    0x1234DD
#define CLOCKFREQ 50


/* Fonction assembleur qui traite l'interruption 32 */
extern void traitant_IT_32 ();

/* Renvoie le nombre de secondes ecoulees depuis le demarrage du systeme */
uint32_t nbr_secondes ();

/* Traite une interruption venant de l'horloge */
void tic_PIT ();

/* Initialisation de la frequence de l'horloge */
void init_clock ();

/* Fonction d'endormissement d'un processus */
void dors (uint32_t nbr_secs);

#endif
