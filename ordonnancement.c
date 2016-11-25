#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <cpu.h>
#include "ordonnancement.h"
#include "shell.h"
#include "horloge.h"
#include "ecran.h"
#include "aleatoire.h"

extern void ctx_sw (int32_t *ancien_contexte, int32_t *nouveau_contexte);

Processus table_processus[TAILLE_TABLE_PROCESSUS];
uint32_t nombre_processus;
int32_t indice_actif;

uint8_t mon_pid () {
    return table_processus[indice_actif].pid;
}

char *mon_nom () {
    return table_processus[indice_actif].nom;
}

void ordonnance () {
    int8_t tmp;

    cli();

    if(table_processus[indice_actif].etat != ENDORMI && table_processus[indice_actif].etat != ATTENTE_TERM && table_processus[indice_actif].etat != MORT)
        table_processus[indice_actif].etat = ACTIVABLE;

    tmp = indice_actif;
    
    do{
        indice_actif = (indice_actif + 1) % NB_MAX_PROCESSUS;

        if(table_processus[indice_actif].etat == ENDORMI && nbr_secondes() >= table_processus[indice_actif].heure_reveil){
            table_processus[indice_actif].heure_reveil = -1;
            break;
        }
    }while(table_processus[indice_actif].etat != ACTIVABLE);

    table_processus[indice_actif].etat = ELU;

    sti();
    
    ctx_sw((int32_t*) &(table_processus[tmp].registres), (int32_t*) &(table_processus[indice_actif].registres));
}

int32_t creer_processus (void (*code)(), char *nom) {
    char chaine[16];
    int32_t i;
    int32_t nouveau_pid;

    cli();

    if(nombre_processus == NB_MAX_PROCESSUS)
        return -1;

    for(i = 1; i < NB_MAX_PROCESSUS; i++){
        if(table_processus[i].pid < 0 || table_processus[i].etat == MORT){
            printf("ah je peux creer le %d\n", i);
            table_processus[i].pid = i;
            strncpy(table_processus[i].nom, nom, strlen(nom));
            
            table_processus[i].etat = ACTIVABLE;
            table_processus[i].heure_reveil = -1;
            
            table_processus[i].registres[1] = (int32_t) &(table_processus[i].pile_execution[TAILLE_PILE_EXEC - 2]);
            
            table_processus[i].pile_execution[TAILLE_PILE_EXEC - 1] = (int32_t) &fin_processus;
            table_processus[i].pile_execution[TAILLE_PILE_EXEC - 2] = (int32_t) code;

            nombre_processus++;

            if(nombre_processus < 10)
                sprintf(chaine, "PROCESSUS :   %d", nombre_processus);
            else if(nombre_processus < 100)
                sprintf(chaine, "PROCESSUS :  %d", nombre_processus);
            else
                sprintf(chaine, "PROCESSUS : %d", nombre_processus);
            
            maj_GUI(chaine, C_MAJ_PROC, FORMAT_BLEU_FOND);

            printf("pid cree: %d\n", i);
            nouveau_pid = i;
            
            break;
        }
        printf("peux pas creer le %d : %d %s\n", i, table_processus[i].pid, table_processus[i].nom);
    }

    sti();

    return nouveau_pid;
}

int32_t creer_processus_init () {    
    if(nombre_processus == NB_MAX_PROCESSUS){
        return -1;
    }else{
        table_processus[0].pid = 0;
        strncpy(table_processus[0].nom, "idle", 4);
        table_processus[0].etat = ELU;
        table_processus[0].heure_reveil = -1;

        indice_actif = 0;
        nombre_processus = 1;
        maj_GUI("PROCESSUS :   1", C_MAJ_PROC, FORMAT_BLEU_FOND);

        return 0;
    }
}

void fin_processus () {
    char chaine[16];
    
    table_processus[indice_actif].etat = ATTENTE_TERM;
    nombre_processus--;

    if(nombre_processus < 10)
        sprintf(chaine, "PROCESSUS :   %d", nombre_processus);
    else if(nombre_processus < 100)
        sprintf(chaine, "PROCESSUS :  %d", nombre_processus);
    else
        sprintf(chaine, "PROCESSUS : %d", nombre_processus);

    maj_GUI(chaine, C_MAJ_PROC, FORMAT_BLEU_FOND);

    ordonnance();
}

void attendre_terminaison (int32_t pid) {
    while(table_processus[pid].etat != ATTENTE_TERM)
        ordonnance();

    table_processus[pid].pid = -1;
    table_processus[pid].etat = MORT;
}

int8_t init_table_processus () {
    int32_t i;
    
    for(i = 0; i < TAILLE_TABLE_PROCESSUS; i++)
        table_processus[i].pid = -1;
    
    if(creer_processus_init() < 0){
        printf("[ERREUR] Impossible de creer le processus init\n");
        return -1;
    }

    jobs();

    return 0;
}
