#ifndef __SCROLL_H__
#define __SCROLL_H__

#include <inttypes.h>

#define NB_LIGNES_SAUVEGARDE 1024

/* Sauvegarde la 1ere ligne de l'ecran */
void sauvegarde_premiere_ligne ();

/* Sauvegarde la derniere ligne de l'ecran */
void Sauvegarde_derniere_ligne ();

/* Supprime la derniere 1ere ligne sauvegardee */
void supprimer_sauvegarde_haut ();

/* Supprime la derniere derniere ligne sauvegardee */
void supprimer_sauvegarde_bas ();

/* Scrolle l'ecran d'une ligne vers le haut */
void scroll_haut ();

/* Scrolle l'ecran d'une ligne vers le bas */
void scroll_bas ();

/* Scrolle tout vers le haut */
void tout_scroller_haut ();

/* Scrolle tout vers le bas */
void tout_scroller_bas ();

/* Sauvegarde une ligne d'indice i */
void sauvegarde_ligne (uint32_t i);

/* Supprime les lignes d'ecran sauvegardees */
void vider_historique_ecran ();

#endif
