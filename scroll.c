#include <inttypes.h>
#include <string.h>
#include "ecran.h"
#include "malloc.h"
#include "scroll.h"

uint16_t *sauvegarde_haut[NB_LIGNES_SAUVEGARDE];
uint16_t *sauvegarde_bas[NB_LIGNES_SAUVEGARDE];
int32_t prochain_haut = 0;
int32_t prochain_bas = 0;
int32_t a_copier_haut = 0;
int32_t a_copier_bas = 0;

void sauvegarde_premiere_ligne () {
    void *save;
    int32_t i;
    
    if(prochain_haut == NB_LIGNES_SAUVEGARDE){
        for(i = 1; i < NB_LIGNES_SAUVEGARDE; i++)
            sauvegarde_haut[i - 1] = sauvegarde_haut[i];

        save = (void*) malloc(NB_COLONNES * sizeof(uint16_t));
        memcpy(save, ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), NB_COLONNES * sizeof(uint16_t));
        
        sauvegarde_haut[prochain_haut - 1] = save;
    }else{
        save = (void*) malloc(NB_COLONNES * sizeof(uint16_t));
        memcpy(save, ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), NB_COLONNES * sizeof(uint16_t));

        sauvegarde_haut[prochain_haut] = save;
        prochain_haut++;
        a_copier_haut = prochain_haut;
    }
}

void sauvegarde_derniere_ligne () {
    void *save;
    int32_t i;
    
    if(prochain_bas == NB_LIGNES_SAUVEGARDE){
        for(i = 1; i < NB_LIGNES_SAUVEGARDE; i++)
            sauvegarde_bas[i - 1] = sauvegarde_bas[i];

        save = (void*) malloc(NB_COLONNES * sizeof(uint16_t));
        memcpy(save, ptr_mem(NB_LIGNES - 1, PREMIERE_COLONNE), NB_COLONNES * sizeof(uint16_t));
        
        sauvegarde_bas[prochain_bas - 1] = save;
    }else{
        save = (void*) malloc(NB_COLONNES * sizeof(uint16_t));
        memcpy(save, ptr_mem(NB_LIGNES - 1, PREMIERE_COLONNE), NB_COLONNES * sizeof(uint16_t));

        sauvegarde_bas[prochain_bas] = save;
        prochain_bas++;
        a_copier_bas = prochain_bas;
    }
}

void supprimer_sauvegarde_haut () {
    prochain_haut--;
}

void supprimer_sauvegarde_bas () {
    prochain_bas--;
}

void scroll_haut () {
    if(a_copier_haut > 0){
        sauvegarde_derniere_ligne();

        a_copier_haut--;
        
        memmove(ptr_mem(PREMIERE_LIGNE + 1, PREMIERE_COLONNE), ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), (NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t)));

        memmove(ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), sauvegarde_haut[a_copier_haut], NB_COLONNES * sizeof(uint16_t));

        supprimer_sauvegarde_haut();
    }
}

void scroll_bas () {
    if(a_copier_bas > 0){
        sauvegarde_premiere_ligne();

        a_copier_bas--;
        
        memmove(ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), ptr_mem(PREMIERE_LIGNE + 1, PREMIERE_COLONNE), (NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t)));

        memmove(ptr_mem(NB_LIGNES - 1, PREMIERE_COLONNE), sauvegarde_bas[a_copier_bas], NB_COLONNES * sizeof(uint16_t));

        supprimer_sauvegarde_bas();
    }
}

void tout_scroller_haut () {
    while(a_copier_haut > 0)
        scroll_haut();
}

void tout_scroller_bas () {
    while(a_copier_bas > 0)
        scroll_bas();
}
