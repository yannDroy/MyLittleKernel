#include <inttypes.h>
#include "aleatoire.h"

int32_t suivant = 123456;

void srand (int32_t n) {
    if(n >= 0)
        suivant = n;
    else
        suivant = -n;
}

int32_t crand48 () {
  int32_t m = (1L << 24);

  suivant = ((0x5DEECE66D * suivant) + 0xB) & (m - 1);

  return suivant;
}
