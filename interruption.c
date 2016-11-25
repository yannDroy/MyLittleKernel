#include <cpu.h>
#include <segment.h>
#include "interruption.h"

void masque_IRQ (uint32_t num_IRQ, bool masque) {
    uint8_t m;

    m = inb(PORT_DATA_IRQ);

    if(masque)
        m |= 1 << num_IRQ;
    else
        m &= ~(1 << num_IRQ);

    outb(m, PORT_DATA_IRQ);
}

void init_traitant_IT (int32_t num_IT, void (*traitant)(void)) {
    int32_t mot1, mot2;
    int32_t *mem;

    mot1 = (KERNEL_CS << 16) | ((int32_t)traitant & 0x0000FFFF);
    mot2 = ((uint32_t)traitant & 0xFFFF0000) | 0x8E00;

    mem = ((int32_t*) MEM_IDT) + (2 * num_IT);

    *mem = mot1;
    *(mem + 1) = mot2;
}
