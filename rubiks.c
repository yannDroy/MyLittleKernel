#include <stdio.h>
#include <inttypes.h>
#include <cpu.h>
#include "clavier.h"
#include "ecran.h"
#include "horloge.h"
#include "malloc.h"
#include "aleatoire.h"
#include "atoi.h"
#include "rubiks.h"

extern int8_t format;

char cube[6][9];

int32_t white = 0;
int32_t yellow = 1;
int32_t green = 2;
int32_t blue = 3;
int32_t red = 4;
int32_t orange = 5;

void init_cube () {
    int32_t i, j;

    for(i = 0; i < 6; i++){
        for(j = 0; j < 9; j++){
            switch(i){
            case 0:
                cube[i][j] = 'W';
                break;
            case 1:
                cube[i][j] = 'Y';
                break;
            case 2:
                cube[i][j] = 'G';
                break;
            case 3:
                cube[i][j] = 'B';
                break;
            case 4:
                cube[i][j] = 'R';
                break;
            case 5:
                cube[i][j] = 'O';
                break;
            }
        }
    }
}

int32_t est_resolu () {
    int32_t i, j;

    for(i = 0; i < 6; i++){
        for(j = 0; j < 9; j++){
            switch(i){
            case 0:
                if(cube[i][j] != 'W')
                    return 0;
                break;
            case 1:
                if(cube[i][j] != 'Y')
                    return 0;
                break;
            case 2:
                if(cube[i][j] != 'G')
                    return 0;
                break;
            case 3:
                if(cube[i][j] != 'B')
                    return 0;
                break;
            case 4:
                if(cube[i][j] != 'R')
                    return 0;
                break;
            case 5:
                if(cube[i][j] != 'O')
                    return 0;
                break;
            }
        }
    }

    return 1;
}

void turn_R_face (int32_t n) {
    int32_t i;
    char tmp;

    for(i = 0; i < n; i++){
        tmp = cube[white][2]; cube[white][2] = cube[green][2]; cube[green][2] = cube[yellow][2]; cube[yellow][2] = cube[blue][2]; cube[blue][2] = tmp;
        tmp = cube[white][5]; cube[white][5] = cube[green][5]; cube[green][5] = cube[yellow][5]; cube[yellow][5] = cube[blue][5]; cube[blue][5] = tmp;
        tmp = cube[white][8]; cube[white][8] = cube[green][8]; cube[green][8] = cube[yellow][8]; cube[yellow][8] = cube[blue][8]; cube[blue][8] = tmp;

        tmp = cube[red][0]; cube[red][0] = cube[red][6]; cube[red][6] = cube[red][8]; cube[red][8] = cube[red][2]; cube[red][2] = tmp;
        tmp = cube[red][1]; cube[red][1] = cube[red][3]; cube[red][3] = cube[red][7]; cube[red][7] = cube[red][5]; cube[red][5] = tmp;
    }
}

void turn_L_face (int32_t n){
    int32_t i;
    char tmp;

    for(i = 0; i < n; i++){
        tmp = cube[white][0]; cube[white][0] = cube[blue][0]; cube[blue][0] = cube[yellow][0]; cube[yellow][0] = cube[green][0]; cube[green][0] = tmp;
        tmp = cube[white][3]; cube[white][3] = cube[blue][3]; cube[blue][3] = cube[yellow][3]; cube[yellow][3] = cube[green][3]; cube[green][3] = tmp;
        tmp = cube[white][6]; cube[white][6] = cube[blue][6]; cube[blue][6] = cube[yellow][6]; cube[yellow][6] = cube[green][6]; cube[green][6] = tmp;

        tmp = cube[orange][0]; cube[orange][0] = cube[orange][6]; cube[orange][6] = cube[orange][8]; cube[orange][8] = cube[orange][2]; cube[orange][2] = tmp;
        tmp = cube[orange][1]; cube[orange][1] = cube[orange][3]; cube[orange][3] = cube[orange][7]; cube[orange][7] = cube[orange][5]; cube[orange][5] = tmp;
    }
}

void turn_U_face (int32_t n){
    int32_t i;
    char tmp;

    for(i = 0; i < n; i++){
        tmp = cube[green][0]; cube[green][0] = cube[red][0]; cube[red][0] = cube[blue][8]; cube[blue][8] = cube[orange][0]; cube[orange][0] = tmp;
        tmp = cube[green][2]; cube[green][2] = cube[red][2]; cube[red][2] = cube[blue][6]; cube[blue][6] = cube[orange][2]; cube[orange][2] = tmp;
        tmp = cube[green][1]; cube[green][1] = cube[red][1]; cube[red][1] = cube[blue][7]; cube[blue][7] = cube[orange][1]; cube[orange][1] = tmp;

        tmp = cube[white][0]; cube[white][0] = cube[white][6]; cube[white][6] = cube[white][8]; cube[white][8] = cube[white][2]; cube[white][2] = tmp;
        tmp = cube[white][1]; cube[white][1] = cube[white][3]; cube[white][3] = cube[white][7]; cube[white][7] = cube[white][5]; cube[white][5] = tmp;
    }
}

