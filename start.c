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

void kernel_start () {
    init_affichage();
    init_clock();
    init_clavier();

    srand(111);
    printf("\f");

    if(!init_table_processus())
        init();
}
