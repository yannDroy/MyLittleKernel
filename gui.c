#include <stdio.h>
#include <string.h>
#include <cpu.h>
#include "ecran.h"
#include "clavier.h"
#include "ordonnancement.h"
#include "aleatoire.h"
#include "interruption.h"
#include "malloc.h"
#include "horloge.h"
#include "gui.h"

extern int32_t nombre_processus;

extern int32_t nb_utilisateurs;
extern char utilisateurs[][2][TAILLE_LOGIN];
extern char utilisateur[TAILLE_LOGIN];

extern uint32_t ligne;
extern uint32_t colonne;
extern uint8_t format;

int8_t barre_existe = 0;
int8_t veille = 0;
int32_t temps_veille_sec = 30;
int32_t temps_non_actif = 0;

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
    int32_t sec;
    
    ligne = PREMIERE_LIGNE;
    colonne = PREMIERE_COLONNE;

    creer_barre();
    
    maj_GUI("MLK 0.1", C_MAJ_NOM, TEXTE_MAGENTA | FOND_GRIS);
    maj_GUI("|", C_PIPE1, TEXTE_NOIR | FOND_GRIS);

    sprintf(gui, "PROCESSUS : %3d", nombre_processus);
    maj_GUI(gui, C_MAJ_PROC, TEXTE_BLEU | FOND_GRIS);
    maj_GUI("|", C_PIPE2, TEXTE_NOIR | FOND_GRIS);

    sprintf(gui, "UTILISATEUR : %-21s", utilisateur);
    maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);
    
    sec = nbr_secondes();
    sprintf(gui, "%.2d:%.2d:%.2d", (sec / 3600), (sec % 3600) / 60, (sec % 3600) % 60);
    maj_GUI(gui, C_MAJ_CLOCK, TEXTE_ROUGE | FOND_GRIS);
        
    maj_GUI("|", C_PIPE3, TEXTE_NOIR | FOND_GRIS);
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

    format = TEXTE_MAGENTA_C | FOND_NOIR;
    printf("\n\n");
    printf("                       :::   :::   :::        :::    :::\n");
    printf("                     :+:+: :+:+:  :+:        :+:   :+:\n");
    format = TEXTE_BLEU_C | FOND_NOIR;
    printf("                   +:+ +:+:+ +:+ +:+        +:+  +:+\n");
    printf("                  +#+  +:+  +#+ +#+        +#++:++\n");
    format = TEXTE_BLEU | FOND_NOIR;
    printf("                 +#+       +#+ +#+        +#+  +#+\n");
    printf("                #+#       #+# #+#        #+#   #+#\n");
    printf("               ###       ### ########## ###    ###\n\n");

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
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
    printf("                                       ");

    ligne = L_LOGIN + 1;
    colonne = C_LOGIN;
    place_curseur(ligne, colonne);
    printf("          login :                      ");

    ligne = L_LOGIN + 2;
    colonne = C_LOGIN;
    place_curseur(ligne, colonne);
    printf("   mot de passe :                      ");

    ligne = L_LOGIN + 3;
    colonne = C_LOGIN;
    place_curseur(ligne, colonne);
    printf("                                       ");

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

    format = TEXTE_VERT_C | FOND_ROUGE;
    printf("        Identifiants incorrects        ");

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}

