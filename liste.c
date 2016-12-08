#include <stdio.h>
#include "malloc.h"
#include "liste.h"

liste liste_vide () {
  return NULL;
}

int32_t est_vide (liste l) {
  return (l == liste_vide());
}

liste insere_element (int element, liste l) {
  liste new = liste_vide();

  new = (liste) malloc(sizeof(cellule));
  new->valeur = element;
  new->suivant = l;
  
  return new;
}

int32_t renvoie_premier_element (liste l) {
  if(est_vide(l)){
    printf("Pas de premier element\n");
    return -1;
  }

  return l->valeur;
}

liste supprimer_premier_element (liste l) {
  liste next = NULL;
  
  if(est_vide(l))
    return l;
  
  next = l->suivant;
  free(l);
  
  return next;
}

void affiche_liste (liste l) {
  liste tmp = l;

  printf("< ");

  while(!est_vide(tmp)){
    printf("%d ",tmp->valeur);
    tmp = tmp->suivant;
  }

  printf(">\n");
}
