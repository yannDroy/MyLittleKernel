#ifndef __ORDONNANCEMENT_H__
#define __ORDONNANCEMENT_H__

#include <inttypes.h>

#define MAX_PROC_NAME 100

#define TAILLE_PILE_EXEC 512

#define NB_MAX_PROCESSUS 10
#define TAILLE_TABLE_PROCESSUS 100

enum { ELU, ACTIVABLE, ENDORMI, ATTENTE_TERM, MORT };

/* Structure processus */
typedef struct Processus {
    int32_t pid;
    char nom[MAX_PROC_NAME];
    uint8_t etat;
    uint32_t heure_reveil;
    int32_t registres[5];
    int32_t pile_execution[TAILLE_PILE_EXEC];
} Processus;

/* Renvoie le nom du processus en cours d'execution */
char *mon_nom ();

/* Renvoie le pid du processus en cours d'execution */
uint8_t mon_pid();

/* Ordonnanceur de processus Tourniquet (Round Robin) */
void ordonnance ();

/* Cree le processus idle dans la table des processus */
int32_t creer_processus_init ();

/* Cree un processus dans la table des processus */
int32_t creer_processus (void (*code)(void), char *nom);

/* Termine un processus */
void fin_processus ();

/* Attend la terminaison d'un processus */
void attendre_terminaison (int32_t pid);

/* Initialisation du processus principal */
int8_t init_table_processus ();

#endif
