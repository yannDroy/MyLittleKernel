#ifndef __CLAVIER_H__
#define __CLAVIER_H__

#define CANAL_CLAVIER 1
#define INTERRUPTION_CLAVIER 33

#define PORT_OUT_PIT_KB 0x20
#define PORT_COM_KB     0x64
#define PORT_DATA_KB    0x60

#define MAX_TAILLE_BUFFER 4096

#define VISIBLE 1
#define CACHE   0

/* Fonction assembleur qui traite l'interruption 32 */
extern void traitant_IT_33 ();

/* Traite une interruption venant du clavier */
void clavier_PIT ();

/* Lit sur l'entree standard */
char *lire_clavier (int32_t taille, int8_t mode);

/* Traite l'appui sur une touche */
void traiter_touche (int8_t c);

/* Initialise le clavier */
void init_clavier ();

/* Vide le buffer d'ecriture */
void vider_buffer ();

/* Place un caractere dans le buffer */
void mettre_caractere_buffer (char nr, char sh, char ca, char al);

/* Recupere le code de la touche appuyee */
int8_t recuperer_code (int8_t c);

#define KB_ECHAP      1
#define KB_F1        59
#define KB_F2        60
#define KB_F3        61
#define KB_F4        62
#define KB_F5        63
#define KB_F6        64
#define KB_F7        65
#define KB_F8        66
#define KB_F9        67
#define KB_F10       68
#define KB_F11       87
#define KB_F12       88
#define KB_PRINT     55
#define KB_DEFIL     70
#define KB_PAUSE     69
#define KB_INSER     82
#define KB_SUPPR     83
#define KB_DEBUT     71
#define KB_FIN       79
#define KB_DEBUT_P   73
#define KB_FIN_P     81
#define KB_CARRE     41
#define KB_TAB       15
#define KB_CAPSLOCK  58
#define KB_LSHIFT    42
#define KB_CHEVRON   86
#define KB_PAR_FER   12
#define KB_EGAL      13
#define KB_CHAPEAU   26
#define KB_DOLLAR    27
#define KB_POURCENT  40
#define KB_MULT      43
#define KB_VIRG      50
#define KB_PVIRG     51
#define KB_DPOINTS   52
#define KB_EXCL      53
#define KB_BACKSPACE 14
#define KB_ENTER     28
#define KB_RSHIFT    54
#define KB_1          2
#define KB_2          3
#define KB_3          4
#define KB_4          5
#define KB_5          6
#define KB_6          7
#define KB_7          8
#define KB_8          9
#define KB_9         10
#define KB_0         11
#define KB_A         16
#define KB_B         48
#define KB_C         46
#define KB_D         32
#define KB_E         18
#define KB_F         33
#define KB_G         34
#define KB_H         35
#define KB_I         23
#define KB_J         36
#define KB_K         37
#define KB_L         38
#define KB_M         39
#define KB_N         49
#define KB_O         24
#define KB_P         25
#define KB_Q         30
#define KB_R         19
#define KB_S         31
#define KB_T         20
#define KB_U         22
#define KB_V         47
#define KB_W         44
#define KB_X         45
#define KB_Y         21
#define KB_Z         17
#define KB_LCTRL     29
#define KB_FN         0
#define KB_WIN       91
#define KB_ALT       56
#define KB_ESPACE    57
#define KB_ALTGR     56
#define KB_RCLICK    93
#define KB_RCTRL     29
#define KB_UP        72
#define KB_DOWN      80
#define KB_LEFT      75
#define KB_RIGHT     77

#endif
