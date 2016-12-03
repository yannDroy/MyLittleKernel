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

/* Couleurs du texte */
#define TEXTE_NOIR      0x00
#define TEXTE_BLEU      0x01
#define TEXTE_VERT      0x02
#define TEXTE_CYAN      0x03
#define TEXTE_ROUGE     0x04
#define TEXTE_MAGENTA   0x05
#define TEXTE_MARRON    0x06
#define TEXTE_GRIS      0x07
#define TEXTE_GRIS_F    0x08
#define TEXTE_BLEU_C    0x09
#define TEXTE_VERT_C    0x0A
#define TEXTE_CYAN_C    0x0B
#define TEXTE_ROUGE_C   0x0C
#define TEXTE_MAGENTA_C 0x0D
#define TEXTE_JAUNE     0x0E
#define TEXTE_BLANC     0x0F

/* Couleurs du fond */
#define FOND_NOIR    0x00
#define FOND_BLEU    0x10
#define FOND_VERT    0x20
#define FOND_CYAN    0x30
#define FOND_ROUGE   0x40
#define FOND_MAGENTA 0x50
#define FOND_MARRON  0x60
#define FOND_GRIS    0x70

#endif
