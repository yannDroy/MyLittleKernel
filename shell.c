#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "ecran.h"
#include "clavier.h"
#include "aleatoire.h"
#include "horloge.h"
#include "gui.h"
#include "ordonnancement.h"
#include "shell.h"
#include "malloc.c.h"

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern char utilisateur[TAILLE_LOGIN];
extern uint8_t format;
extern uint32_t nombre_processus;
extern int32_t indice_actif;

int32_t nb_utilisateurs = 4;
char utilisateurs[][2][TAILLE_LOGIN] = {
    {"root", "toor"},
    {"yann", "Marseille13"},
    {"test", "test"},
    {"invite", ""},
};
char ancien_utilisateur[TAILLE_LOGIN];

int32_t arret;

void shell () {
    char *commande = NULL;
    char **tokens = NULL;
    int32_t i, j;
    
    sti();

    strcpy(ancien_utilisateur, utilisateur);

    format = TEXTE_JAUNE | FOND_NOIR;
    printf("\f\n  *** MyLittleKernel 0.1 ***\n\n");

    arret = 0;
    do{
        commande = (char*) malloc(MAX_TAILLE_BUFFER * sizeof(char));
        for(i = 0; i < MAX_TAILLE_BUFFER; i++)
            commande[i] = '\0';

        tokens = (char**) malloc(MAX_TOKEN * sizeof(char*));
        for(i = 0; i < MAX_TOKEN; i++){
            tokens[i] = (char*) malloc(TAILLE_COMMANDE * sizeof(char));
            for(j = 0; j < TAILLE_COMMANDE; j++)
                tokens[i][j] = '\0';
        }

        if(strcmp(utilisateur, "root"))
            format = TEXTE_VERT_C | FOND_NOIR;
        else
            format = TEXTE_ROUGE_C | FOND_NOIR;
        printf("%s@MLK_$> ", utilisateur);
        format = TEXTE_BLANC | FOND_NOIR;
        
        lire_clavier(commande, MAX_TAILLE_BUFFER, VISIBLE);
        decouper_commande(commande, tokens);
        executer_commande(tokens);
        
        for(i = 0; i < MAX_TOKEN; i++)
            free(tokens[i]);
        free(tokens);
        free(commande);
    }while(!arret);
}

void decouper_commande (char* chaine, char **tokens) {
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

void clear () {
    sti();
    printf("\f");
}

void hello (char *s) {
    sti();
    
    if(!s)
        printf("Hello world !\n");
    else
        printf("Hello %s !\n", s);
}

void beer (void *n) {
    int32_t i;
    
    sti();

    for(i = (int32_t) n; i > 0; i--)
        printf("%d more beer%s to drink...\n", i, (i > 1)?"s":"");
}

void rand (void *n) {
    sti();
    printf("Nombre aleatoire : %d\n", crand48() % (int)n);
}

void fact (void *n) {
    int32_t i;
    int32_t r;
    
    sti();

    r = (int32_t) n;
    
    if(r < 0){
        printf("Nombre positif ou nul attendu !\n");
    }else if(r <= 1){
        printf("%d! = 1\n", r);
    }else{
        for(i = 1; i < (int32_t) n; i++)
            r *= i;
        printf("%d! = %d\n", (int32_t) n, r);
    }
}

void sleep (void *n) {
    sti();
    dors((uint32_t) n);
}

void su () {
    char gui[TAILLE_LOGIN + 15];
    char mdp[TAILLE_LOGIN];
    int32_t i;

    sti();
    
    if(strcmp(utilisateur, "root")){
        printf("Mot de passe : ");
        lire_clavier(mdp, TAILLE_LOGIN, CACHE);

        for(i = 0; i < nb_utilisateurs; i++){
            if(!strncmp(utilisateurs[i][0], "root", TAILLE_LOGIN)
               && !strncmp(utilisateurs[i][1], mdp, TAILLE_LOGIN)){
                strcpy(ancien_utilisateur, utilisateur);
                strncpy(utilisateur, "root\0", 5);
                
                sprintf(gui, "UTILISATEUR : %-21s", utilisateur);
                maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);

                return;
            }
        }

        printf("Erreur d'authentification\n");
    }else{
        printf("Vous etes deja super utilisateur\n");
    }
}