void etoile_champ (int32_t i, char c) {
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;

    save_l = ligne;
    save_c = colonne;
    save_format = format;
    
    ligne = L_LOGIN + i;
    colonne = C_LOGIN + 1;
    place_curseur(ligne, colonne);
    
    format = TEXTE_ROUGE_C | FOND_BLEU;
    printf("%c", c);

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

        etoile_champ(1, '*');
        
        ligne = L_LOGIN + 1;
        colonne = C_LOGIN + 18;
        place_curseur(ligne, colonne);
        format = TEXTE_MAGENTA_C | FOND_BLEU;
        test_login = lire_clavier(TAILLE_LOGIN, VISIBLE);

        etoile_champ(1, ' ');
        etoile_champ(2, '*');

        ligne = L_LOGIN + 2;
        colonne = C_LOGIN + 18;
        place_curseur(ligne, colonne);
        format = TEXTE_VERT_C | FOND_BLEU;
        test_mdp = lire_clavier(TAILLE_LOGIN, CACHE);

        etoile_champ(2, ' ');
        
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

void choisir_theme (int32_t n, int8_t i) {
    if(!(n % 4)){
        if(!i)
            format = TEXTE_MAGENTA_C | FOND_NOIR;
        else if(i == 1)
            format = TEXTE_BLEU_C | FOND_NOIR;
        else
            format = TEXTE_BLEU | FOND_NOIR;
    }else if(n % 4 == 1){
        if(!i)
            format = TEXTE_JAUNE | FOND_NOIR;
        else if(i == 1)
            format = TEXTE_MARRON | FOND_NOIR;
        else
            format = TEXTE_ROUGE | FOND_NOIR;
    }else if(n % 4 == 2){
        if(!i)
            format = TEXTE_CYAN_C | FOND_NOIR;
        else if(i == 1)
            format = TEXTE_CYAN | FOND_NOIR;
        else
            format = TEXTE_BLEU_C | FOND_NOIR;
    }else{
        if(!i)
            format = TEXTE_BLANC | FOND_NOIR;
        else if(i == 1)
            format = TEXTE_GRIS | FOND_NOIR;
        else
            format = TEXTE_GRIS_F | FOND_NOIR;
    }
}

void ecran_veille () {
    void *save_ecran;
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;
    uint8_t save_barre;
    uint32_t depart_l;
    uint32_t depart_c;
    int32_t touch;
    int8_t sens_l;
    int8_t sens_c;

    sti();

    save_ecran = (void*) malloc(NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t));
    memcpy(save_ecran, ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), (NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t)));

    veille = 1;
    
    save_l = ligne;
    save_c = colonne;
    save_format = format;
    
    efface_ecran(0);
    save_barre = barre_existe;
    barre_existe = 0;

    depart_l = crand48() % 15;
    depart_c = crand48() % 35;

    sens_l = crand48() % 2;
    sens_c = crand48() % 2;

    touch = crand48() % 4;

    while(veille){
        choisir_theme(touch, 0);
        ligne = depart_l;
        colonne = depart_c;
        place_curseur(depart_l, depart_c);
        printf("        :::   :::   :::        :::    :::");
        ligne = depart_l + 1;
        colonne = depart_c;
        place_curseur(depart_l + 1, depart_c);
        printf("      :+:+: :+:+:  :+:        :+:   :+:");
        
        choisir_theme(touch, 1);
        ligne = depart_l + 2;
        colonne = depart_c;
        place_curseur(depart_l + 2, depart_c);
        printf("    +:+ +:+:+ +:+ +:+        +:+  +:+");
        ligne = depart_l + 3;
        colonne = depart_c;
        place_curseur(depart_l + 3, depart_c);
        printf("   +#+  +:+  +#+ +#+        +#++:++");
        
        choisir_theme(touch, 2);
        ligne = depart_l + 4;
        colonne = depart_c;
        place_curseur(depart_l + 4, depart_c);
        printf("  +#+       +#+ +#+        +#+  +#+");
        ligne = depart_l + 5;
        colonne = depart_c;
        place_curseur(depart_l + 5, depart_c);
        printf(" #+#       #+# #+#        #+#   #+#");
        ligne = depart_l + 6;
        colonne = depart_c;
        place_curseur(depart_l + 6, depart_c);
        printf("###       ### ########## ###    ###");
        
        if(sens_c){
            if(depart_c + 40 == NB_COLONNES - 1){
                sens_c = 1 - sens_c;
                depart_c--;
                touch++;
            }else{
                depart_c++;
            }
        }else{
            if(depart_c == 0){
                sens_c = 1 - sens_c;
                depart_c++;
                touch++;
            }else{
                depart_c--;
            }
        }

        if(sens_l){
            if(depart_l + 6 == NB_LIGNES - 1){
                sens_l = 1 - sens_l;
                depart_l--;
                touch++;
            }else{
                depart_l++;
            }
        }else{
            if(depart_l == 0){
                sens_l = 1 - sens_l;
                depart_l++;
                touch++;
            }else{
                depart_l--;
            }
        }

        dors(1);
        efface_ecran(0);
    }

    if(save_barre)
        init_affichage();
    
    memcpy(ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), save_ecran, (NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t)));
    free(save_ecran);

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}

void prompt_deverouillage () {
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;

    save_l = ligne;
    save_c = colonne;
    save_format = format;
    
    format = TEXTE_BLANC | FOND_ROUGE;

    ligne = L_LOCK;
    colonne = C_LOCK;
    place_curseur(ligne, colonne);
    printf("                                       ");

    ligne = L_LOCK + 1;
    colonne = C_LOCK;
    place_curseur(ligne, colonne);
    printf("   mot de passe :                      ");

    ligne = L_LOCK + 2;
    colonne = C_LOCK;
    place_curseur(ligne, colonne);
    printf("                                       ");

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}

void verouiller () {
    void *save_ecran;
    char *test_mdp;
    uint32_t save_l;
    uint32_t save_c;
    uint8_t save_format;
    int32_t indice;
    int32_t save_temps_veille;
    int8_t ok;

    sti();

    save_ecran = (void*) malloc(NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t));
    memcpy(save_ecran, ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), (NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t)));

    save_l = ligne;
    save_c = colonne;
    save_format = format;

    efface_ecran(0);
    barre_existe = 0;

    save_temps_veille = temps_veille_sec;
    temps_veille_sec = 3;

    for(indice = 0; indice < nb_utilisateurs; indice++){
        if(!strcmp(utilisateurs[indice][0], utilisateur))
            break;
    }

    ok = 0;
    while(!ok){
        prompt_deverouillage();

        ligne = L_LOCK + 1;
        colonne = C_LOCK + 18;
        place_curseur(ligne, colonne);
        format = TEXTE_NOIR | FOND_ROUGE;
        test_mdp = lire_clavier(TAILLE_LOGIN, CACHE);

        if(!strncmp(utilisateurs[indice][1], test_mdp, TAILLE_LOGIN))
            ok = 1;

        free(test_mdp);
    }

     memcpy(ptr_mem(PREMIERE_LIGNE, PREMIERE_COLONNE), save_ecran, (NB_COLONNES * (NB_LIGNES - 1) * sizeof(uint16_t)));
    free(save_ecran);

    init_affichage();

    temps_veille_sec = save_temps_veille;

    ligne = save_l;
    colonne = save_c;
    format = save_format;
    place_curseur(ligne, colonne);
}
