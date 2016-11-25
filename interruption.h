#ifndef __INTERRUPTION_H__
#define __INTERRUPTION_H__

#define MEM_IDT 0x1000

#define PORT_OUT_PIT 0x20

#define PORT_DATA_IRQ 0x21

#include <inttypes.h>
#include <stdbool.h>

/* Fonction de masquage d'interruption */
void masque_IRQ (uint32_t num_IRQ, bool masque);

/* Modifie la table des traitements d'interruptions */
void init_traitant_IT (int32_t num_IT, void (*traitant)(void));

#endif
