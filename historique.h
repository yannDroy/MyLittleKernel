#ifndef __HISTORIQUE_H__
#define __HISTORIQUE_H__

#define TAILLE_HISTORIQUE 512

/* Ajoute une commande a l'historique */
void ajouter_historique (char* s);

/* Copie la ligne d'historique dans le buffer */
void copier ();

/* Charge une ligne de l'historique */
void charger_dans_buffer (int8_t sens);

/* Vide l'historique */
void vider_historique_commandes ();

#endif
