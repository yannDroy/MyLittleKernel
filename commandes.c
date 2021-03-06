#include <cpu.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "init.h"
#include "horloge.h"
#include "ordonnancement.h"
#include "aleatoire.h"
#include "ecran.h"
#include "clavier.h"
#include "scroll.h"
#include "atoi.h"
#include "malloc.h"
#include "gui.h"
#include "commandes.h"

extern Processus table_processus[TAILLE_TABLE_PROCESSUS];
extern uint32_t nombre_processus;
extern int32_t indice_actif;

extern int32_t nb_utilisateurs;
extern char utilisateurs[][2][TAILLE_LOGIN];
extern char utilisateur[TAILLE_LOGIN];
extern char ancien_utilisateur[TAILLE_LOGIN];

extern int8_t sauvegarde_active;

extern uint8_t format;

extern int32_t temps_veille_sec;

extern int32_t arret;

void test (void *n) {
    sti();

    creer_processus(&sleep, "sleep_test 30", (void*) 30);
    creer_processus(&sleep, "sleep_test 60", (void*) 60);
    
    if((int32_t) n)
        creer_processus(&test, "test_dans_test", (void*) ((int32_t) n - 1));

    dors(90);
}

void clear () {
    sti();
    
    sauvegarde_active = 1;
    printf("\f");
    sauvegarde_active = 0;
}

void reset () {
    sti();
    
    printf("\f");
    vider_historique_ecran();
}

void hello (char *s) {
    sti();
    
    if(!s)
        printf("Hello world !\n");
    else
        printf("Hello %s !\n", s);
}

void beer (void *n) {
    int32_t i;
    
    sti();

    for(i = (int32_t) n; i > 0; i--)
        printf("%d more beer%s to drink...\n", i, (i > 1)?"s":"");
}

void rand (void *n) {
    sti();
    printf("Nombre aleatoire : %d\n", crand48() % (int) n);
}

void init_rand (void *n) {
    sti();
    srand((int32_t) n);
}

void set_veille (void *n) {
    sti();

    if((int32_t) n < 5 && (int32_t) n != -1)
        printf("Le delai doit etre d'au moins 5 secondes ou -1 pour une desactivation\n");
    else
        temps_veille_sec = (int32_t) n;
}

void infinity () {
    sti();
    while(1);
}

void fact (void *n) {
    int32_t i;
    int32_t r;
    
    sti();

    r = (int32_t) n;
    
    if(r < 0){
        printf("Nombre positif ou nul attendu !\n");
    }else if(r <= 1){
        printf("%d! = 1\n", r);
    }else{
        for(i = 1; i < (int32_t) n; i++)
            r *= i;
        printf("%d! = %d\n", (int32_t) n, r);
    }
}

void sleep (void *n) {
    sti();
    dors((uint32_t) n);
}

void time () {
    sti();
    printf("Le systeme est allume depuis %d seconde(s)\n", nbr_secondes());
}

void su () {
    char gui[TAILLE_LOGIN + 15];
    char *mdp;
    int32_t i;

    sti();
    
    if(strcmp(utilisateur, "root")){
        printf("Mot de passe : ");
        mdp = lire_clavier(TAILLE_LOGIN, CACHE);

        for(i = 0; i < nb_utilisateurs; i++){
            if(!strcmp(utilisateurs[i][0], "root")
               && !strcmp(utilisateurs[i][1], mdp)){
                strcpy(ancien_utilisateur, utilisateur);
                strncpy(utilisateur, "root\0", 5);
                
                sprintf(gui, "UTILISATEUR : %-21s", utilisateur);
                maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);

                free(mdp);
                
                return;
            }
        }

        printf("Echec d'authentification\n");
    }else{
        printf("Vous etes deja super utilisateur\n");
    }
}

void users () {
    int32_t i, j;

    sti();
    
    if(!strcmp(utilisateur, "root")){
        printf("   *** Liste des utilisateurs du systeme :\n");
        printf("  _______________________________________________\n");
        printf(" |   nom d'utilisateur   |      mot de passe     |\n");
        printf(" |-----------------------+-----------------------|\n");
        
        for(i = 0; i < nb_utilisateurs; i++){
            printf(" |");
            for(j = 0; j < TAILLE_LOGIN + 1 - strlen(utilisateurs[i][0]); j++)
                printf(" ");
            printf("%s |", utilisateurs[i][0]);
            for(j = 0; j < TAILLE_LOGIN + 1 - strlen(utilisateurs[i][1]); j++)
                printf(" ");
            printf("%s |\n", utilisateurs[i][1]);
        }

        printf(" '-----------------------+-----------------------'\n");
    }else{
        printf("Vous devez etre super utilisateur pour voir la liste des utilisateurs\n");
    }
}

