#include <stdio.h>
#include <cpu.h>
#include <string.h>
#include "horloge.h"
#include "aleatoire.h"
#include "ordonnancement.h"
#include "processus.h"
#include "ecran.h"
#include "shell.h"
#include "clavier.h"

extern uint8_t format;

char utilisateur[TAILLE_LOGIN];

void login () {
    char gui[TAILLE_LOGIN + 15];
    char test_login[TAILLE_LOGIN];
    char test_mdp[TAILLE_LOGIN];
    char utilisateurs[][2][TAILLE_LOGIN] = {
        {"yann", "yann"},
        {"root", "toor"},
        {"invite", ""}
    };
    int32_t i;
    int8_t ok;

    sti();

    ok = 0;
    
    // printf("\f\n\n\n\n\n\n\n\n\n\n");
    printf("\n");
    
    while(!ok){
        format = FORMAT_VERT;
        printf("                              login : ");
        format = FORMAT_BLANC;
        lire_clavier(test_login, TAILLE_LOGIN, VISIBLE);

        format = FORMAT_VERT;
        printf("                       mot de passe : ");
        format = FORMAT_BLANC;
        lire_clavier(test_mdp, TAILLE_LOGIN, CACHE);
        
        for(i = 0; i < 3; i++){
            if(!strncmp(utilisateurs[i][0], test_login, TAILLE_LOGIN) && !strncmp(utilisateurs[i][1], test_mdp, TAILLE_LOGIN)){
                ok = 1;
                break;
            }
        }
        
        if(!ok){
            format = FORMAT_ROUGE;
            printf("\f\n\n\n\n\n\n\n\n                       Identifiants incorrects !\n\n");
        }
    }

    strncpy(utilisateur, test_login, TAILLE_LOGIN);
    
    sprintf(gui, "UTILISATEUR : %s", test_login);
    maj_GUI(gui, C_MAJ_USER, FORMAT_MARRON_FOND);
    format = FORMAT_BLANC;
}

void init () {
    int32_t pid;

    sti();

    /*
    pid = creer_processus(&login, "login");
    if(pid > 0)
    attendre_terminaison(pid);*/

    pid = creer_processus(&shell, "shell");
    jobs();
    if(pid > 0)
        attendre_terminaison(pid);
    
    for(;;){
        sti();
        
        printf("!");
        
        hlt();
        cli();
    }
}
