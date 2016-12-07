#ifndef __GUI_H__
#define __GUI_H__

#include "ecran.h"

#define TAILLE_LOGIN 21

#define C_MAJ_CLOCK  NB_COLONNES - 9
#define C_MAJ_NOM    1
#define C_MAJ_PROC  11
#define C_MAJ_USER  29
#define C_PIPE1      9
#define C_PIPE2     27
#define C_PIPE3     NB_COLONNES - 11

#define L_NOM PREMIERE_LIGNE + 10
#define C_NOM 40

#define L_LOGIN 15
#define C_LOGIN 19

#define L_IDENT_INCORR C_LOGIN
#define C_IDENT_INCORR C_LOGIN

#define L_VEILLE 12
#define C_VEILLE 30

/* Met a jour la GUI */
void maj_GUI (char *chaine, uint32_t col, uint8_t f);

/* Affiche la barre de taches */
void creer_barre ();

/* Initie les affichages GUI */
void init_affichage ();

/* Champs d'identification */
void prompt_login ();

/* Dessine 'MLK' en ASCII Art a l'ecran de login */
void dessine_MLK ();

/* Indique que les identifiants entrés sont incorrects */
void identifiants_incorrects ();

/* Caractere devant le champ a remplir */
void etoile_champ (int32_t i, char c);

/* Processus de login */
void login ();

/* Choisit la couleur pour l'ecran de veille */
void choisir_theme (int32_t n, int8_t i);

/* Ecran de veille */
void ecran_veille ();

#endif
