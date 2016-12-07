#include <inttypes.h>
#include <string.h>
#include "gui.h"
#include "malloc.h"
#include "shell.h"
#include "ecran.h"
#include "clavier.h"
#include "historique.h"

extern int32_t ligne;
extern int32_t colonne;
extern uint16_t format;

extern char buffer[MAX_TAILLE_BUFFER];
extern int32_t indice;

extern char utilisateur[TAILLE_LOGIN];

char historique[TAILLE_HISTORIQUE][MAX_TAILLE_BUFFER];
int32_t prochain = 0;
int32_t a_copier = 0;

void ajouter_historique (char* s) {
    int32_t i;

    if(strcmp(s, "")){
        if(prochain == TAILLE_HISTORIQUE){
            for(i = 1; i < TAILLE_HISTORIQUE; i++)
                strcpy(historique[i - 1], historique[i]);
        
            strcpy(historique[prochain - 1], s);
        }else{
            strcpy(historique[prochain], s);
            prochain++;
            a_copier = prochain;
        }
    }
}

void charger_dans_buffer (int8_t sens) {
    if(sens < 0){
        if(a_copier > 0){
            a_copier--;
            copier(historique[a_copier]);
        }
    }else{
        if(a_copier < prochain - 1){
            a_copier++;
            copier(historique[a_copier]);
        }else if(a_copier < prochain){
            a_copier++;
            copier("");
        }
    }
}

void copier (char *s) {
    int32_t i;
    
    vider_buffer();
    
    printf("\r");
    prompt_shell();
    
    strcpy(buffer, s);
    indice = strlen(s);
        
    printf("%s", buffer);

    for(i = 0; i < TAILLE_COMMANDE * MAX_TOKEN - strlen(buffer); i++)
        printf(" ");
    for(i = 0; i < TAILLE_COMMANDE * MAX_TOKEN - strlen(buffer); i++)
        printf("\b");
}
