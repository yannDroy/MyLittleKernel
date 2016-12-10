#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <cpu.h>
#include "ordonnancement.h"
#include "shell.h"
#include "horloge.h"
#include "gui.h"
#include "ecran.h"
#include "aleatoire.h"

extern void ctx_sw (int32_t *ancien_contexte, int32_t *nouveau_contexte);

extern char utilisateur[TAILLE_LOGIN];

extern int8_t barre_existe;

Processus table_processus[TAILLE_TABLE_PROCESSUS];
uint32_t nombre_processus;
int32_t indice_actif;
int32_t indice_dernier_attente = -1;

uint8_t mon_pid () {
    return table_processus[indice_actif].pid;
}

char *mon_nom () {
    return table_processus[indice_actif].nom;
}

void ordonnance () {
    int8_t tmp;

    cli();

    if(table_processus[indice_actif].etat != ENDORMI
       && table_processus[indice_actif].etat != ATTENTE_TERM
       && table_processus[indice_actif].etat != MORT)
        table_processus[indice_actif].etat = ACTIVABLE;

    tmp = indice_actif;
    
    do{
        indice_actif = (indice_actif + 1) % NB_MAX_PROCESSUS;
        
        if(table_processus[indice_actif].etat == ENDORMI
           && nbr_secondes() >= table_processus[indice_actif].heure_reveil){
            table_processus[indice_actif].heure_reveil = -1;
            break;
        }
    }while(table_processus[indice_actif].etat != ACTIVABLE);

    table_processus[indice_actif].etat = ELU;
    
    ctx_sw((int32_t*) &(table_processus[tmp].registres), (int32_t*) &(table_processus[indice_actif].registres));

    sti();
}

int32_t creer_processus (void (*code)(), char *nom, void *param) {
    int32_t i;
    int32_t nouveau_pid;

    cli();

    if(nombre_processus == NB_MAX_PROCESSUS)
        return -1;

    nouveau_pid = -1;

    for(i = 1; i < NB_MAX_PROCESSUS; i++){
        if(table_processus[i].pid < 0 || table_processus[i].etat == MORT){
            table_processus[i].pid = i;
            table_processus[i].ppid = mon_pid();
            strcpy(table_processus[i].nom, nom);

            if(!strcmp(utilisateur, "root"))
                table_processus[i].proprietaire = ROOT;
            else
                table_processus[i].proprietaire = UTILISATEUR;
            
            table_processus[i].etat = ACTIVABLE;
            table_processus[i].heure_reveil = -1;
            table_processus[i].a_attendre = 0;
            
            table_processus[i].registres[1] = (int32_t) &(table_processus[i].pile_execution[TAILLE_PILE_EXEC - 3]);

            table_processus[i].pile_execution[TAILLE_PILE_EXEC - 1] = (int32_t) param;
            table_processus[i].pile_execution[TAILLE_PILE_EXEC - 2] = (int32_t) &fin_processus;
            table_processus[i].pile_execution[TAILLE_PILE_EXEC - 3] = (int32_t) code;

            maj_nb_processus(1);

            nouveau_pid = i;
            
            break;
        }
    }

    sti();

    return nouveau_pid;
}

void maj_nb_processus (int8_t p) {
    char chaine[16];

    cli();
    
    nombre_processus += p;

    if(barre_existe){
        sprintf(chaine, "PROCESSUS : %3d", nombre_processus);
        maj_GUI(chaine, C_MAJ_PROC, TEXTE_BLEU | FOND_GRIS);
    }

    sti();
}

void fin_processus () {
    cli();
    
    if(table_processus[indice_actif].a_attendre)
        table_processus[indice_actif].etat = ATTENTE_TERM;
    else
        tuer_processus(indice_actif, 0);

    sti();
    
    ordonnance();
}

void tuer_processus (int32_t pid, int8_t rec) {
    int32_t i;
    
    if(rec)
        cli();
    
    if(pid != 0){
        if(table_processus[pid].etat != MORT){
            for(i = 0; i < NB_MAX_PROCESSUS; i++){
                if(table_processus[i].ppid == pid && table_processus[i].etat != MORT)
                    tuer_processus(table_processus[i].pid, 0);
            }

            table_processus[pid].etat = MORT;
            maj_nb_processus(-1);
        }else{
            printf("Aucun processus a tuer\n");
        }
    }else{
        printf("Impossible de tuer le processus init\n");
    }

    if(rec)
        sti();
}

void attendre_terminaison (int32_t pid) {
    cli();
    
    table_processus[pid].a_attendre = 1;
    indice_dernier_attente = pid;
    
    while(table_processus[pid].etat != ATTENTE_TERM)
        ordonnance();

    tuer_processus(pid, 0);

    sti();
}

int8_t init_table_processus () {
    int32_t i;

    cli();

    nombre_processus = 0;
    
    for(i = 0; i < TAILLE_TABLE_PROCESSUS; i++){
        table_processus[i].pid = -1;
        table_processus[i].ppid = -1;
        table_processus[i].etat = MORT;
        strncpy(table_processus[i].nom, "\0", 1);
        table_processus[i].heure_reveil = -1;
        table_processus[i].a_attendre = 0;
        table_processus[i].proprietaire = -1;
    }
    
    if(creer_processus_init() < 0){
        printf("Impossible de creer le processus init\n");
        return -1;
    }

    sti();
    
    return 0;
}

int32_t creer_processus_init () {
    cli();
    
    if(nombre_processus == NB_MAX_PROCESSUS){
        return -1;
    }else{
        table_processus[0].pid = 0;
        table_processus[0].ppid = -1;
        strncpy(table_processus[0].nom, "init\0", 4);
        table_processus[0].etat = ELU;
        table_processus[0].heure_reveil = -1;
        table_processus[0].a_attendre = 0;
        table_processus[0].proprietaire = ROOT;

        indice_actif = 0;
        
        nombre_processus = 1;

        return 0;
    }

    sti();
}
