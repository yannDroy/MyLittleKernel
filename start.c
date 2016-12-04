#include <cpu.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include "ecran.h"
#include "horloge.h"
#include "init.h"
#include "gui.h"
#include "ordonnancement.h"
#include "clavier.h"
#include "aleatoire.h"

extern char utilisateur[TAILLE_LOGIN];

void kernel_start () {
    init_affichage();
    init_clock();
    init_clavier();

    srand(111);
    printf("\f");

    strncpy(utilisateur, "root\0", 5);

    if(!init_table_processus())
        init();
}
