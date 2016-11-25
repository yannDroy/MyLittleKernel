#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "ecran.h"
#include "clavier.h"
#include "aleatoire.h"
#include "processus.h"
#include "horloge.h"
#include "ordonnancement.h"
#include "shell.h"

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern char utilisateur[TAILLE_LOGIN];
extern uint8_t format;
extern uint32_t nombre_processus;
extern int32_t indice_actif;

void shell () {
    char commande[MAX_TAILLE_BUFFER];
    char tokens[MAX_TOKEN][TAILLE_COMMANDE];
    
    sti();

    format = FORMAT_JAUNE;
    printf("\n  *** MyLittleKernel 0.1 ***\n\n");

    do{
        format = FORMAT_BLEU_C;
        printf("%s$> %d ", utilisateur, mon_pid());
        format = FORMAT_BLANC;
        lire_clavier(commande, MAX_TAILLE_BUFFER, VISIBLE);
        decouper_commande(commande, tokens);
        executer_commande(tokens);
        strcpy(tokens[0], "");
    }while(strncmp(commande, "exit", 4));

    format = FORMAT_BLANC;
}

void decouper_commande (char* chaine, char tokens[MAX_TOKEN][TAILLE_COMMANDE]) {
    char *token;
    int32_t i;

    i = 0;
    
    while((token = strtok(chaine, " ")) != NULL){
        if(strcmp(token, "")){
            strncpy(tokens[i], token, strlen(token));
            i++;
        }
        chaine = NULL;
    }

    strcpy(tokens[i], "");
}

void executer_commande (char tokens[MAX_TOKEN][TAILLE_COMMANDE]) {
    int32_t pid;
    
    if(!strncmp(tokens[0], "exit", 4)){
        if(!strcmp(tokens[1], ""))
            printf("Bye.\n");
        else
            printf("Commande inconnue !\n");
    }else if(!strncmp(tokens[0], "clear", 5)) {
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&clear, "clear");
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Commande inconnue !\n");
        }
    }else if(!strncmp(tokens[0], "hello", 4)){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&hello, "hello");
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Commande inconnue !\n");
        }
    }else if(!strncmp(tokens[0], "jobs", 3)){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&jobs, "jobs");
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Commande inconnue !\n");
        }
    }else if(!strncmp(tokens[0], "help", 3)){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&help, "help");
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Commande inconnue !\n");
        }
    }else if(!strncmp(tokens[0], "rand", 4)){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&rand, "rand");
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Commande inconnue !\n");
        }
    }else if(strcmp(tokens[0], "") != 0){
        printf("Commande inconnue !\n");
    }
}

void clear () {
    sti();
    printf("\f");
}

void hello () {
    sti();
    printf("Hello world!\n");
}

void rand () {
    sti();
    printf("Nombre aleatoire : %d\n", crand48());
}

void help () {
    sti();
}

void jobs () {
    int32_t i;
    int32_t j;
    
    sti();

    printf("Liste des processus (%d) :\n", nombre_processus);
    printf("  _______________________________________\n");
    printf(" | PID |         nom         |    etat   |\n");
    printf(" |-----+---------------------+-----------|\n");
    
    for(i = 0; i < NB_MAX_PROCESSUS; i++){
        if(table_processus[i].pid >= 0){
            if(table_processus[i].pid < 10)
                printf(" |  %d  |", table_processus[i].pid);
            else if(table_processus[i].pid < 100)
                printf(" |  %d |", table_processus[i].pid);

            for(j = 0; j < 20 - strlen(table_processus[i].nom); j++)
                printf(" ");
            printf("%s ", table_processus[i].nom);

            switch(table_processus[i].etat){
            case MORT:
                printf("|    MORT   |\n");
                break;
            case ELU:
                printf("|    ELU    |\n");
                break;
            case ACTIVABLE:
                printf("| ACTIVABLE |\n");
                break;
            case ENDORMI:
                printf("|  ENDORMI  |\n");
                break;
            case ATTENTE_TERM:
                printf("| ATTENTE_T |\n");
                break;
            default:
                break;
            }
        }
    }

    printf(" '-----+---------------------+-----------'\n");
}

int32_t atoi (char* c) {
    int32_t valeur;
    int32_t signe;

    valeur = 0;
    signe = 1;
    
    if(*c == '+' || *c == '-'){
       if(*c == '-')
           signe = -1;
       c++;
    }
    
    while(*c >= '0' && *c <= '9'){
        valeur *= 10;
        valeur += (int32_t) (*c - '0');
        c++;
    }
    
    return valeur * signe;
}
