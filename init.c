#include <stdio.h>
#include <cpu.h>
#include <stddef.h>
#include "ordonnancement.h"
#include "init.h"
#include "shell.h"
#include "gui.h"

void init () {
    int32_t pid;

    sti();

    pid = creer_processus(&login, "login\0", NULL);
    if(pid > 0)
        attendre_terminaison(pid);
    
    pid = creer_processus(&shell, "shell\0", NULL);
    if(pid > 0)
        attendre_terminaison(pid);
    
    for(;;){
        sti();
        hlt();
        cli();
    }
}
