#include <stdlib.h>
#include <time.h>

#include "random.h"

/* Sets the seed for the randomizer based on time */
void SetRandomizerSeed()
{
    srand(time(NULL));
}

/* Gets a random number from the random library */
int GetRNGMod(int mod)
{
    if(!mod)
        return 0;
    return rand()%mod;
}