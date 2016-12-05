#include <inttypes.h>
#include "atoi.h"

int32_t atoi (char* c) {
    int32_t valeur;
    int32_t signe;

    valeur = 0;
    signe = 1;
    
    if(*c == '+' || *c == '-'){
        if(*c == '-')
            signe = -1;
        c++;
    }
    
    while(*c >= '0' && *c <= '9'){
        valeur *= 10;
        valeur += (int32_t) (*c - '0');
        c++;
    }

    return valeur * signe;
}