void turn_D_face (int n){
    int i;
    char tmp;

    for(i = 0; i < n; i++){
        tmp = cube[green][6]; cube[green][6] = cube[orange][6]; cube[orange][6] = cube[blue][2]; cube[blue][2] = cube[red][6]; cube[red][6] = tmp;
        tmp = cube[green][8]; cube[green][8] = cube[orange][8]; cube[orange][8] = cube[blue][0]; cube[blue][0] = cube[red][8]; cube[red][8] = tmp;
        tmp = cube[green][7]; cube[green][7] = cube[orange][7]; cube[orange][7] = cube[blue][1]; cube[blue][1] = cube[red][7]; cube[red][7] = tmp;

        tmp = cube[yellow][0]; cube[yellow][0] = cube[yellow][6]; cube[yellow][6] = cube[yellow][8]; cube[yellow][8] = cube[yellow][2]; cube[yellow][2] = tmp;
        tmp = cube[yellow][1]; cube[yellow][1] = cube[yellow][3]; cube[yellow][3] = cube[yellow][7]; cube[yellow][7] = cube[yellow][5]; cube[yellow][5] = tmp;
    }
}

void turn_F_face (int32_t n){
    int32_t i;
    char tmp;

    for(i = 0; i < n; i++){
        tmp = cube[white][6]; cube[white][6] = cube[orange][8]; cube[orange][8] = cube[yellow][2]; cube[yellow][2] = cube[red][0]; cube[red][0] = tmp;
        tmp = cube[white][8]; cube[white][8] = cube[orange][2]; cube[orange][2] = cube[yellow][0]; cube[yellow][0] = cube[red][6]; cube[red][6] = tmp;
        tmp = cube[white][7]; cube[white][7] = cube[orange][5]; cube[orange][5] = cube[yellow][1]; cube[yellow][1] = cube[red][3]; cube[red][3] = tmp;

        tmp = cube[green][0]; cube[green][0] = cube[green][6]; cube[green][6] = cube[green][8]; cube[green][8] = cube[green][2]; cube[green][2] = tmp;
        tmp = cube[green][1]; cube[green][1] = cube[green][3]; cube[green][3] = cube[green][7]; cube[green][7] = cube[green][5]; cube[green][5] = tmp;
    }
}

void turn_B_face (int32_t n){
    int32_t i;
    char tmp;

    for(i = 0; i < n; i++){
        tmp = cube[white][0]; cube[white][0] = cube[red][2]; cube[red][2] = cube[yellow][8]; cube[yellow][8] = cube[orange][6]; cube[orange][6] = tmp;
        tmp = cube[white][2]; cube[white][2] = cube[red][8]; cube[red][8] = cube[yellow][6]; cube[yellow][6] = cube[orange][0]; cube[orange][0] = tmp;
        tmp = cube[white][1]; cube[white][1] = cube[red][5]; cube[red][5] = cube[yellow][7]; cube[yellow][7] = cube[orange][3]; cube[orange][3] = tmp;

        tmp = cube[blue][0]; cube[blue][0] = cube[blue][6]; cube[blue][6] = cube[blue][8]; cube[blue][8] = cube[blue][2]; cube[blue][2] = tmp;
        tmp = cube[blue][1]; cube[blue][1] = cube[blue][3]; cube[blue][3] = cube[blue][7]; cube[blue][7] = cube[blue][5]; cube[blue][5] = tmp;
    }
}

void afficher_case (char c) {
    switch(c){
    case 'W':
        format = TEXTE_GRIS | FOND_NOIR;
        break;
    case 'Y':
        format = TEXTE_JAUNE | FOND_NOIR;
        break;
    case 'G':
        format = TEXTE_VERT_C | FOND_NOIR;
        break;
    case 'B':
        format = TEXTE_BLEU_C | FOND_NOIR;
        break;
    case 'R':
        format = TEXTE_ROUGE_C | FOND_NOIR;
        break;
    case 'O':
        format = TEXTE_MARRON | FOND_NOIR;
        break;
    }
    printf("%c ", c);
}