void kill (void *n) {
    int32_t pid;
    
    sti();

    pid = (int32_t) n;

    if(pid >= 0 && pid < NB_MAX_PROCESSUS){
        if(pid != indice_actif){
            if(table_processus[pid].proprietaire == ROOT){
                if(!strcmp(utilisateur, "root"))
                    tuer_processus(pid, 1);
                else
                    printf("Vous devez etre super utilisateur pour tuer ce processus\n");
            }else{
                tuer_processus(pid, 1);
            }
        }else{
            printf("Vous ne pouvez pas tuer un processus en cours d'execution\n");
        }
    }else{
        printf("Aucun processus a tuer\n");
    }
}

void quitter (void* arret) {
    char gui[TAILLE_LOGIN + 15];
    
    if(!strcmp(utilisateur, "root")){
        if(strcmp(ancien_utilisateur, "root")){
            strcpy(utilisateur, ancien_utilisateur);
            
            sprintf(gui, "UTILISATEUR : %-21s", utilisateur);
            maj_GUI(gui, C_MAJ_USER, TEXTE_MARRON | FOND_GRIS);
        }else{
            printf("Bye.\n");
            *((int32_t*)arret) = ARRET;
            dors(1);
        }
    }else{
        printf("Bye.\n");
        *((int32_t*)arret) = ARRET;
        dors(1);
    }
}

void deconnexion (void* arret) {
    sti();
    
    printf("Deconnexion...\n");
    *((int32_t*)arret) = DECONNEXION;
    dors(1);
}

void help () {
    sti();

    format = TEXTE_BLANC | FOND_NOIR;
    printf("   *** Liste des commandes shell :\n\n");

    format = TEXTE_CYAN | FOND_NOIR;
    printf(" Systeme :\n");
    printf("  - clear : nettoie l'ecran\n");
    printf("  - reset : nettoie l'ecran et supprime son historique\n");
    printf("  - su : passe en mode super utilisateur\n");
    printf("  - users : affiche la liste des utilisateurs du systeme\n");
    printf("  - jobs : affiche les processus en cours d'execution\n");
    printf("  - sleep <entier> : sieste de <entier> secondes\n");
    printf("  - kill <entier> : termine le processus de PID <entier>\n");
    printf("  - time : donne le temps d'allumage du systeme\n");
    printf("  - set_veille <entier> : change le delai avant la veille (-1 desactive)\n");
    printf("  - veille : lance l'ecran de veille\n");
    printf("  - shell : lance un interpreteur de commandes\n");
    printf("  - lock : verouille la session\n");
    printf("  - exit : sort du mode super utilisateur ou quitte le shell\n");
    printf("  - logout : deconnexion\n");
    printf("  - help : affiche cette aide\n\n");

    format = TEXTE_JAUNE | FOND_NOIR;
    printf(" Mathematiques :\n");
    printf("  - srand <entier> : initialise la suite d'entiers aleatoire\n");
    printf("  - rand <entier> : calcule un entier aleatoire entre 0 et <entier>\n");
    printf("  - fact <entier> : calcule la factorielle de <entier>\n");
    printf("  - pascal <entier> : triangle de Pascal jusqu'a la <entier>-ieme ligne\n");
    printf("  - fibo <entier> : suite de Fibonacci jusqu'au rang U_<entier>\n");
    printf("  - calc : calculatrice en ligne de commandes (notation post-fixee)\n\n");

    format = TEXTE_BLEU_C | FOND_NOIR;
    printf(" Jeux (srand avant, c'est bien) :\n");
    printf("  - tictactoe : jeu de tic-tac-toe contre une IA\n");
    printf("  - devine [<entier>] : devinez un nombre entre 0 et <entier> (100 par defaut)\n");
    printf("  - rubiks : jeu de Rubik's Cube\n\n");

    format = TEXTE_MARRON | FOND_NOIR;
    printf(" Divers :\n");
    printf("  - hello [<chaine>] : dit bonjour <chaine> ('world' par defaut)\n");
    printf("  - beer <entier> : il reste <entier> biere(s) a boire...\n");
    printf("  - triangle <entier> : dessine un joli triangle de <entier> lignes\n");
    printf("  - test [<entier>]: cree 3 * <entier> + 3 processus de test\n");
    printf("  - infinity : boucle infinie\n");
}

