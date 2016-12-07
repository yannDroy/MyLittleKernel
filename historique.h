#ifndef __HISTORIQUE_H__
#define __HISTORIQUE_H__

#define TAILLE_HISTORIQUE 100

/* Ajoute une commande a l'historique */
void ajouter_historique (char* s);

/* Copie la ligne d'historique dans le buffer */
void copier_dans_buffer (int8_t sens);

#endif