void afficher_cube () {
    printf("          ");
    afficher_case(cube[blue][0]);
    afficher_case(cube[blue][1]);
    afficher_case(cube[blue][2]);
    printf("\n");
    printf("          ");
    afficher_case(cube[blue][3]);
    afficher_case(cube[blue][4]);
    afficher_case(cube[blue][5]);
    printf("\n");
    printf("          ");
    afficher_case(cube[blue][6]);
    afficher_case(cube[blue][7]);
    afficher_case(cube[blue][8]);
    printf("\n\n");

    printf("  ");
    afficher_case(cube[orange][6]);
    afficher_case(cube[orange][3]);
    afficher_case(cube[orange][0]);
    printf("  ");
    afficher_case(cube[white][0]);
    afficher_case(cube[white][1]);
    afficher_case(cube[white][2]);
    printf("  ");
    afficher_case(cube[red][2]);
    afficher_case(cube[red][5]);
    afficher_case(cube[red][8]);
    printf("  ");
    afficher_case(cube[yellow][8]);
    afficher_case(cube[yellow][7]);
    afficher_case(cube[yellow][6]);
    printf("\n");

    printf("  ");
    afficher_case(cube[orange][7]);
    afficher_case(cube[orange][4]);
    afficher_case(cube[orange][1]);
    printf("  ");
    afficher_case(cube[white][3]);
    afficher_case(cube[white][4]);
    afficher_case(cube[white][5]);
    printf("  ");
    afficher_case(cube[red][1]);
    afficher_case(cube[red][4]);
    afficher_case(cube[red][7]);
    printf("  ");
    afficher_case(cube[yellow][5]);
    afficher_case(cube[yellow][4]);
    afficher_case(cube[yellow][3]);
    printf("\n");

    printf("  ");
    afficher_case(cube[orange][8]);
    afficher_case(cube[orange][5]);
    afficher_case(cube[orange][2]);
    printf("  ");
    afficher_case(cube[white][6]);
    afficher_case(cube[white][7]);
    afficher_case(cube[white][8]);
    printf("  ");
    afficher_case(cube[red][0]);
    afficher_case(cube[red][3]);
    afficher_case(cube[red][6]);
    printf("  ");
    afficher_case(cube[yellow][2]);
    afficher_case(cube[yellow][1]);
    afficher_case(cube[yellow][0]);
    printf("\n\n");

    printf("          ");
    afficher_case(cube[green][0]);
    afficher_case(cube[green][1]);
    afficher_case(cube[green][2]);
    printf("\n");
    printf("          ");
    afficher_case(cube[green][3]);
    afficher_case(cube[green][4]);
    afficher_case(cube[green][5]);
    printf("\n");
    printf("          ");
    afficher_case(cube[green][6]);
    afficher_case(cube[green][7]);
    afficher_case(cube[green][8]);
    printf("\n\n");

    format = TEXTE_BLANC | FOND_NOIR;
}

void melanger (int32_t n){
    int32_t i;
    int32_t face;
    int32_t previous;
    int32_t nb;

    printf("\n\n Melange : ");

    previous = -1;

    for(i = 0; i < n; i++){
        do
            face = crand48() % 5;
        while(face == previous);

        nb = (crand48() % 3) + 1;

        switch(face){
        case 0:
            printf("R");
            turn_R_face(nb);
            break;
        case 1:
            printf("L");
            turn_L_face(nb);
            break;
        case 2:
            printf("U");
            turn_U_face(nb);
            break;
        case 3:
            printf("D");
            turn_D_face(nb);
            break;
        case 4:
            printf("F");
            turn_F_face(nb);
            break;
        case 5:
            printf("B");
            turn_B_face(nb);
            break;
        }

        switch(nb){
        case 1:
            printf("  ");
            break;
        case 2:
            printf("2  ");
            break;
        case 3:
            printf("-  ");
            break;
        }

        previous = face;
    }

    printf("\n");
}

void rubiks () {
    char *input;
    int32_t n_scramble;
    int32_t nb;
    int32_t coups;
    int32_t temps;
    int32_t jeu;

    sti();

    init_cube();

    printf("\f\n  *** Rubik's Cube ***\n\n");
    printf(" Nombre de mouvements pour le melange ? ");

    input = (char*) malloc(4 * sizeof(char));
    for(nb = 0; nb < 4; nb++)
        input[nb] = '\0';

    input = lire_clavier(4, VISIBLE);
    n_scramble = atoi(input);

    coups = 0;
    
    melanger(n_scramble);
    temps = nbr_secondes();

    jeu = 1;
    while(jeu){
        printf("\f\n  *** Rubik's Cube ***\n\n");
        
        afficher_cube();
        free(input);

        if(est_resolu()){
            printf(" Bravo, cube resolu en %d coup(s) et %d seconde(s) !\n\n", coups, nbr_secondes() - temps);
            break;
        }else{
            printf(" Mouvement ? ");
            input = lire_clavier(4, VISIBLE);

            switch(input[1]){
            case '-':
                nb = 3;
                break;
            case '2':
                nb = 2;
                break;
            case '\'':
                nb = 3;
                break;
            default:
                nb = 1;
            }

            switch(input[0]){
            case 'R':
                turn_R_face(nb);
                coups++;
                break;
            case 'L':
                turn_L_face(nb);
                coups++;
                break;
            case 'U':
                turn_U_face(nb);
                coups++;
                break;
            case 'D':
                turn_D_face(nb);
                coups++;
                break;
            case 'F':
                turn_F_face(nb);
                coups++;
                break;
            case 'B':
                turn_B_face(nb);
                coups++;
                break;
            case 'q':
                printf("\n");
                jeu = 0;
                break;
            }
        }
    }
}
