#include "defs.h"
#include "manipular_indiv.h"
#include "individuo.h"


/* ------ */
plot* alocarSolucao (plot *solucao,
                     int TERRENOS)
{
    int i;
    solucao = NULL;
    
    for (i = 0; i < TERRENOS; i++)
    {
        solucao = addPlot (solucao,
                           i,
                           0,
                           NULL,
                           NULL);
    }
    
    return solucao;
}

/* ------ */
int* calculateMetDemand (plot *solucao,
                         int ESPECIES,
                         int *area_terreno,
                         int *demanda,
                         int *productivity)
{
    int *demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
    plot *p_walker = solucao;
    crop *c_walker = NULL;
    
    /*  Calcular a demanda atendida da solução  */
    while (p_walker)
    {
        c_walker = p_walker->crops;
        while (c_walker)
        {
            demanda_atendida[c_walker->idx] += area_terreno[p_walker->idx] * productivity[c_walker->idx];
            c_walker = c_walker->next_crop;
        }
        p_walker = p_walker->next_plot;
    }
    
    return demanda_atendida;
}

/* ------ */
individuo clearIndividuo (individuo indiv,
                          int ESPECIES)
{
    /*  Zerar funcao objetivo  */
    indiv.f_obj = 0;
    
    /*  Zerar demanda atendida  */
    free (indiv.demanda_atendida);
    indiv.demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
    
    /*  Zerar solucao  */
    indiv.solucao = clearSolution (indiv.solucao);
    
    return indiv;
}

/* ------ */
plot* clearPlot(plot *p)
{
    crop *c_walker = p->crops, *c_deleter = NULL;
    
    while (c_walker)
    {
        c_deleter = c_walker;
        c_walker = c_walker->next_crop;
        free(c_deleter);
    }
    p->free_time = 0;
    p->crops = NULL;
    
    return p;
}

/* ------ */
plot* clearSolution(plot *p)
{
    plot *p_walker = p;
    
    while (p_walker)
    {
        clearPlot(p_walker);
        p_walker = p_walker->next_plot;
    }
    
    return p;
}

/* ------ */
void copyIndividuo (individuo from_ind,
                    individuo *to_ind,
                    int ESPECIES)
{
    /*  Copiar solucao  */
    to_ind->solucao = copySolution (from_ind.solucao,
                                    to_ind->solucao);
    
    /*  Copiar demanda atendida  */
    int i;
    int *dem = (int *)calloc(ESPECIES, sizeof(int));
    for (i = 0; i < ESPECIES; i++)
    {
        dem[i] = from_ind.demanda_atendida[i];
    }
    to_ind->demanda_atendida = dem;
    
    /*  Copiar funcao objetivo  */
    to_ind->f_obj = from_ind.f_obj;
    
    
    return;
}
/* ------ */
plot* copyPlot (plot *from_p,
                plot *to_p)
{
    crop *c_walker_from = from_p->crops;
    
    while (c_walker_from)
    {
        /*  Adiciona a planta lida no final do terreno  */
        to_p->crops = addCrop(to_p->crops,
                              c_walker_from->idx,
                              c_walker_from->inicio,
                              (c_walker_from->free_time - c_walker_from->inicio));
        
        c_walker_from = c_walker_from->next_crop;
    }
    
    return to_p;
}

/* ------ */
plot* copySolution (plot *from_p,
                    plot *to_p)
{
    plot *p_walker_to = to_p, *p_walker_from = from_p;
    
    /*  Ponteiro "from" comeca no inicio da lista  */
    while (p_walker_from)
    {
        /*  Copiar os plantas do terreno "from" para o terreno "to"  */
        p_walker_to = copyPlot(p_walker_from,
                               p_walker_to);
        
        p_walker_to = p_walker_to->next_plot;
        p_walker_from = p_walker_from->next_plot;
    }
    
    return to_p;
}

/* ------ */
void preencherTerreno (individuo *indiv,
                       plot *p,
                       int ESPECIES,
                       int PERIODOS,
                       int *area_terreno,
                       int *temp_proc,
                       int *familia,
                       int *demanda,
                       int *productivity,
                       int **per_plantio,
                       int PERIODOS_ANO)
{
    while (p->free_time < PERIODOS)
    {
        addNewPlantToPlot (ESPECIES,
                           PERIODOS,
                           area_terreno,
                           temp_proc,
                           familia,
                           demanda,
                           productivity,
                           per_plantio,
                           PERIODOS_ANO,
                           indiv->demanda_atendida,
                           p);
    }
    
    return;
}
