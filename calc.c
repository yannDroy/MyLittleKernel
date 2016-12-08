#include <inttypes.h>
#include <cpu.h>
#include <stdio.h>
#include "clavier.h"
#include "liste.h"
#include "ecran.h"
#include "malloc.h"
#include "atoi.h"
#include "calc.h"

extern int8_t format;

void calc () {
    char *input;
    char **tokens;
    liste execution;
    int32_t nb_operandes;
    int32_t i, j;
    int8_t ok;
    
    sti();

    printf("\n  *** PostFix Calculatrice ***\n\n");
    printf(" 'q' pour quitter\n\n");

    execution = liste_vide();
    
    ok = 1;
    while(ok){
        tokens = (char**) malloc(MAX_OPERANDES * sizeof(char*));
        for(i = 0; i < MAX_OPERANDES; i++){
            tokens[i] = (char*) malloc(TAILLE_OPERANDE * sizeof(char));
            for(j = 0; j < TAILLE_OPERANDE; j++)
                tokens[i][j] = '\0';
        }

        format = TEXTE_CYAN | FOND_NOIR;
        printf("> ");
        
        nb_operandes = 0;

        format = TEXTE_BLANC | FOND_NOIR;
        input = lire_clavier(MAX_TAILLE_BUFFER, VISIBLE);

        if(!strcmp(input, "q")){
            ok = 0;
        }else{
            decouper_expression(input, tokens, &nb_operandes);
            calculer_expression(execution, nb_operandes, tokens);
        }
        
        for(i = 0; i < MAX_OPERANDES; i++)
            free(tokens[i]);
        free(tokens);
        free(input);
    }
}

void calculer_expression (liste p, int32_t n, char **op) {
    int32_t i;
    int32_t x, y;
    int32_t r;

    for(i = 0; i < n; i++){
        switch(op[i][0]){
        case '+':
            x = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            y = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            r = x + y;
            p = insere_element(r, p);
            break;
        case '*':
            x = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            y = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            r = x * y;
            p = insere_element(r, p);
            break;
        case '-':
            if(strlen(op[i]) == 1){
                x = renvoie_premier_element(p);
                p = supprimer_premier_element(p);
                y = renvoie_premier_element(p);
                p = supprimer_premier_element(p);
                r = x - y;
                p = insere_element(r, p);
            }else{
                p = insere_element(atoi(op[i]), p);
            }
            break;
        case '/':
            x = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            y = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            r = x / y;
            p = insere_element(r, p);
            break;
        case '%':
            x = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            y = renvoie_premier_element(p);
            p = supprimer_premier_element(p);
            r = y % x;
            p = insere_element(r, p);
            break;
        default:
            p = insere_element(atoi(op[i]), p);
        }
    }

    r = renvoie_premier_element(p);
    p = supprimer_premier_element(p);

    format = TEXTE_ROUGE | FOND_NOIR;
    printf(" %d\n", r);
}

void decouper_expression (char* chaine, char **tokens, int32_t *n) {
    char *token;
    int32_t i;

    i = 0;
    while((token = strtok(chaine, " ")) != NULL){
        if(strcmp(token, "")){
            strncpy(tokens[i], token, TAILLE_OPERANDE);
            (*n)++;
            i++;

            if(i == MAX_OPERANDES)
                break;
        }
        chaine = NULL;
    }

    if(i < MAX_OPERANDES)
        strcpy(tokens[i], "");
}
