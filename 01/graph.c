#include "defs.h"

void graphBest ()
{
    FILE *f;
    f = fopen("graphBest.txt","w");
    
    fprintf(f,"100");
    
    fclose(f);
    
    return;
}
