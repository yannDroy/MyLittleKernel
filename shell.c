#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "ecran.h"
#include "init.h"
#include "clavier.h"
#include "horloge.h"
#include "malloc.h"
#include "gui.h"
#include "historique.h"
#include "ordonnancement.h"
#include "commandes.h"
#include "atoi.h"
#include "tictactoe.h"
#include "rubiks.h"
#include "shell.h"

extern void tictactoe ();
extern void rubiks ();
extern void ecran_veille ();
extern void verouiller ();
extern void calc ();

extern int8_t continuer;

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern uint32_t nombre_processus;
extern int32_t indice_actif;

extern uint8_t format;

extern int8_t historique_actif;
extern int8_t tabulation_active;

int32_t nb_utilisateurs = 4;
char utilisateurs[][2][TAILLE_LOGIN] = {
    {"root", "toor"},
    {"yann", "Marseille13"},
    {"test", "test"},
    {"invite", ""},
};
char utilisateur[TAILLE_LOGIN];
char ancien_utilisateur[TAILLE_LOGIN];

void prompt_shell () {
    if(strcmp(utilisateur, "root"))
        format = TEXTE_VERT_C | FOND_NOIR;
    else
        format = TEXTE_ROUGE_C | FOND_NOIR;
    printf("%s@MLK_$> ", utilisateur);
        
    format = TEXTE_BLANC | FOND_NOIR;
}

void shell () {
    char *commande;
    char **tokens;
    int32_t i, j;
    int32_t arret;
    
    sti();

    strcpy(ancien_utilisateur, utilisateur);

    format = TEXTE_JAUNE | FOND_NOIR;
    printf("\n  *** MyLittleKernel 0.1 ***\n\n");

    format = TEXTE_GRIS | FOND_NOIR;
    printf("commande 'help' pour afficher l'aide\n");

    arret = MARCHE;
    while(arret == MARCHE){
        tokens = (char**) malloc(MAX_TOKEN * sizeof(char*));
        for(i = 0; i < MAX_TOKEN; i++){
            tokens[i] = (char*) malloc(TAILLE_COMMANDE * sizeof(char));
            for(j = 0; j < TAILLE_COMMANDE; j++)
                tokens[i][j] = '\0';
        }

        prompt_shell();

        historique_actif = 1;
        tabulation_active = 1;
        commande = lire_clavier(MAX_TAILLE_BUFFER, VISIBLE);
        if(strcmp(commande, ""))
            ajouter_historique(commande);
        historique_actif = 0;
        tabulation_active = 0;
        
        decouper_commande(commande, tokens);
        executer_commande(tokens, &arret);
        
        for(i = 0; i < MAX_TOKEN; i++)
            free(tokens[i]);
        free(tokens);
        free(commande);
    }

    continuer = arret;
}

void decouper_commande (char* chaine, char **tokens) {
    char *token;
    int32_t i;

    i = 0;
    while((token = strtok(chaine, " ")) != NULL){
        if(strcmp(token, "")){
            strncpy(tokens[i], token, TAILLE_COMMANDE);
            i++;

            if(i == MAX_TOKEN)
                break;
        }
        chaine = NULL;
    }

    if(i < MAX_TOKEN)
        strcpy(tokens[i], "");
}

void executer_commande (char **tokens, int32_t *arret) {
    char nom[TAILLE_COMMANDE];
    int32_t pid;
    int32_t param_int;
    
    if(!strcmp(tokens[0], "exit")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&quitter, "exit\0", (void*) arret);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "logout")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&deconnexion, "logout\0", (void*) arret);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "tictactoe")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&tictactoe, "tictactoe\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "veille")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&ecran_veille, "veille\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "lock")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&verouiller, "lock\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "rubiks")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&rubiks, "rubiks\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
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

    }else if(!strcmp(tokens[0], "shell")) {
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&shell, "shell\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "time")) {
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&time, "time\0", NULL);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            printf("Pas d'argument attendu !\n");
        }

    }else if(!strcmp(tokens[0], "calc")) {
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&calc, "calc\0", NULL);
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

    }else if(!strcmp(tokens[0], "devine")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&devine, "devine\0", (void*) 100);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                sprintf(nom, "devine %s", tokens[1]);
                param_int = atoi(tokens[1]);
                pid = creer_processus(&devine, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul (ou aucun) argument attendu : entier !\n");
            }
        }

    }else if(!strcmp(tokens[0], "test")){
        if(!strcmp(tokens[1], "") || !strcmp(tokens[1], "&")){
            pid = creer_processus(&test, "test\0", (void*) 0);
            if(pid > 0 && strcmp(tokens[1], "&"))
                attendre_terminaison(pid);
        }else{
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                sprintf(nom, "test %s", tokens[1]);
                param_int = atoi(tokens[1]);
                pid = creer_processus(&test, nom, (void*) param_int);
                if(pid > 0 && strcmp(tokens[2], "&"))
                    attendre_terminaison(pid);
            }else{
                printf("Un seul (ou aucun) argument attendu : entier !\n");
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

    }else if(!strcmp(tokens[0], "pascal")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "pascal %d", param_int);
                pid = creer_processus(&pascal, nom, (void*) param_int);
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

    }else if(!strcmp(tokens[0], "set_veille")){
        if(strcmp(tokens[1], "")){
            if(!strcmp(tokens[2], "") || !strcmp(tokens[2], "&")){
                param_int = atoi(tokens[1]);
                sprintf(nom, "set_veille %d", param_int);
                pid = creer_processus(&set_veille, nom, (void*) param_int);
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
