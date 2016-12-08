#ifndef __LISTE_H__
#define __LISTE_H__

#include <inttypes.h>

typedef struct cellule{
  int32_t valeur;
  struct cellule *suivant;
} cellule;

typedef cellule* liste;

/* Renvoie une liste vide */
liste liste_vide();

/* Teste si une liste est vide */
int32_t est_vide(liste l);

/* Insere un element dans la liste l */
liste insere_element(int32_t element, liste l);

/* Renvoie le 1er element de la liste l */
int32_t renvoie_premier_element(liste l);

/* Supprime le premier element de la liste l */
liste supprimer_premier_element(liste l);

/* Affiche une liste */
void affiche_liste(liste l);

#endif