void jobs () {
    int32_t i, j;
    
    sti();

    printf("   *** Liste des processus (%d) :\n", nombre_processus);
    printf("  ___________________________________________________________________________\n");
    printf(" | PID | PPID | PROPRIETAIRE |                nom                |    etat   |\n");
    printf(" |-----+------+--------------+-----------------------------------+-----------|\n");
    
    for(i = 0; i < NB_MAX_PROCESSUS; i++){
        if(table_processus[i].pid >= 0){
            if(table_processus[i].pid < 10)
                printf(" |  %d  |", table_processus[i].pid);
            else if(table_processus[i].pid < 100)
                printf(" |  %d |", table_processus[i].pid);
            else
                printf(" | %d |", table_processus[i].pid);

            if(table_processus[i].ppid < 10 && table_processus[i].ppid >= 0)
                printf("   %d  |", table_processus[i].ppid);
            else if(table_processus[i].ppid < 100 || table_processus[i].ppid < 0)
                printf("  %d  |", table_processus[i].ppid);
            else
                printf("  %d |", table_processus[i].ppid);

            if(table_processus[i].proprietaire == ROOT)
                printf("     ROOT     |");
            else
                printf("  UTILISATEUR |");

            for(j = 0; j < MAX_PROC_NAME - strlen(table_processus[i].nom); j++)
                printf(" ");
            printf("%s ", table_processus[i].nom);

            switch(table_processus[i].etat){
            case MORT:
                printf("|   MORT    |\n");
                table_processus[i].pid = -1;
                break;
            case ELU:
                printf("|    ELU    |\n");
                break;
            case ACTIVABLE:
                printf("| ACTIVABLE |\n");
                break;
            case ENDORMI:
                printf("|  ENDORMI  |\n");
                break;
            case ATTENTE_TERM:
                printf("| ATTENTE_T |\n");
                break;
            default:
                break;
            }
        }
    }

    printf(" '-----+------+--------------+-----------------------------------+-----------'\n");
}

void devine (void *n) {
    char *input;
    int32_t nombre, choix;
    int32_t coups;

    sti();

    sauvegarde_active = 1;
    printf("\f");
    sauvegarde_active = 0;

    if((int32_t) n < 10){
        printf("Le nombre doit etre superieur ou egal a 10\n");
        return;
    }else if((int32_t) n > 1000000){
        printf("Le nombre doit etre inferieur a 1.000.000\n");
        return;
    }
    
    nombre = crand48() % ((int32_t) (n + 1));
    choix = -1;
    coups = 0;

    while(1){
        printf("\f\n  *** Devinez le nombre ***\n\n");
        printf(" Devinez un nombre entre 0 et %d (inclus)\n\n", (int32_t) n);

        if(choix != -1){
            coups++;
            
            if(choix > nombre){
                printf(" C'est moins que %d !\n", choix);
            }else if(choix < nombre){
                printf(" C'est plus que %d !\n", choix);
            }else{
                printf(" Bravo, vous avez trouve le nombre %d en %d coups !\n\n", nombre, coups);
                break;
            }
            choix = -1;
        }

        while(choix < 0 || choix > (int32_t) n){
            printf("\n Faites une proposition : ");
            input = lire_clavier(8, VISIBLE);
            choix = atoi(input);
        }

        free(input);
    }
}

void pascal (void *n) {
    int32_t **matrice;
    int32_t lignes;
    int32_t i, j;

    lignes = (int32_t) n;
    
    matrice = (int32_t**) calloc(lignes, (sizeof(int*)));
    for(i = 0; i < lignes; i++)
        matrice[i] = (int32_t*) calloc((i + 1), (sizeof(int)));
      
    for(i = 0; i < lignes; i++)
        matrice[i][0] = 1;

    for(i = 1; i < lignes; i++){
        for(j = 1; j < i; j++)
            matrice[i][j] = matrice[i - 1][j - 1] + matrice[i - 1][j];
        matrice[i][i] = 1;
    }

    for(i = 0; i < lignes; i++){
        for(j = 0; j < (i + 1); j++)
            printf(" %d", matrice[i][j]);
        printf("\n");
    }
 
    for(i = 0; i < lignes; i++)
        free(matrice[i]);
    free(matrice);
}

void fibonacci (void *n) {
    int32_t i;
    int32_t tmp;
    int32_t u0;
    int32_t u1;
    
    sti();
    
    u0 = 1;
    u1 = 1;

    if((int32_t) n < 0){
        printf("Le parametre doit etre un entier positif ou nul\n");
        return;
    }

    printf(" 1");
    for(i = 0; i < (int32_t) n; i++){
        tmp = u0 + u1;
        u0 = u1;
        u1 = tmp;
        printf(" %d", u0);
    }

    printf("\n");
}

void triangle (void *n) {
    int32_t lignes;
    int32_t nb_espaces, nb_etoiles;
    int32_t i, j;

    lignes = (int32_t) n;
    
    for(i = 1; i <= lignes; i++){
        nb_etoiles = (2 * i) - 1;
        nb_espaces = (((2 * lignes) - 1) - nb_etoiles) / 2;
        
        for(j = 0; j < nb_espaces ; j++)
            printf(" ");
        for(j = 0; j < nb_etoiles ; j++){
            format = (crand48() % 15 + 1) | FOND_NOIR;
            printf("*");
        }
        
        printf("\n");
    }
}
