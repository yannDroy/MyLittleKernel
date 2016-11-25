#include <cpu.h>
#include <inttypes.h>
#include <string.h>
#include <stdio.h>
#include "ecran.h"
#include "horloge.h"
#include "processus.h"
#include "ordonnancement.h"
#include "clavier.h"
#include "aleatoire.h"

void kernel_start () {
    init_affichage();
    init_clock();
    init_clavier();

    srand(100);

    if(!init_table_processus())
        init();
}
