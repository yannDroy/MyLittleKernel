#include <inttypes.h>
#include <stdbool.h>
#include <debug.h>
#include <cpu.h>
#include "horloge.h"
#include "ecran.h"
#include "gui.h"
#include "ordonnancement.h"
#include "interruption.h"

extern int8_t barre_existe;

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern uint8_t indice_actif;

uint32_t temps;

uint32_t nbr_secondes () {
    return temps / CLOCKFREQ;
}

void tic_PIT () {
    int32_t sec;
    char s[8];

    cli();
    
    outb(0x20, PORT_OUT_PIT_CLOCK);

    temps++;

    if(temps % CLOCKFREQ == 0){
        sec = nbr_secondes();
        sprintf(s, "%.2d:%.2d:%.2d", (sec / 3600), (sec % 3600) / 60, (sec % 3600) % 60);

        if(barre_existe)
            maj_GUI(s, C_MAJ_CLOCK, TEXTE_ROUGE | FOND_GRIS);
    }

    ordonnance();

    sti();
}

void init_clock () {
    int16_t frequence;

    masque_IRQ(CANAL_CLOCK, 0);

    frequence = QUARTZ / CLOCKFREQ;
    
    outb(0x34, PORT_COM_CLOCK);
    outb(frequence & 0x00FF, PORT_DATA_CLOCK);
    outb(frequence >> 8, PORT_DATA_CLOCK);

    temps = 0;

    init_traitant_IT(INTERRUPTION_CLOCK, &traitant_IT_32);
}

void dors (uint32_t sec) {
    if(indice_actif != 0){
        table_processus[indice_actif].etat = ENDORMI;
        table_processus[indice_actif].heure_reveil = nbr_secondes() + sec;

        ordonnance();
    }
}
