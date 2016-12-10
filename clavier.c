#include <stdio.h>
#include <cpu.h>
#include <inttypes.h>
#include <string.h>
#include "clavier.h"
#include "malloc.h"
#include "init.h"
#include "gui.h"
#include "tabulation.h"
#include "historique.h"
#include "scroll.h"
#include "ordonnancement.h"
#include "interruption.h"
#include "shell.h"
#include "ecran.h"

extern int8_t systeme;

extern uint32_t ligne;
extern uint32_t colonne;

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern int32_t indice_dernier_attente;

extern int8_t veille;
extern int32_t temps_non_actif;
extern int32_t temps_veille_sec;

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

    temps_non_actif = 0;
    
    sti();
}

char *lire_clavier (int32_t taille, int8_t mode) {
    char *tmp;
    int32_t pid;
    int32_t i;
    
    vider_buffer();
    input = 1;
    rempli = 0;
    visible = mode;

    tmp = (char*) malloc(taille * sizeof(char));
    for(i = 0; i < taille; i++)
        tmp[i] = '\0';
    
    while(!rempli && buffer[taille - 2] == '\0'){
        if(temps_veille_sec >= 0 && temps_non_actif >= temps_veille_sec){
            pid = creer_processus(&ecran_veille, "veille\0", NULL);
            if(pid > 0)
                attendre_terminaison(pid);
        }
    }

    input = 0;
    rempli = 0;
    visible = 1;

    strncpy(tmp, buffer, strlen(buffer));

    return tmp;
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

    tout_scroller_bas();

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

    
    if(veille && c > 0){
        veille = 0;
        return;
    }
    
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
        tout_scroller_bas();
        
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
        tout_scroller_bas();
        
        while(indice > 0){
            indice--;
            printf("\b");
        }
        break;
    case KB_FIN :
        tout_scroller_bas();
        
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
        if(!shift)
            scroll_haut();
        else
            tout_scroller_haut();
        break;
    case KB_FIN_P :
        if(!shift)
            scroll_bas();
        else
            tout_scroller_bas();
        break;
    case KB_CARRE :            
        break;
    case KB_TAB :
        tabuler();
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
        tout_scroller_bas();
        
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
        tout_scroller_bas();
        
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
            if(indice_dernier_attente > 0){
                if(strcmp(table_processus[indice_dernier_attente].nom, "init")){
                    if(!strcmp(table_processus[indice_dernier_attente].nom, "login"))
                        systeme = ARRET;
                    
                    table_processus[indice_dernier_attente].etat = ATTENTE_TERM;
                    if(colonne > PREMIERE_COLONNE)
                        printf("\n");
                    ordonnance();
                }
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
        tout_scroller_bas();
        charger_dans_buffer(-1);
        break;
    case KB_DOWN :
        tout_scroller_bas();
        charger_dans_buffer(1);
        break;
    case KB_LEFT :
        tout_scroller_bas();
        
        if(indice > 0){
            indice--;
            printf("\b");
        }
        break;
    case KB_RIGHT :
        tout_scroller_bas();
        
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
