#include "defs.h"
#include "constantes.h"

void graphBest (individuo indiv)
{
    FILE *f;
    f = fopen("graphBest.txt","a");
    
    fprintf(f,"%d\n",indiv.f_obj[0]);
    
    fclose(f);
    
    return;
}

void graphAverage (individuo *group,
                   int size)
{
    FILE *f;
    f = fopen("graphAverage.txt","a");
    
    int i, sumFObj = 0, average = 0;
    
    for (i = 0; i < size; i++)
    {
        sumFObj += group[i].f_obj[0];
    }
    average = sumFObj/size;
    
    fprintf(f,"%d\n",average);
    
    fclose(f);
    
    return;
}

void resetTxt()
{
    FILE *f, *g;
    f = fopen("graphBest.txt","w");
    g = fopen("graphAverage.txt","w");
    
    fprintf(f,"\n");
    fprintf(g,"\n");
    
    fclose(f);
    fclose(g);
    return;
}
