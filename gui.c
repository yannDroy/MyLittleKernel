#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "ecran.h"
#include "clavier.h"
#include "ordonnancement.h"
#include "malloc.h"
#include "horloge.h"
#include "gui.h"

extern int32_t nb_utilisateurs;
extern char utilisateurs[][2][TAILLE_LOGIN];
extern char utilisateur[TAILLE_LOGIN];

extern uint32_t ligne;
extern uint32_t colonne;
extern uint8_t format;

int8_t barre_existe = 0;

void detruire_barre () {
    barre_existe = 0;
}

void maj_GUI (char *chaine, uint32_t col, uint8_t f) {
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;

    save_l = ligne;
    save_c = colonne;
    save_format = format;

    format = f;
   
    ligne = 0;
    colonne = col;
    place_curseur(ligne, colonne);
    
    console_putbytes(chaine, strlen(chaine));

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}

void creer_barre () {
    int32_t i;
    uint8_t save_format;

    barre_existe = 1;

    save_format = format;
    format = TEXTE_NOIR | FOND_GRIS;
    
    for(i = 0; i < NB_COLONNES; i++)
        ecrit_car(0, i, ' ');

    format = save_format;
}

void init_affichage () {
    char gui[TAILLE_LOGIN + 15];
    
    ligne = PREMIERE_LIGNE;
    colonne = PREMIERE_COLONNE;

    creer_barre();
    
    maj_GUI("MLK 0.1", C_MAJ_NOM, TEXTE_MAGENTA | FOND_GRIS);
    maj_GUI("|", C_PIPE1, TEXTE_NOIR | FOND_GRIS);

    maj_GUI("PROCESSUS :   0", C_MAJ_PROC, TEXTE_BLEU | FOND_GRIS);
    maj_GUI("|", C_PIPE2, TEXTE_NOIR | FOND_GRIS);

    sprintf(gui, "UTILISATEUR : %-21s", utilisateur);
    maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);
    //maj_GUI("UTILISATEUR : ---", C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);
    
    maj_GUI("00:00:00", C_MAJ_CLOCK, TEXTE_ROUGE | FOND_GRIS);
    maj_GUI("|", C_PIPE3, TEXTE_NOIR | FOND_GRIS);
}

void prompt_login () {
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;

    save_l = ligne;
    save_c = colonne;
    save_format = format;
    
    format = TEXTE_JAUNE | FOND_BLEU;

    ligne = L_LOGIN;
    colonne = C_LOGIN;
    place_curseur(ligne, colonne);
    printf("                                      ");

    ligne = L_LOGIN + 1;
    colonne = C_LOGIN;
    place_curseur(ligne, colonne);
    printf("         login :                      ");

    ligne = L_LOGIN + 2;
    colonne = C_LOGIN;
    place_curseur(ligne, colonne);
    printf("  mot de passe :                      ");

    ligne = L_LOGIN + 3;
    colonne = C_LOGIN;
    place_curseur(ligne, colonne);
    printf("                                      ");

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}

void dessine_MLK () {
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;

    save_l = ligne;
    save_c = colonne;
    save_format = format;

    ligne = L_NOM;
    colonne = C_NOM;
    format = TEXTE_MAGENTA | FOND_NOIR;
    place_curseur(ligne, colonne);
    printf("MyLittleKernel 0.1");

    ligne = PREMIERE_LIGNE;
    colonne = PREMIERE_COLONNE;

    place_curseur(ligne, colonne);
    
    format = TEXTE_CYAN | FOND_NOIR;
    printf("\n\n");
    printf("                       :::   :::   :::        :::    :::\n");
    printf("                     :+:+: :+:+:  :+:        :+:   :+:\n");
    printf("                   +:+ +:+:+ +:+ +:+        +:+  +:+\n");
    printf("                  +#+  +:+  +#+ +#+        +#++:++\n");
    printf("                 +#+       +#+ +#+        +#+  +#+\n");
    printf("                #+#       #+# #+#        #+#   #+#\n");
    printf("               ###       ### ########## ###    ###\n\n");

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}

void identifiants_incorrects () {
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;

    save_l = ligne;
    save_c = colonne;
    save_format = format;
    
    ligne = L_IDENT_INCORR;
    colonne = C_IDENT_INCORR;

    place_curseur(ligne, colonne);

    format = TEXTE_VERT | FOND_ROUGE;
    printf("       Identifiants incorrects        ");

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}

void login () {
    char gui[TAILLE_LOGIN + 15];
    char *test_login;
    char *test_mdp;
    int32_t i;
    int8_t ok;

    sti();

    dessine_MLK();
    
    ok = 0;
    
    while(!ok){
        prompt_login();

        test_login = (char*) malloc(TAILLE_LOGIN * sizeof(char));
        for(i = 0; i < TAILLE_LOGIN; i++)
            test_login[i] = '\0';

        test_mdp = (char*) malloc(TAILLE_LOGIN * sizeof(char));
        for(i = 0; i < TAILLE_LOGIN; i++)
            test_mdp[i] = '\0';

        format = TEXTE_ROUGE_C | FOND_BLEU;
        ligne = L_LOGIN + 1;
        colonne = C_LOGIN + 17;
        place_curseur(ligne, colonne);
        lire_clavier(test_login, TAILLE_LOGIN, VISIBLE);

        format = TEXTE_VERT_C | FOND_BLEU;
        ligne = L_LOGIN + 2;
        colonne = C_LOGIN + 17;
        place_curseur(ligne, colonne);
        lire_clavier(test_mdp, TAILLE_LOGIN, CACHE);
        
        for(i = 0; i < nb_utilisateurs; i++){
            if(!strncmp(utilisateurs[i][0], test_login, TAILLE_LOGIN)
               && !strncmp(utilisateurs[i][1], test_mdp, TAILLE_LOGIN)){
                ok = 1;
                
                strncpy(utilisateur, test_login, TAILLE_LOGIN);
                sprintf(gui, "UTILISATEUR : %-21s", test_login);
                maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);
                
                break;
            }
        }
        
        if(!ok)
            identifiants_incorrects();

        free(test_login);
        free(test_mdp);
    }
}
