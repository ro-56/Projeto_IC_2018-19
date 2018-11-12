#include "defs.h"
#include "genetic.h"
#include "manipular_indiv.h"

#define PROBABILIDADE_CROSSOVER 0
#define PROBABILIDADE_MUTATION 0.1
#define DIVISION_PERCENTAGE 0.6

void runGeneration (individuo *populacao,
                    int POPULACAO,
                    int PERIODOS,
                    int TERRENOS,
                    int ESPECIES,
                    int PERIODOS_ANO,
                    int *area_terreno,
                    int *temp_proc,
                    int *familia,
                    int *demanda,
                    int *lucrativity,
                    int *productivity,
                    int **per_plantio)
{
 
    /******** MALLOC ********/
    int i,j;
    
    individuo *filhos, *mista, pai1, pai2;
    int *indPai1, *indPai2;
    
    filhos = (individuo *)malloc(POPULACAO * sizeof(individuo));
    for (i = 0; i < POPULACAO; i++)
    {
        filhos[i].demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
        
        filhos[i].solucao = NULL;
    }
    
    mista = (individuo *)malloc((2 * POPULACAO) * sizeof(individuo));
    for (i = 0; i < 2*POPULACAO; i++)
    {
        mista[i].demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
        
        mista[i].solucao = NULL;
    }
    
    indPai1 = (int *)malloc(POPULACAO * sizeof(int));
    indPai2 = (int *)malloc(POPULACAO * sizeof(int));
    
    /******** GENERATION ********/
    
    gerarConjuntoPais (populacao,
                       indPai1,
                       indPai2,
                       POPULACAO);
    
    for (i = 0; i < POPULACAO; i += 2)
    {
        /* Selecionar pais por torneio */
        pai1 = torneio(populacao[indPai1[i]], populacao[indPai2[i]]);
        pai2 = torneio(populacao[indPai1[i+1]], populacao[indPai2[i+1]]);
        
        double rand_num = uniforme(0,1);
        
        /* Probabilidade de Crossover, copia individuos caso contrario */
        if (rand_num <= PROBABILIDADE_CROSSOVER)
        {
            crossover (pai1,
                       pai2,
                       &filhos[i],
                       &filhos[i+1],
                       TERRENOS,
                       ESPECIES,
                       area_terreno,
                       demanda,
                       lucrativity,
                       productivity);
        }
        else
        {
            copiarIndividuo (pai1,
                             &filhos[i],
                             ESPECIES);
            
            copiarIndividuo (pai2,
                             &filhos[i+1],
                             ESPECIES);
        }
        
        mutation (&filhos[0],
                  ESPECIES,
                  PERIODOS,
                  area_terreno,
                  temp_proc,
                  familia,
                  demanda,
                  productivity,
                  per_plantio,
                  PERIODOS_ANO);
        
        
        
    }
    
    return;
}


/* ------ */
void crossover (individuo pai1,
                individuo pai2,
                individuo *filho1,
                individuo *filho2,
                int TERRENOS,
                int ESPECIES,
                int *area_terreno,
                int *demanda,
                int *lucrativity,
                int *productivity)
{
    int division_line = TERRENOS * DIVISION_PERCENTAGE;
    plot *p_walker_filho = NULL, *p_walker_pai = NULL;
    
    /*  Copiar as solucoes dos pais para os filhos  */
    filho1->solucao = copySolution (pai1.solucao,
                                    filho1->solucao);
    
    filho2->solucao = copySolution (pai2.solucao,
                                    filho2->solucao);
    
    
    /*  Realizar o crossover para os filhos na linha divisoria  */
    
        /*  Filho 1  */
    p_walker_filho = filho1->solucao;
    p_walker_pai = pai2.solucao;
    
        /* Ir ate uma casa antes da linha divisoria */
    while (p_walker_filho->next_plot->idx < division_line)
    {
        p_walker_pai = p_walker_pai->next_plot;
        p_walker_filho = p_walker_filho->next_plot;
    }
        /* Limpar a solucao a partir da linha divisoria */
    
    p_walker_filho->next_plot = clearSolution(p_walker_filho->next_plot);
    
    p_walker_filho = copySolution(p_walker_pai,
                                  p_walker_filho);
    
        /*  Filho 1  */
    p_walker_filho = filho2->solucao;
    p_walker_pai = pai1.solucao;
    
        /* Ir ate uma casa antes da linha divisoria */
    while (p_walker_filho->next_plot->idx < division_line)
    {
        p_walker_pai = p_walker_pai->next_plot;
        p_walker_filho = p_walker_filho->next_plot;
    }
        /* Limpar a solucao a partir da linha divisoria */
    
    p_walker_filho->next_plot = clearSolution(p_walker_filho->next_plot);
    
    p_walker_filho = copySolution(p_walker_pai,
                                  p_walker_filho);
    
    
    /* Recalcular demanda_atendida */
    filho1->demanda_atendida = calculateMetDemand (filho1->solucao,
                                                   ESPECIES,
                                                   area_terreno,
                                                   demanda,
                                                   productivity);
    
    filho2->demanda_atendida = calculateMetDemand (filho2->solucao,
                                                   ESPECIES,
                                                   area_terreno,
                                                   demanda,
                                                   productivity);

    /* Calcular funcao objetivo */
    filho1->f_obj = calculateFObj (ESPECIES,
                                   demanda,
                                   lucrativity,
                                   filho1->demanda_atendida);
    
    filho2->f_obj = calculateFObj (ESPECIES,
                                   demanda,
                                   lucrativity,
                                   filho2->demanda_atendida);
    
    return;
}

