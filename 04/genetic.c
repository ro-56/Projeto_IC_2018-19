#include "defs.h"
#include "genetic.h"
#include "manipular_indiv.h"

#define N_ATTRIBUTES 4
/*
 Matrix [N]x[M]
 N: Individuos na populacao mista
 M: Numero de atributos
    M[i][0] - Origem do Indv. (0:Pai, 1:Filho)
    M[i][1] - Indice do Indv. na populacao original ([0,1,...,POPULACAO])
    M[i][2] - Passou p/ a proxima geracao ([0,1])
    M[i][3] - Funcao Objetivo 1
    ... 1000 - f1 para f2

*/

#define PROBABILIDADE_CROSSOVER 0.8
#define PROBABILIDADE_MUTATION 0.1
#define DIVISION_PERCENTAGE 0.6
#define ELITISM_FACTOR 0.05

void runGenerations (int GENERATION,
                     individuo *populacao,
                     int POPULACAO,
                     int PERIODOS,
                     int TERRENOS,
                     int ESPECIES,
                     int ADJ_EDGES,
                     int PERIODOS_ANO,
                     int *area_terreno,
                     int *temp_proc,
                     int *familia,
                     int *demanda,
                     int *lucrativity,
                     int *productivity,
                     int **ter_adjacent,
                     int **per_plantio)
{
 
    graphBest (populacao[0]);
    
    graphAverage (populacao,
                  POPULACAO);
    
    /******** MALLOC ********/
    int i, j, gen;
    
    individuo *filhos, pai1, pai2;
    int **mista;
    int *indPai1, *indPai2;
    
    filhos = (individuo *)malloc(POPULACAO * sizeof(individuo));
    for (i = 0; i < POPULACAO; i++)
    {
        filhos[i].demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
        
        filhos[i].solucao = alocarSolucao (filhos[i].solucao,
                                           TERRENOS);
    }
    
    mista = (int **)malloc((2 * POPULACAO) * sizeof(int*));
    for (i = 0; i < (2 * POPULACAO); i++)
        mista[i] = (int *)calloc(N_ATTRIBUTES, sizeof(int));
    
    indPai1 = (int *)malloc(POPULACAO * sizeof(int));
    indPai2 = (int *)malloc(POPULACAO * sizeof(int));
    
    /******** GENERATION ********/
    
    for (gen = 0; gen < GENERATION; gen++)
    {
        gerarConjuntoPais (populacao,
                           indPai1,
                           indPai2,
                           POPULACAO);

        for (i = 0; i < POPULACAO; i += 2)
        {
            /* Selecionar pais por torneio */
            pai1 = torneio(populacao[indPai1[i]], populacao[indPai2[i]]);
            pai2 = torneio(populacao[indPai1[i+1]], populacao[indPai2[i+1]]);
            
            /* Probabilidade de Crossover, copia individuos caso contrario */
            double rand_num = uniforme(0,1);
            if (rand_num <= PROBABILIDADE_CROSSOVER)
            {
                crossover (pai1,
                           pai2,
                           &filhos[i],
                           &filhos[i+1],
                           TERRENOS,
                           ESPECIES,
                           ADJ_EDGES,
                           area_terreno,
                           demanda,
                           lucrativity,
                           productivity,
                           ter_adjacent);
            }
            else
            {
                copyIndividuo (pai1,
                               &filhos[i],
                               ESPECIES);

                copyIndividuo (pai2,
                               &filhos[i+1],
                               ESPECIES);
            }

            /*  Probabilidade de Mutacao em cada individuo */
            rand_num = uniforme(0,1);
            if (rand_num <= PROBABILIDADE_MUTATION)
            {
                mutation (&filhos[i],
                          TERRENOS,
                          ESPECIES,
                          PERIODOS,
                          ADJ_EDGES,
                          area_terreno,
                          temp_proc,
                          familia,
                          demanda,
                          lucrativity,
                          productivity,
                          ter_adjacent,
                          per_plantio,
                          PERIODOS_ANO);
            }

            rand_num = uniforme(0,1);
            if (rand_num <= PROBABILIDADE_MUTATION)
            {
                mutation (&filhos[i+1],
                          TERRENOS,
                          ESPECIES,
                          PERIODOS,
                          ADJ_EDGES,
                          area_terreno,
                          temp_proc,
                          familia,
                          demanda,
                          lucrativity,
                          productivity,
                          ter_adjacent,
                          per_plantio,
                          PERIODOS_ANO);
            }

        }
        
        /*  Juntar pais e filhos na populacao mista e organiza-la  */
        mergePopulations (populacao,
                          filhos,
                          mista,
                          POPULACAO);
        
        ordenarPopulacaoMistaFObj (mista,
                                   2*POPULACAO);
        
        /*  Criar uma nova populacao com os melhores individuos e organiza-la  */
        makeNewPopulation (populacao,
                           filhos,
                           mista,
                           POPULACAO,
                           ESPECIES);

        ordenarPopulacao (populacao,
                          POPULACAO);
        
        /*  Liberar regiao de memoria das populacao ja utilizadas  */
        for (i = 0; i < POPULACAO; i++)
        {
            filhos[i] = clearIndividuo (filhos[i],
                                        ESPECIES);
        }
        
        /* Limpar matriz populacao mista */
        for (i = 0; i < 2*POPULACAO; i++)
        {
            for (j = 0; j < N_ATTRIBUTES; j++)
            {
                mista[i][j] = 0;
            }
        }
        
        /*  Grava funcao objetivo para uso em graficos  */
        graphBest (populacao[0]);
        
        graphAverage (populacao,
                      POPULACAO);
        
    }
    
    return;
}


