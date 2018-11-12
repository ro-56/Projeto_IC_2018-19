#include "defs.h"
#include "manipular_indiv.h"
#include "individuo.h"
/*    */












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
    plot *p_walker = p, *p_deleter = NULL;
    
    while (p_walker)
    {
        p_deleter = p_walker;
        p_deleter = clearPlot(p_deleter);
        p_walker = p_walker->next_plot;
        free(p_deleter);
    }
    p = NULL;
    
    return p;
}

/* ------ */
void copiarIndividuo (individuo from_ind,
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
plot* copySolution (plot *from_p,
                    plot *to_p)
{
    plot *p_walker_to = to_p, *p_walker_from = from_p;
    crop *c_walker_from = NULL;
    
    /*  Ponteiro "from" comeca no inicio da lista  */
    while (p_walker_from)
    {
        /*  Adicionar um novo terreno no fim da lista do ponteiro original "to"  */
        to_p = addPlot (to_p,
                        p_walker_from->idx,
                        p_walker_from->free_time,
                        NULL,
                        NULL);
        
        p_walker_to = to_p;
        /*  Ponteiro "to" vai ate o final da lista  */
        while (p_walker_to->next_plot)
        {
            p_walker_to = p_walker_to->next_plot;
        }
        
        /*  Ponteiro "from" le todas as plantas do terreno onde esta  */
        c_walker_from = p_walker_from->crops;
        while (c_walker_from)
        {
            /*  Adicionar as plantas lidas no terreno do ponteiro "to"  */
            p_walker_to->crops = addCrop(p_walker_to->crops,
                                         c_walker_from->idx,
                                         c_walker_from->inicio,
                                         (c_walker_from->free_time - c_walker_from->inicio));
            
            c_walker_from = c_walker_from->next_crop;
        }
        
        /*  Ponteiro "from" vai ate o proximo terreno  */
        p_walker_from = p_walker_from->next_plot;
    }
    return to_p;
}

/* ------ */
void preencherTerrenoAleatoriamente (individuo *indiv,
                                     int plot_idx,
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
    plot *p = indiv->solucao;
    
    puts("A");
    printIndividuo((*indiv));
    
    while (p->idx != plot_idx)
    {
        printf(": %d :",p->idx);
        p = p->next_plot;
    }
    puts(" ");
    
    /*  Zerar terreno  */
//    clearPlot(p);
    
    puts("B");
    printIndividuo((*indiv));
    
    int i;
    for (i = 0; i < ESPECIES; i++)
    {
        printf("-%d-",indiv->demanda_atendida[i]);
    }
    puts(" ");
    
    /*  Calcular nova demanda atendida  */
    indiv->demanda_atendida = calculateMetDemand (indiv->solucao,
                                                  ESPECIES,
                                                  area_terreno,
                                                  demanda,
                                                  productivity);
   
    
    for (i = 0; i < ESPECIES; i++)
    {
        printf("-%d-",indiv->demanda_atendida[i]);
    }
    puts(" ");
    
    puts("C");
    printIndividuo((*indiv));
    /*  Preencher o novo terreno  */
    printf("\n!p: %d, %d !\n",p->free_time, p->idx);
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
    
    puts("D");
    printIndividuo((*indiv));
    
    return;
}
