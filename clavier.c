#include <stdio.h>
#include <cpu.h>
#include <inttypes.h>
#include <string.h>
#include "clavier.h"
#include "ordonnancement.h"
#include "interruption.h"
#include "shell.h"
#include "ecran.h"

extern uint32_t ligne;
extern uint32_t colonne;

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern int32_t indice_dernier_attente;

char buffer[MAX_TAILLE_BUFFER];

int32_t indice;
int8_t input = 0;
int8_t rempli = 0;
int8_t visible = 1;

int8_t shift = 0;
int8_t caps = 0;
int8_t alt = 0;
int8_t ctrl = 0;
int8_t inser = 0;

void clavier_PIT () {
    int8_t c;

    cli();
    
    outb(0x20, PORT_OUT_PIT_KB);

    if(inb(PORT_COM_KB) & 0x01) {
        c = inb(PORT_DATA_KB);

        traiter_touche(c);
    }

    sti();
}

void lire_clavier (char* tmp, int32_t taille, int8_t mode) {
    vider_buffer();
    input = 1;
    rempli = 0;
    visible = mode;
    
    while(!rempli && buffer[taille - 2] == '\0');

    input = 0;
    rempli = 0;
    visible = 1;

    strncpy(tmp, buffer, strlen(buffer));
}

void init_clavier () {
    masque_IRQ(CANAL_CLAVIER, 0);

    init_traitant_IT(INTERRUPTION_CLAVIER, &traitant_IT_33);

    vider_buffer();
}

void vider_buffer () {
    int32_t i;

    for(i = 0; i < MAX_TAILLE_BUFFER; i++)
        buffer[i] = '\0';
    
    indice = 0;
}

void mettre_caractere_buffer (char nr, char sh, char ca, char al) {
    char a_mettre;
    char save_1;
    char save_2;
    int32_t save_i;

    a_mettre = 0;

    if(shift && sh)
        a_mettre = sh;
    else if(caps && ca)
        a_mettre = ca;
    else if(alt && al)
        a_mettre = al;
    else if(nr)
        a_mettre = nr;

    if(a_mettre){
        if(!inser){
            save_i = indice;
            save_1 = a_mettre;
            while(save_1 != '\0' && indice < MAX_TAILLE_BUFFER){
                save_2 = buffer[indice];
                buffer[indice] = save_1;
                save_1 = save_2;
                if(visible)
                    printf("%c", buffer[indice]);
                else
                    printf("*");
                indice++;
            }
            buffer[indice] = '\0';
            while(indice > save_i + 1){
                indice--;
                printf("\b");
            }
        }else{
            buffer[indice] = a_mettre;
            if(visible)
                printf("%c", buffer[indice]);
            else
                printf("*");
            indice++;
        }
    }
}

