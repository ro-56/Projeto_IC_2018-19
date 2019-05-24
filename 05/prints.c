#include "defs.h"

/* ------ */
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

/* ------ */
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

/* ------ */
void printRelatorio(individuo indiv,
                    int ESPECIES,
                    int *temp_proc,
                    double time)
{
    FILE *f;
    f = fopen("relatorio.txt","w");
    
    
    /*  Print solucao  */
    fprintf(f, "Individuo: \n");
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
                    fprintf(f,"%d ",c_walker->idx + 1);
                }
            }
            else
            {
                fprintf(f,"%d ",c_walker->idx + 1);
            }
            c_walker = c_walker->next_crop;
        }
        fprintf(f, "\n");
        p_walker = p_walker->next_plot;
    }
    
    
    /*  Print funcao objetivo  */
    fprintf(f, "\nFuncao Obj: %d\n", indiv.f_obj[0]);
    
    
    /*  Print demanda atendida  */
    fprintf(f, "\nDemanda atendida:\n");
    for (i = 0; i < ESPECIES; i++)
    {
        fprintf(f, "%d ", indiv.demanda_atendida[i]);
    }
    fprintf(f, "\n");
    
    
    /*  Print o tempo de processamento  */
    fprintf(f, "\nTempo: %lf (s)\n", time);
    
    
    fclose(f);
    
    return;
}

/* ------ */
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
