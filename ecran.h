#ifndef __ECRAN_H__
#define __ECRAN_H__

#include <inttypes.h>

#define MEM_ECRAN     0xB8000

#define PORT_IN_CUR   0x3D4
#define PORT_OUT_CUR  0x3D5

#define NB_LIGNES   25
#define NB_COLONNES 80

#define PREMIERE_LIGNE    1
#define PREMIERE_COLONNE  0

#define C_MAJ_CLOCK  NB_COLONNES - 9
#define C_MAJ_NOM    1
#define C_MAJ_PROC  11
#define C_MAJ_USER  29
#define C_PIPE1      9
#define C_PIPE2     27
#define C_PIPE3     NB_COLONNES - 11

/* Renvoie l'adresse de la case de l'ecran aux coordonnees (lig,col) */
uint16_t *ptr_mem (uint32_t lig, uint32_t col);

/* Ecrit le caractere c aux coordonnees (lig,col) de l'ecran */
void ecrit_car (uint32_t lig, uint32_t col, char c);

/* Efface l'ecran */
void efface_ecran (uint32_t depart);

/* Place le curseur aux coordonnees (lig,col) */
void place_curseur(uint32_t lig, uint32_t col);

/* Traite le caractere c */
void traite_car (char c);

/* Fait defiler l'ecran d'une ligne vers le haut */
void defilement ();

/* Affiche une chaine de caracteres a l'ecran */
void console_putbytes(char *chaine, int32_t taille);

/* Met a jour la GUI */
void maj_GUI (char *chaine, uint32_t col, uint8_t f);

/* Affiche la barre de taches */
void creer_barre ();

/* Initie les affichages GUI */
void init_affichage ();

#define FORMAT_NOIR      0x00
#define FORMAT_BLEU      0x01
#define FORMAT_VERT      0x02
#define FORMAT_CYAN      0x03
#define FORMAT_ROUGE     0x04
#define FORMAT_MAGENTA   0x05
#define FORMAT_MARRON    0x06
#define FORMAT_GRIS      0x07
#define FORMAT_GRIS_F    0x08
#define FORMAT_BLEU_C    0x09
#define FORMAT_VERT_C    0x0A
#define FORMAT_CYAN_C    0x0B
#define FORMAT_ROUGE_C   0x0C
#define FORMAT_MAGENTA_C 0x0D
#define FORMAT_JAUNE     0x0E
#define FORMAT_BLANC     0x0F
#define FORMAT_NOIR_FOND      0x70
#define FORMAT_BLEU_FOND      0x71
#define FORMAT_VERT_FOND      0x72
#define FORMAT_CYAN_FOND      0x73
#define FORMAT_ROUGE_FOND     0x74
#define FORMAT_MAGENTA_FOND   0x75
#define FORMAT_MARRON_FOND    0x76
#define FORMAT_GRIS_FOND      0x77
#define FORMAT_GRIS_F_FOND    0x78
#define FORMAT_BLEU_C_FOND    0x79
#define FORMAT_VERT_C_FOND    0x7A
#define FORMAT_CYAN_C_FOND    0x7B
#define FORMAT_ROUGE_C_FOND   0x7C
#define FORMAT_MAGENTA_C_FOND 0x7D
#define FORMAT_JAUNE_FOND     0x7E
#define FORMAT_BLANC_FOND     0x7F

#endif
