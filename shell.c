#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "ecran.h"
#include "clavier.h"
#include "horloge.h"
#include "malloc.c.h"
#include "gui.h"
#include "ordonnancement.h"
#include "commandes.h"
#include "tictactoe.h"
#include "shell.h"

extern void tictactoe();

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern uint32_t nombre_processus;
extern int32_t indice_actif;

extern uint32_t ligne;
extern uint32_t colonne;
extern uint8_t format;

int32_t nb_utilisateurs = 4;
char utilisateurs[][2][TAILLE_LOGIN] = {
    {"root", "toor"},
    {"yann", "Marseille13"},
    {"test", "test"},
    {"invite", ""},
};
char utilisateur[TAILLE_LOGIN];
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

    format = TEXTE_GRIS | FOND_NOIR;
    printf("commande 'help' pour afficher l'aide\n");

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

void executer_commande (char **tokens) {
    char nom[TAILLE_COMMANDE];
    int32_t pid;
    int32_t param_int;
    
    if(!strcmp(tokens[0], "exit")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&quitter, "exit\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "tictactoe")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&tictactoe, "tictactoe\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }
        
    }else if(!strcmp(tokens[0], "clear")) {
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&clear, "clear\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "su")) {
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&su, "su\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "jobs")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&jobs, "jobs\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "infinity")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&infinity, "infinity\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "help")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&help, "help\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "users")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&users, "users\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }
        
    }else if(!strcmp(tokens[0], "hello")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&hello, "hello\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                sprintf(nom, "hello %s", tokens[1]);
                pid = creer_processus(&hello, nom, (void*) tokens[1]);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul (ou aucun) argument attendu : chaine !\n");
            }
        }

    }else if(!strcmp(tokens[0], "beer")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "beer %d", param_int);
                pid = creer_processus(&beer, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }

    }else if(!strcmp(tokens[0], "beer")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "beer %d", param_int);
                pid = creer_processus(&beer, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }
        
    }else if(!strcmp(tokens[0], "rand")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "rand %d", param_int);
                pid = creer_processus(&rand, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }

    }else if(!strcmp(tokens[0], "srand")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "srand %d", param_int);
                pid = creer_processus(&init_rand, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }

    }else if(!strcmp(tokens[0], "sleep")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "sleep %d", param_int);
                pid = creer_processus(&sleep, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }

    }else if(!strcmp(tokens[0], "kill")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "kill %d", param_int);
                pid = creer_processus(&kill, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul parametre attendu : entier !\n");
            }
        }else{
            printf("Un entier attendu en parametre !\n");
        }
        
    }else if(strcmp(tokens[0], "")){
        printf("Commande inconnue '%s' !\n", tokens[0]);
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

void login () {
    char gui[TAILLE_LOGIN + 15];
    char *test_login;
    char *test_mdp;
    int32_t i;
    int8_t ok;

    sti();

    dessine_MLK();
    
    ok = 0;
    
    while(!ok){
        prompt_login();

        test_login = (char*) malloc(TAILLE_LOGIN * sizeof(char));
        for(i = 0; i < TAILLE_LOGIN; i++)
            test_login[i] = '\0';

        test_mdp = (char*) malloc(TAILLE_LOGIN * sizeof(char));
        for(i = 0; i < TAILLE_LOGIN; i++)
            test_mdp[i] = '\0';

        format = TEXTE_ROUGE_C | FOND_BLEU;
        ligne = L_LOGIN + 1;
        colonne = C_LOGIN + 17;
        place_curseur(ligne, colonne);
        lire_clavier(test_login, TAILLE_LOGIN, VISIBLE);

        format = TEXTE_VERT_C | FOND_BLEU;
        ligne = L_LOGIN + 2;
        colonne = C_LOGIN + 17;
        place_curseur(ligne, colonne);
        lire_clavier(test_mdp, TAILLE_LOGIN, CACHE);
        
        for(i = 0; i < nb_utilisateurs; i++){
            if(!strncmp(utilisateurs[i][0], test_login, TAILLE_LOGIN)
               && !strncmp(utilisateurs[i][1], test_mdp, TAILLE_LOGIN)){
                ok = 1;
                
                strncpy(utilisateur, test_login, TAILLE_LOGIN);
                sprintf(gui, "UTILISATEUR : %-21s", test_login);
                maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);
                
                break;
            }
        }
        
        if(!ok)
            identifiants_incorrects();

        free(test_login);
        free(test_mdp);
    }
}