/* ------ */
// Maybe another division method?
// 1 plot from pai1 -> 1 from pai2 -> 1 from pai1 -> ...
void crossover (individuo pai1,
                individuo pai2,
                individuo *filho1,
                individuo *filho2,
                int TERRENOS,
                int ESPECIES,
                int ADJ_EDGES,
                int *area_terreno,
                int *demanda,
                int *lucrativity,
                int *productivity,
                int **ter_adjacent)
{
    int division_line = TERRENOS * DIVISION_PERCENTAGE;

    plot *p_walker_pai_1 = pai1.solucao, *p_walker_pai_2 = pai2.solucao;
    plot *p_walker_filho_1 = filho1->solucao, *p_walker_filho_2 = filho2->solucao;

    /*  Copiar terrenos para os filhos ate a linha divisoria  */
    while (p_walker_pai_1->idx < division_line)
    {
        p_walker_filho_1 = copyPlot(p_walker_pai_1,
                                    p_walker_filho_1);
        
        p_walker_filho_2 = copyPlot(p_walker_pai_2,
                                    p_walker_filho_2);
        
        p_walker_filho_1 = p_walker_filho_1->next_plot;
        p_walker_filho_2 = p_walker_filho_2->next_plot;
        p_walker_pai_1 = p_walker_pai_1->next_plot;
        p_walker_pai_2 = p_walker_pai_2->next_plot;
    }
    
    /*  Copiar terrenos para os filhos depois da linha divisoria  */
    while (p_walker_pai_1)
    {
        p_walker_filho_1 = copyPlot(p_walker_pai_2,
                                    p_walker_filho_1);
        
        p_walker_filho_2 = copyPlot(p_walker_pai_1,
                                    p_walker_filho_2);
        
        p_walker_filho_1 = p_walker_filho_1->next_plot;
        p_walker_filho_2 = p_walker_filho_2->next_plot;
        p_walker_pai_1 = p_walker_pai_1->next_plot;
        p_walker_pai_2 = p_walker_pai_2->next_plot;
    }

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
    filho1->f_obj = calculateFObj (filho1->solucao,
                                   ESPECIES,
                                   ADJ_EDGES,
                                   demanda,
                                   lucrativity,
                                   ter_adjacent,
                                   filho1->demanda_atendida);

    filho2->f_obj = calculateFObj (filho2->solucao,
                                   ESPECIES,
                                   ADJ_EDGES,
                                   demanda,
                                   lucrativity,
                                   ter_adjacent,
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
void makeNewPopulation (individuo *new_population,
                        individuo *filhos,
                        int **combined_group,
                        int POPULACAO,
                        int ESPECIES)
{
    double *prob;
    prob = (double *)calloc(2 * POPULACAO, sizeof(double));

    int num_elite = POPULACAO * ELITISM_FACTOR;
    if (num_elite == 0) // Fail-Safe for small population size
        num_elite = 1;
    
    int i, j, sum_fobj = 0;
    int num_items_selected = num_elite;
    double rand_num;
    
    /* Separar os melhores individuos */
    for (i = 0; i < num_elite; i++)
        combined_group[i][2] = 1;

    /*  Calcula a soma da funcao Obj de todos os individuos do grupo  */
    for (i = num_elite; i < 2 * POPULACAO; i++)
    {
        if (combined_group[i][3] <= 0)
            continue;

        sum_fobj += combined_group[i][3];
    }

    /* Calcular a probabilidade de cada individuo ser escolhido */
    for (i = 0; i < 2 * POPULACAO; i++) // Redundante
        prob[i] = 0;
    
    for (i = num_elite; i < 2 * POPULACAO; i++)
    {
        if (combined_group[i][3] <= 0)
        {
            prob[i] = prob[i-1];
        }
        else
        {
            prob[i] = prob[i-1] + ((double)combined_group[i][3] / (double)sum_fobj);
        }
    }

    /* Selecionar os individuos baseado na probabilidade calculada */
        //TODO: Colocar outro modo de selecionar caso passe de X iteracoes
    while (num_items_selected < POPULACAO)
    {
        rand_num = uniforme(0,1);

        for (i = num_elite; i < 2 * POPULACAO; i++)
            if (prob[i-1] <= rand_num && rand_num < prob[i])
                break;
        
        if (combined_group[i][2])
            continue;

        combined_group[i][2] = 1;

        num_items_selected++;
    }

    /* Limpar a populacao nova nos indices nao escolhidos */
    int *pop_cleared = (int *)calloc(POPULACAO, sizeof(int));
    
    for (i = 0; i < 2 * POPULACAO; i++)
    {
        if (!combined_group[i][2] && !combined_group[i][0])
        {
            int idx = combined_group[i][1];
            
            pop_cleared[idx] = 1;
            
            new_population[idx] = clearIndividuo (new_population[idx],
                                                  ESPECIES);
        }
    }
    /* Copiar os individuos para a nova populacao */
    for (i = 0; i < 2 * POPULACAO; i++)
    {
        if (combined_group[i][2] && combined_group[i][0])
        {
            int idx = combined_group[i][1];

            for (j = 0; j < POPULACAO; j++)
            {
                if (pop_cleared[j])
                {
                    pop_cleared[j] = 0;

                    copyIndividuo (filhos[idx],
                                   &(new_population[j]),
                                   ESPECIES);

                    break;
                }
            }
        }
    }
    
    return;
}

/* ------ */
void mergePopulations (individuo *group_A,
                       individuo *group_B,
                       int **combined_group,
                       int POPULACAO)
{
//M[i][0] - Origem do Indv. (0:Pai, 1:Filho)
//M[i][1] - Indice do Indv. na populacao original ([0,1,...,POPULACAO])
//M[i][2] - Passou p/ a proxima geracao ([0,1])
//M[i][3] - Funcao Objetivo 1
    
    int i;
    
    for (i = 0; i < POPULACAO; i++)
    {
        /* Passar Pais */
        combined_group[i][0] = 0;

        combined_group[i][1] = i;

        combined_group[i][2] = 0; // Redundante

        combined_group[i][3] = group_A[i].f_obj;
    }
    
    for (i = POPULACAO; i < 2*POPULACAO; i++)
    {
        /* Passar Filhos */
        combined_group[i][0] = 1;
        
        combined_group[i][1] = i - POPULACAO;
        
        combined_group[i][2] = 0; // Redundante
        
        combined_group[i][3] = group_B[i - POPULACAO].f_obj;
    }

    return;
}

/* ------ */
void mutation (individuo *indiv,
               int TERRENOS,
               int ESPECIES,
               int PERIODOS,
               int ADJ_EDGES,
               int *area_terreno,
               int *temp_proc,
               int *familia,
               int *demanda,
               int *lucrativity,
               int *productivity,
               int **ter_adjacent,
               int **per_plantio,
               int PERIODOS_ANO)
{
//    crop *c_current = NULL, *c_previous = NULL;
    plot *p_walker = indiv->solucao;

    /*  Ir para um terreno aleatorio  */
    int i, rand_num = inteiro(0,TERRENOS);;
    for (i = 0; i < rand_num; i++)
    {
        p_walker = p_walker->next_plot;
    }
    
    /*  Limpar terreno selecionado  */
    p_walker = clearPlot(p_walker);
    
    /* Recalcular demanda_atendida */
    indiv->demanda_atendida = calculateMetDemand (indiv->solucao,
                                                  ESPECIES,
                                                  area_terreno,
                                                  demanda,
                                                  productivity);
    
    /*  Preencher terreno  */
    preencherTerreno (indiv,
                      p_walker,
                      ESPECIES,
                      PERIODOS,
                      area_terreno,
                      temp_proc,
                      familia,
                      demanda,
                      productivity,
                      per_plantio,
                      PERIODOS_ANO);
    
    /* Recalcular demanda_atendida */
    indiv->demanda_atendida = calculateMetDemand (indiv->solucao,
                                                  ESPECIES,
                                                  area_terreno,
                                                  demanda,
                                                  productivity);
    
    /* Calcular funcao objetivo */
    indiv->f_obj = calculateFObj (indiv->solucao,
                                  ESPECIES,
                                  ADJ_EDGES,
                                  demanda,
                                  lucrativity,
                                  ter_adjacent,
                                  indiv->demanda_atendida);
    
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


    //TODO: Pos otimizacao de terrenos adcionando adubacao verde em periodos ociosos, influenciando lucratividade e nutrientes.
