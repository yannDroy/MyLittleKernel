#include <stdio.h>
#include <cpu.h>
#include <stddef.h>
#include "ordonnancement.h"
#include "horloge.h"
#include "gui.h"
#include "clavier.h"
#include "interruption.h"
#include "ecran.h"
#include "shell.h"
#include "init.h"

void init () {
    int32_t pid;

    sti();

    efface_ecran(0);

    pid = creer_processus(&login, "login\0", NULL);
    if(pid > 0)
        attendre_terminaison(pid);

    printf("\f");
    init_affichage();
    
    pid = creer_processus(&shell, "shell\0", NULL);
    if(pid > 0)
        attendre_terminaison(pid);

    detruire_barre();
    efface_ecran(0);
    masque_IRQ(CANAL_CLOCK, 1);
    masque_IRQ(CANAL_CLAVIER, 1);
    
    for(;;){
        sti();
        hlt();
        cli();
    }
}