/* ------ */
void gerarConjuntoPais (individuo *populacao,
                        int *indPai1,
                        int *indPai2,
                        int QTDE)
{
    int i;
    
    for (i=0; i < QTDE; i++)
    {
        indPai1[i] = individuoAleatorio(QTDE);
        indPai2[i] = individuoAleatorio(QTDE);
            // caso primeiro pai igual ao segundo pai
        while (indPai1[i] == indPai2[i])
        {
            indPai2[i] = individuoAleatorio(QTDE);
        }
    }
}

/* ------ */
int individuoAleatorio (int n)
{
    double aux;
    int i;
    
        // número aleatorio entre 0 e 1
    aux = uniforme(0,1);
    
        // i é inteiro entre 0 e n-1
        // ou seja, os índices do vetor selecionados
    i = (int)(1 + aux*n) - 1;
    return i;
}

/* ------ */
    //TODO: rever todo esse processo
void mutation (individuo *indiv,
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
    
    /*  Escolher um terreno aleatorio  */
    int rand_num = inteiro (0, ESPECIES);
    
    plot *p_walker = indiv->solucao;
    crop *c_walker = NULL, *aux = NULL;
    
    /*  Ir ate o terreno escolhido  */
    while (p_walker->idx != rand_num)
    {
        p_walker = p_walker->next_plot;
    }
    c_walker = p_walker->crops;
    
    /*  Contar o numero de plantas no terreno selecionado  */
    int count = 0;
    while (c_walker)
    {
        count++;
        c_walker = c_walker->next_crop;
    }
    
    /*  Se o terreno tiver poucas plantas, gerar novo aleatorio*/
    if (count < 2)
    {
        
         /*  Calcular demanda atendida  */
        
        return;
    }
    
    /*  Selecionar uma planta aleatoria  */
    rand_num = inteiro (0, count - 1);
    
    /*  Ir ate planta escolhida  */
    c_walker = p_walker->crops;
    int i;
    for (i = 0; i < rand_num; i++)
    {
        c_walker = c_walker->next_crop;
    }
    aux = c_walker->next_crop;
    
    /*  Trocar a ordem das plantas  */
    c_walker->next_crop = aux->next_crop;
    aux->next_crop = c_walker;
    
    if (count == 2)
    {
        indiv->solucao->crops = aux;
    }
    else
    {
        c_walker = indiv->solucao->crops;
        for (i = 0; i < rand_num-1; i++)
        {
            c_walker = c_walker->next_crop;
        }
        
        c_walker->next_crop = aux;
    }
    
    /*  Checar se novo terreno é factivel  */
    
    
    /*  Caso seja factivel, calcular demanda atendida e retornar  */
    
    
    /*  Caso nao seja factivel, gerar novo terreno aleatorio  */
    printf("%d \n", p_walker->idx);
    
    preencherTerrenoAleatoriamente (indiv,
                                    p_walker->idx,
                                    ESPECIES,
                                    PERIODOS,
                                    area_terreno,
                                    temp_proc,
                                    familia,
                                    demanda,
                                    productivity,
                                    per_plantio,
                                    PERIODOS_ANO);
    
    /*  Calcular demanda atendida  */
    
    
    return;
}

/* ------ */
individuo torneio (individuo pai1,
                   individuo pai2)
{
    int aux = pai1.f_obj - pai2.f_obj;
    
    if (aux < 0) //p1 < p2
    {
        return pai2;
    }
    else if (aux > 0) //p1 > p2
    {
        return pai1;
    }
    else
    {
        if (uniforme(0,1) <= 0.5)
        {
            return pai1;
        }
        else
        {
            return pai2;
        }
    }
}

/* ------ */
