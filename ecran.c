#include <string.h>
#include <stdio.h>
#include <cpu.h>
#include "ecran.h"

uint32_t ligne = PREMIERE_LIGNE;
uint32_t colonne = PREMIERE_COLONNE;

uint8_t format = TEXTE_BLANC | FOND_NOIR;

uint16_t *ptr_mem (uint32_t lig, uint32_t col) {
    return (uint16_t*) (MEM_ECRAN + 2 * (lig * NB_COLONNES + col));
}

void ecrit_car (uint32_t lig, uint32_t col, char c) {
    uint16_t *adresse;

    adresse = ptr_mem(lig, col);
    *adresse = c | (format << 8);
}

void efface_ecran (uint32_t depart) {
    uint32_t i, j;
    uint8_t save_format;

    save_format = format;
    format = TEXTE_NOIR | FOND_NOIR;

    for(i = depart; i < NB_LIGNES; i++){
        for(j = 0; j < NB_COLONNES; j++) {
            ecrit_car(i, j, ' ');
        }
    }

    ligne = PREMIERE_LIGNE;
    colonne = PREMIERE_COLONNE;

    format = save_format;
    place_curseur(depart, PREMIERE_COLONNE);
}

void place_curseur (uint32_t lig, uint32_t col) {
    uint16_t position;

    position = (NB_COLONNES * lig) + col;

    outb(0x0F, PORT_OUT_CUR);
    outb(position & 0x00FF, PORT_OUT_CUR);

    outb(0x0E, PORT_OUT_CUR);
    outb(position >> 8, PORT_OUT_CUR);
}

void traite_car (char c) {
    switch(c) {
    case '\b' :
        if(colonne > PREMIERE_COLONNE){
            place_curseur(ligne, --colonne);
        }else{
            if(ligne > PREMIERE_LIGNE){
                colonne = NB_COLONNES - 1;
                ligne--;

                place_curseur(ligne, colonne);
            }
        }
            
        break;
    case '\t' :
        colonne = 8 * ((colonne / 8) + 1);
        if(colonne > NB_COLONNES - 1)
            colonne = NB_COLONNES - 1;
        
        place_curseur(ligne, colonne);
        break;
    case '\n' :
        ligne++;
        colonne = PREMIERE_COLONNE;

        place_curseur(ligne, colonne);
        break;
    case '\f' :
        efface_ecran(PREMIERE_LIGNE);
        ligne = PREMIERE_LIGNE;
        colonne = PREMIERE_COLONNE;
        
        place_curseur(ligne, colonne);
        break;
    case '\r' :
        colonne = PREMIERE_COLONNE;
        
        place_curseur(ligne, colonne);
        break;
    default:
        if(colonne >= NB_COLONNES){
            colonne = PREMIERE_COLONNE;
            ligne++;

            if(ligne >= NB_LIGNES){
                ligne = NB_LIGNES - 1;
                defilement();
            }

            place_curseur(ligne, colonne);
        }else if(ligne >= NB_LIGNES){
            ligne = NB_LIGNES - 1;
            defilement();

            place_curseur(ligne, colonne);
        }
        
        ecrit_car(ligne, colonne, c);

        colonne++;
        place_curseur(ligne, colonne);
    }
}

void defilement () {
    int32_t i;
    uint8_t save_format;
    
    memmove(ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), ptr_mem(PREMIERE_LIGNE + 1, PREMIERE_COLONNE), (NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t)));

    save_format = format;
    format = TEXTE_NOIR | FOND_NOIR;

    for(i = 0; i < NB_COLONNES; i++)
        ecrit_car(NB_LIGNES - 1, i, ' ');

    format = save_format;
}

void console_putbytes (char *chaine, int32_t taille) {
    int i;

    for(i = 0; i < taille; i++)
        traite_car(chaine[i]);
}