void traiter_touche (int8_t c) {
    char save;
    int32_t save_i;
    
    switch(c){
    case KB_ECHAP :
        break;
    case KB_F1 :
        break;
    case KB_F2 :
        break;
    case KB_F3 :
        break;
    case KB_F4 :
        break;
    case KB_F5 :
        break;
    case KB_F6 :
        break;
    case KB_F7 :
        break;
    case KB_F8 :
        break;
    case KB_F9 :
        break;
    case KB_F10 :
        break;
    case KB_F11 :
        break;
    case KB_F12 :
        break;
    case KB_PRINT :
        break;
    case KB_DEFIL :
        break;
    case KB_PAUSE :
        break;
    case KB_INSER :
        inser = 1 - inser;
        break;
    case KB_SUPPR :
        indice++;
        save_i = indice;
        save = buffer[indice];
        do{
            buffer[indice - 1] = save;
            if(visible){
                printf("%c", save);
            }else{
                if(save == '\0')
                    printf(" ");
                else
                    printf("*");
            }
            save = buffer[indice + 1];
            indice++;
        }while(save != '\0');
        buffer[indice - 1] = '\0';
        printf(" ");
        while(indice > save_i - 1){
            indice--;
            printf("\b");
        }
        break;
    case KB_DEBUT :
        while(indice > 0){
            indice--;
            printf("\b");
        }
        break;
    case KB_FIN :
        while(buffer[indice] != '\0'){
            indice++;
            colonne++;
            if(colonne >= NB_COLONNES){
                colonne = PREMIERE_COLONNE;
                ligne++;
                if(ligne >= NB_LIGNES){
                    ligne = NB_LIGNES - 1;
                    defilement();
                }
            }
            place_curseur(ligne, colonne);
        }
        break;
    case KB_DEBUT_P :
        break;
    case KB_FIN_P :
        break;
    case KB_CARRE :            
        break;
    case KB_TAB :
        break;
    case KB_CAPSLOCK :
        caps = 1 - caps;
        break;
    case KB_LSHIFT :
        shift = 1 - shift;
        break;
    case KB_LSHIFT - 128:
        shift = 1 - shift;
        break;
    case KB_CHEVRON :
        mettre_caractere_buffer('<', '>', '<', '|');
        break;
    case KB_PAR_FER :
        mettre_caractere_buffer(')', ')', ')', ']');
        break;
    case KB_EGAL :
        mettre_caractere_buffer('=', '+', '=', '}');
        break;
    case KB_CHAPEAU :
        mettre_caractere_buffer('^', '^', '^', 0);
        break;
    case KB_DOLLAR :
        mettre_caractere_buffer('$', 0, '$', 0);
        break;
    case KB_POURCENT :
        mettre_caractere_buffer(0, '%', 0, 0);
        break;
    case KB_MULT :
        mettre_caractere_buffer('*', 0, '*', 0);
        break;
    case KB_VIRG :
        mettre_caractere_buffer(',', '?', ',', 0);
        break;
    case KB_PVIRG :
        mettre_caractere_buffer(';', '.', ';', 0);
        break;
    case KB_DPOINTS :
        mettre_caractere_buffer(':', '/', ':', 0);
        break;
    case KB_EXCL :
        mettre_caractere_buffer('!', 0, '!', 0);
        break;
    case KB_BACKSPACE :
        if(indice > 0){
            save_i = indice;
            save = buffer[indice];
            printf("\b");
            do{
                buffer[indice - 1] = save;
                if(visible){
                    printf("%c", save);
                }else{
                    if(save == '\0')
                        printf(" ");
                    else
                        printf("*");
                }
                save = buffer[indice + 1];
                indice++;
            }while(save != '\0');
            buffer[indice - 1] = '\0';
            printf(" ");
            while(indice > save_i - 1){
                indice--;
                printf("\b");
            }
        }
        break;
    case KB_ENTER :
        if(input){
            rempli = 1;
            printf("\n");
        }else{
            printf("\n");
            vider_buffer();
        }
        return;
        break;
    case KB_RSHIFT :
        shift = 1 - shift;
        break;
    case KB_RSHIFT - 128:
        shift = 1 - shift;
        break;
    case KB_1 :
        mettre_caractere_buffer('&', '1', '&', 0);
        break;
    case KB_2 :
        mettre_caractere_buffer(0, '2', 0, '~');
        break;
    case KB_3 :
        mettre_caractere_buffer('\"', '3', '\"', '#');
        break;
    case KB_4 :
        mettre_caractere_buffer('\'', '4', '\'', '{');
        break;
    case KB_5 :
        mettre_caractere_buffer('(', '5', '(', '[');
        break;
    case KB_6 :
        mettre_caractere_buffer('-', '6', '-', '|');
        break;
    case KB_7 :
        mettre_caractere_buffer(0, '7', 0, 0);
        break;
    case KB_8 :
        mettre_caractere_buffer('_', '8', '_', '\\');
        break;
    case KB_9 :
        mettre_caractere_buffer(0, '9', 0, '^');
        break;
    case KB_0 :
        mettre_caractere_buffer(0, '0', 0, '@');
        break;
    case KB_A :
        mettre_caractere_buffer('a', 'A', 'A', 0);
        break;
    case KB_B :
        mettre_caractere_buffer('b', 'B', 'B', 0);
        break;
    case KB_C :
        if(ctrl){
            if(strcmp(table_processus[indice_dernier_attente].nom, "init")
               && strcmp(table_processus[indice_dernier_attente].nom, "shell")
               && strcmp(table_processus[indice_dernier_attente].nom, "login")){
                table_processus[indice_dernier_attente].etat = ATTENTE_TERM;
                if(colonne > PREMIERE_COLONNE)
                    printf("\n");
                ordonnance();
            }
        }else{
            mettre_caractere_buffer('c', 'C', 'C', 0);
        }
        break;
    case KB_D :
        mettre_caractere_buffer('d', 'D', 'D', 0);
        break;
    case KB_E :
        mettre_caractere_buffer('e', 'E', 'E', 0);
        break;
    case KB_F :
        mettre_caractere_buffer('f', 'F', 'F', 0);
        break;
    case KB_G :
        mettre_caractere_buffer('g', 'G', 'G', 0);
        break;
    case KB_H :
        mettre_caractere_buffer('h', 'H', 'H', 0);
        break;
    case KB_I :
        mettre_caractere_buffer('i', 'I', 'I', 0);
        break;
    case KB_J :
        mettre_caractere_buffer('j', 'J', 'J', 0);
        break;
    case KB_K :
        mettre_caractere_buffer('k', 'K', 'K', 0);
        break;
    case KB_L :
        mettre_caractere_buffer('l', 'L', 'L', 0);
        break;
    case KB_M :
        mettre_caractere_buffer('m', 'M', 'M', 0);
        break;
    case KB_N :
        mettre_caractere_buffer('n', 'N', 'N', 0);
        break;
    case KB_O :
        mettre_caractere_buffer('o', 'O', 'O', 0);
        break;
    case KB_P :
        mettre_caractere_buffer('p', 'P', 'P', 0);
        break;
    case KB_Q :
        mettre_caractere_buffer('q', 'Q', 'Q', 0);
        break;
    case KB_R :
        mettre_caractere_buffer('r', 'R', 'R', 0);
        break;
    case KB_S :
        mettre_caractere_buffer('s', 'S', 'S', 0);
        break;
    case KB_T :
        mettre_caractere_buffer('t', 'T', 'T', 0);
        break;
    case KB_U :
        mettre_caractere_buffer('u', 'U', 'U', 0);
        break;
    case KB_V :
        mettre_caractere_buffer('v', 'V', 'V', 0);
        break;
    case KB_W :
        mettre_caractere_buffer('w', 'W', 'W', 0);
        break;
    case KB_X :
        mettre_caractere_buffer('x', 'X', 'X', 0);
        break;
    case KB_Y :
        mettre_caractere_buffer('y', 'Y', 'Y', 0);
        break;
    case KB_Z :
        mettre_caractere_buffer('z', 'Z', 'Z', 0);
        break;
    case KB_LCTRL :
        ctrl = 1 - ctrl;
        break;
    case KB_LCTRL - 128 :
        ctrl = 1 - ctrl;
        break;
    case KB_FN :
        break;
    case KB_WIN :
        break;
    case KB_ALT :
        alt = 1 - alt;
        break;
    case KB_ALT - 128:
        alt = 1 - alt;
        break;
    case KB_ESPACE :
        mettre_caractere_buffer(' ', ' ', ' ', 0);
        break;
    case KB_RCLICK :
        break;
    case KB_UP :
        break;
    case KB_DOWN :
        break;
    case KB_LEFT :
        if(indice > 0){
            indice--;
            printf("\b");
        }
        break;
    case KB_RIGHT :
        if(buffer[indice] != '\0'){
            indice++;
            colonne++;
            if(colonne >= NB_COLONNES){
                colonne = PREMIERE_COLONNE;
                ligne++;
                if(ligne >= NB_LIGNES){
                    ligne = NB_LIGNES - 1;
                    defilement();
                }
            }
            place_curseur(ligne, colonne);
        }
        break;
    default :
        break;
    }

    if(indice == MAX_TAILLE_BUFFER - 1){
        if(input){
            buffer[indice] = '\0';
            rempli = 1;
            printf("\n");
        }else{
            printf("\n");
            vider_buffer();
        }
    }
}
