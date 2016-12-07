#include <stdio.h>
#include <inttypes.h>
#include <cpu.h>
#include "aleatoire.h"
#include "atoi.h"
#include "clavier.h"
#include "malloc.h"
#include "tictactoe.h"

extern int8_t sauvegarde_active;

char plateau[3][3];
position positions_libres[9];

int32_t nb_place_libre;
int32_t x, y;

void afficher_matrice () {
    int32_t i, j;

    printf("  _____\n");
    for(i = 0; i < 3; i++){
        printf(" |");
        for(j = 0; j < 3 ; j++)
            printf("%c|", plateau[i][j]);
        printf("\n");
    }
    printf("\n");
}

void chercher_place_libre () {
    int32_t i, j;
    int32_t choix;
    
    nb_place_libre = 0;

    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            if(plateau[i][j] == '_'){
                positions_libres[nb_place_libre].absc = i;
                positions_libres[nb_place_libre].ord = j;
                nb_place_libre++;
            }
        }
    }

    choix = crand48() % (nb_place_libre - 1);
    x = positions_libres[choix].absc;
    y = positions_libres[choix].ord;
}

void jouer_ia () {
    chercher_place_libre();
    plateau[x][y] = 'O';
}

void initialiser_tab () {
    int32_t i, j;

    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++)
            plateau[i][j] = '_';
    }
}

int32_t est_valide (int32_t choixX , int32_t choixY) {
    if((choixX < 0) || (2 < choixX))
        return 0;
    if((choixY < 0) || (2 < choixY))
        return 0;
    if(plateau[choixX][choixY] != '_')
        return 0;

    return 1;
}

int32_t est_finie () {
    int32_t i, j;

    for(i = 0; i < 3; i++){
        if((plateau[i][0] == plateau[i][1]) &&
           (plateau[i][0] == plateau[i][2]) &&
           (plateau[i][0] != '_')){
            if(plateau[i][0] == 'X')
                printf(" Vous avez gagne !\n");
            else
                printf(" Vous avez perdu !\n");
            
            return 1;
        }
    }
    
    for(j = 0; j < 3; j++){
        if((plateau[0][j] == plateau[1][j]) &&
           (plateau[0][j] == plateau[2][j]) &&
           (plateau[0][j] != '_')){
            if(plateau[0][j] == 'X')
                printf(" Vous avez gagne !\n");
            else
                printf(" Vous avez perdu !\n");
            
            return 1;
        }
    }

    if((plateau[0][0] == plateau[1][1]) && 
       (plateau[0][0] == plateau[2][2]) &&
       (plateau[0][0] != '_')){
        if(plateau[0][0] == 'X')
            printf(" Vous avez gagne !\n");
        else
            printf(" Vous avez perdu !\n");
        
        return 1;
    }

    if((plateau[0][2] == plateau[1][1]) && 
       (plateau[0][2] == plateau[2][0]) &&
       (plateau[0][2] != '_')){
        if(plateau[0][2] == 'X')
            printf(" Vous avez gagne !\n");
        else
            printf(" Vous avez perdu !\n");
        
        return 1;
    }

    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            if(plateau[i][j] == '_')
                return 0;
        }
    }

    printf(" Match nul !\n");
    return 1;
}

void tictactoe () {
    char *input;
    int32_t choixX, choixY;

    sti();

    initialiser_tab();

    sauvegarde_active = 1;
    printf("\f");
    sauvegarde_active = 0;
    
    while(1){
        printf("\f\n  *** Tic Tac Toe ***\n\n");
        printf(" Vous jouez les 'X'\n\n");
        afficher_matrice();
    
        choixX = -1;
        choixY = -1;
        
        while(!est_valide(choixX,choixY)){
            printf("\n Entrez une position :\n  X : ");
            input = lire_clavier(3, VISIBLE);
            choixX = atoi(input);
            free(input);
            
            printf("  Y : ");
            input = lire_clavier(3, VISIBLE);
            printf("\n");
            choixY = atoi(input);
            free(input);
        }

        plateau[choixX][choixY] = 'X';

        if(est_finie()){
            afficher_matrice();
            break;
        }
        
        jouer_ia();

        if(est_finie()){
            afficher_matrice();
            break;
        }

        printf("\f");
    }
}