void users () {
    int32_t i, j;

    sti();
    
    if(!strcmp(utilisateur, "root")){
        printf("*** Liste des utilisateurs du systeme :\n");
        printf("  _______________________________________________\n");
        printf(" |                       |                       |\n");
        printf(" |-----------------------+-----------------------|\n");
        
        for(i = 0; i < nb_utilisateurs; i++){
            printf(" |");
            for(j = 0; j < TAILLE_LOGIN + 1 - strlen(utilisateurs[i][0]); j++)
                printf(" ");
            printf("%s |", utilisateurs[i][0]);
            for(j = 0; j < TAILLE_LOGIN + 1 - strlen(utilisateurs[i][1]); j++)
                printf(" ");
            printf("%s |\n", utilisateurs[i][1]);
        }

        printf(" '-----------------------+-----------------------'\n");
    }else{
        printf("Vous devez etre super utilisateur pour voir la liste des utilisateurs\n");
    }
}

void quitter () {
    char gui[TAILLE_LOGIN + 15];
    
    if(!strcmp(utilisateur, "root")){
        if(strcmp(ancien_utilisateur, "root")){
            strcpy(utilisateur, ancien_utilisateur);
            
            sprintf(gui, "UTILISATEUR : %-21s", utilisateur);
            maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);
        }else{
            printf("Bye.\n");
            arret = 1;
        }
    }else{
        printf("Bye.\n");
        arret = 1;
    }
}

void help () {
    sti();

    format = TEXTE_BLEU_C | FOND_NOIR;
    printf("*** Liste des commandes shell :\n");
    printf(" - clear : nettoie l'ecran\n");
    printf(" - hello [<chaine>] : dit bonjour\n");
    printf(" - rand <entier> : calcule un entier aleatoire entre 0 et <entier>\n");
    printf(" - fact <entier> : calcule la factorielle de <entier>\n");
    printf(" - beer <entier> : il reste <entier> biere(s) a boire\n");
    printf(" - sleep <entier> : sieste de <entier> secondes\n");
    printf(" - jobs : affiche les processus en cours d'execution\n");
    printf(" - users : affiche la liste des utilisateurs du systeme\n");
    printf(" - su : passe en mode super utilisateur\n");
    printf(" - help : affiche cette aide\n");
    printf(" - exit : sort du mode super utilisateur ou quitte le shell\n");
}

void jobs () {
    int32_t i, j;
    
    sti();

    printf(" *** Liste des processus (%d) :\n", nombre_processus);
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

void executer_commande (char **tokens) {
    char nom[TAILLE_COMMANDE];
    int32_t pid;
    int32_t param_int;
    
    if(!strcmp(tokens[0], "exit")){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&quitter, "exit\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }
        
    }else if(!strcmp(tokens[0], "clear")) {
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&clear, "clear\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "su")) {
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&su, "su\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }
        
    }else if(!strcmp(tokens[0], "hello")){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&hello, "hello\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            if(!strcmp(tokens[2], "")){
                sprintf(nom, "hello %s", tokens[1]);
                pid = creer_processus(&hello, nom, (void*) tokens[1]);
                if(pid > 0)
                    attendre_terminaison(pid);
            }else{
                printf("Un seul (ou aucun) argument attendu : chaine !\n");
            }
        }
        
    }else if(!strcmp(tokens[0], "jobs")){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&jobs, "jobs\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }
        
    }else if(!strcmp(tokens[0], "help")){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&help, "help\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "users")){
        if(!strcmp(tokens[1], "")){
            pid = creer_processus(&users, "users\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "beer")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "beer %d", param_int);
                pid = creer_processus(&beer, nom, (void*) param_int);
                if(pid > 0)
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }

    }else if(!strcmp(tokens[0], "fact")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "fact %d", param_int);
                pid = creer_processus(&fact, nom, (void*) param_int);
                if(pid > 0)
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }
        
    }else if(!strcmp(tokens[0], "rand")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "rand %d", param_int);
                pid = creer_processus(&rand, nom, (void*) param_int);
                if(pid > 0)
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }

    }else if(!strcmp(tokens[0], "sleep")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "sleep %d", param_int);
                pid = creer_processus(&sleep, nom, (void*) param_int);
                if(pid > 0)
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }
        
    }else if(strcmp(tokens[0], "")){
        printf("Commande inconnue !\n");
    }
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
