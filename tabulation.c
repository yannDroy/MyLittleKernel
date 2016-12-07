#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "clavier.h"
#include "tabulation.h"

extern char buffer[MAX_TAILLE_BUFFER];
extern int32_t indice;

int32_t tabulation_active = 0;

int32_t nb_commandes = 21;
char commandes[21][TAILLE_COMMANDE] = {
    "clear", "su", "users", "jobs", "sleep", "time",
    "set_veille", "veille", "shell", "exit", "help", "srand",
    "rand", "fact", "tictactoe", "devine", "rubiks", "hello",
    "beer", "test", "infinity"
};

void tabuler () {
    char test[TAILLE_COMMANDE];
    int32_t i;
    int32_t dernier;
    int32_t nb;

    if(tabulation_active && strcmp(buffer, "")){
        nb = 0;

        for(i = 0; i < nb_commandes; i++){
            if(strlen(buffer) > 1){
                if(strlen(commandes[i]) > strlen(buffer)){
                    strncpy(test, commandes[i], strlen(buffer));
                }else{
                    strcpy(test, commandes[i]);
                }
            }else{
                sprintf(test, "%c", commandes[i][0]);
            }
            
            if(!strcmp(test, buffer)){
                nb++;
                dernier = i;
            }
        }

        if(nb == 1){
            printf("\r");
            prompt_shell();
            
            strcpy(buffer, commandes[dernier]);
            indice = strlen(commandes[dernier]);
            buffer[indice] = ' ';
            indice++;
            
            printf("%s", buffer);
        }
    }
}
