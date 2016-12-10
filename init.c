#include <stdio.h>
#include <cpu.h>
#include <string.h>
#include <stddef.h>
#include "ordonnancement.h"
#include "horloge.h"
#include "gui.h"
#include "clavier.h"
#include "interruption.h"
#include "ecran.h"
#include "shell.h"
#include "init.h"

extern char utilisateur[TAILLE_LOGIN];

int8_t systeme;

void init () {
    int32_t pid;

    sti();

    systeme = MARCHE;
    while(systeme != ARRET){
        efface_ecran(0);

        pid = creer_processus(&login, "login\0", NULL);
        if(pid > 0)
            attendre_terminaison(pid);
        
        if(systeme != ARRET){
            printf("\f");
            init_affichage();
    
            pid = creer_processus(&shell, "shell\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }

        desactiver_barre();
        strcpy(utilisateur, "root\0");
    }

    cli();
    efface_ecran(0);
    
    while(1)
        hlt();
}
