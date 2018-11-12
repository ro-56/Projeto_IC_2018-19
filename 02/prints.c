#include "defs.h"


void printIndividuo (individuo indiv)
{
    plot *p_walker = indiv.solucao;
    crop *c_walker = NULL;
    
    while (p_walker)
    {
        c_walker = p_walker->crops;
        while (c_walker)
        {
            printf("%d ",c_walker->idx);
            c_walker = c_walker->next_crop;
        }
        puts("");
        p_walker = p_walker->next_plot;
    }
    
    
    return;
}

void printIndividuoSolution (individuo indiv,
                             int *temp_proc)
{
    plot *p_walker = indiv.solucao;
    crop *c_walker = NULL;
    int i;
    
    while (p_walker)
    {
        c_walker = p_walker->crops;
        while (c_walker)
        {
            if (c_walker->idx >= 0)
            {
                for (i = 0; i < temp_proc[c_walker->idx]; i++)
                {
                    printf("%d ",c_walker->idx + 1);
                }
            }
            else
            {
                printf("%d ",c_walker->idx + 1);
            }
            c_walker = c_walker->next_crop;
        }
        puts("");
        p_walker = p_walker->next_plot;
    }
    
    
    return;
}

void printTerrenos (plot *t)
{
    printf("T: ");
    while(t)
    {
        printf("%d ",t->idx);
        t = t->next_plot;
    }
    puts(" ");
    
    return;
}
