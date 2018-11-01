#include "defs.h"
#include "genetic.h"

#define PROBABILIDADE_CROSSOVER 0.9
#define MAX_CROSSOVER_PERCENTAGE 0.6 //[0.5 , 1]
#define PROBABILIDADE_MUTATION 0.1
#define MUTATION_SIZE 2
// ^^^ Pode dar problema caso a quantidade de terrenos seja menor que esse numero.
#define ELITISM_FACTOR 0.05


void runGeneration (individuo *populacao,
                    int POPULACAO,
                    int PERIODOS,
                    int TERRENOS,
                    int ESPECIES,
                    int PERIODOS_ANO,
                    int *area_terreno,
                    int *temp_proc,
                    int *demanda,
                    int *lucrativity,
                    int *productivity,
                    int **per_plantio)
{

    /*************************** MALLOC ******************************/
    int i,j;
    
    individuo *filhos, *mista, pai1, pai2;
    int *indPai1, *indPai2;
    
    filhos = (individuo *)malloc(POPULACAO * sizeof(individuo));
    for (i = 0; i < POPULACAO; i++)
    {
        filhos[i].sol = (int **)malloc(TERRENOS * sizeof(int *));
        for (j = 0; j < TERRENOS; j++)
        {
            filhos[i].sol[j] = (int *)malloc(PERIODOS * sizeof(int));
        }
        
        filhos[i].demanda_atendida = (int *)malloc(ESPECIES * sizeof(int));
    }
    
    mista = (individuo *)malloc((2 * POPULACAO) * sizeof(individuo));
    for (i = 0; i < 2*POPULACAO; i++)
    {
        mista[i].sol = (int **)malloc(TERRENOS * sizeof(int *));
        for (j = 0; j < TERRENOS; j++)
        {
            mista[i].sol[j] = (int *)malloc(PERIODOS * sizeof(int));
        }
        
        mista[i].demanda_atendida = (int *)malloc(ESPECIES * sizeof(int));
    }
    
    indPai1 = (int *)malloc(POPULACAO * sizeof(int));
    indPai2 = (int *)malloc(POPULACAO * sizeof(int));

    /*************************** GENERATION ******************************/
    
    gerarConjuntoPais (populacao, indPai1, indPai2, POPULACAO);
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
                       PERIODOS,
                       TERRENOS,
                       ESPECIES,
                       area_terreno,
                       demanda,
                       lucrativity,
                       productivity);
        }
        else
        {
            copyIndividuo (pai1,
                           &filhos[i],
                           PERIODOS,
                           TERRENOS,
                           ESPECIES);
            copyIndividuo (pai2,
                           &filhos[i+1],
                           PERIODOS,
                           TERRENOS,
                           ESPECIES);
        }

        /* Mutação de cada filho */
        mutation (filhos[i],
                  ESPECIES,
                  PERIODOS,
                  TERRENOS,
                  PERIODOS_ANO,
                  temp_proc,
                  area_terreno,
                  demanda,
                  lucrativity,
                  productivity,
                  per_plantio);
        
        mutation (filhos[i+1],
                  ESPECIES,
                  PERIODOS,
                  TERRENOS,
                  PERIODOS_ANO,
                  temp_proc,
                  area_terreno,
                  demanda,
                  lucrativity,
                  productivity,
                  per_plantio);
    }
    mergePopulations (populacao,
                      filhos,
                      mista,
                      PERIODOS,
                      TERRENOS,
                      POPULACAO,
                      ESPECIES);

    ordenarPopulacao (mista, 2*POPULACAO);

    makeNewPopulation (populacao,
                       mista,
                       POPULACAO,
                       PERIODOS,
                       TERRENOS,
                       ESPECIES);

    ordenarPopulacao(populacao, POPULACAO);

    /*************************** FREE ******************************/
    
    for (i = 0; i < POPULACAO; i++)
    {
        for (j = 0; j < TERRENOS; j++)
        {
            free(filhos[i].sol[j]);
        }
        free(filhos[i].sol);
    }
    free (filhos);
    free (mista);
    free (indPai1);
    free (indPai2);
}

/*************************** FUNCTIONS ******************************/
/* Calcula a lucratividade de cada individuo */
//int calcularNovaFObj (individuo solucao,
//                      int PERIODOS,
//                      int TERRENOS,
//                      int *lucratividade_especies)
//{
//
//    int i, j, fobj = 0, last = 0;
//
//    for (i = 0; i < TERRENOS; i++)
//    {
//        for (j = 0; j < PERIODOS; j++)
//        {
//            /*
//             Adiciona o lucro da especie no periodo caso seja diferente do
//              periodo anterior e de 0
//            */
//            if (solucao.sol[i][j] != 0 && solucao.sol[i][j] != last)
//            {
//                last = solucao.sol[i][j];
//                fobj += lucratividade_especies[solucao.sol[i][j] - 1];
//            }
//        }
//        last = 0;
//    }
//    return fobj;
//}
/*****/
int compararIndiv (const void *i,
                   const void *j)
{
    individuo *a,*b;
    a = (individuo *)i;
    b = (individuo *)j;
    if(a->f_obj > b->f_obj)    // decrescente
        return -1;
    else if(a->f_obj < b->f_obj)
        return 1;
    else
        return 0;
}
/*****/
/* Copia os campos do .from_ind. para o .to_ind. */
void copyIndividuo (individuo from_ind,
                    individuo *to_ind,
                    int PERIODOS,
                    int TERRENOS,
                    int ESPECIES)
{
    
    int i,j;
    
    (*to_ind).f_obj = from_ind.f_obj;
    
    for (i = 0; i< TERRENOS; i++)
    {
        for (j = 0; j < PERIODOS; j++)
        {
            (*to_ind).sol[i][j] = from_ind.sol[i][j];
        }
    }
    
    for (i = 0; i < ESPECIES; i++)
    {
        (*to_ind).demanda_atendida[i] = from_ind.demanda_atendida[i];
    }
}
/*****/
void crossover (individuo pai1,
                individuo pai2,
                individuo *filho1,
                individuo *filho2,
                int PERIODOS,
                int TERRENOS,
                int ESPECIES,
                int *area_terreno,
                int *demanda,
                int *lucrativity,
                int *productivity)
{
    int i, j;
    int division_line = (int)(TERRENOS * MAX_CROSSOVER_PERCENTAGE);
//    Alternative division_line calculation?
//    division_line = inteiro((int)(TERRENOS * (1- MAX_CROSSOVER_PERCENTAGE)),
//                            (int)(TERRENOS * MAX_CROSSOVER_PERCENTAGE));
//
//    printf("%d ",division_line);
    
    for (i = 0; i < division_line; i++)
    {
        for (j = 0; j < PERIODOS; j++)
        {
            (*filho1).sol[i][j] = pai1.sol[i][j];
            (*filho2).sol[i][j] = pai2.sol[i][j];
        }
    }

    for (i = division_line; i < TERRENOS; i++)
    {
        for (j = 0; j < PERIODOS; j++)
        {
            (*filho1).sol[i][j] = pai1.sol[i][j];
            (*filho2).sol[i][j] = pai2.sol[i][j];
        }
    }
    
        /////////Calculo de demanda atendida nova
    /* Zerar demanda_atendida */
    
    for(i = 0; i < ESPECIES; i++)
    {
        (*filho1).demanda_atendida[i] = 0;
        (*filho2).demanda_atendida[i] = 0;
    }
    
    /* Recalcular demanda_atendida */
    int last_esp = 0;
    for (i = 0; i < TERRENOS; i++)
    {
        for(j = 0; j < PERIODOS; j++)
        {
            if ((*filho1).sol[i][j] == 0 || (*filho1).sol[i][j] == last_esp)
            {
                continue;
            }
            
            last_esp = (*filho1).sol[i][j];
            (*filho1).demanda_atendida[last_esp-1] += area_terreno[i] * productivity[last_esp-1];
            
        }
    }
    for (i = 0; i < TERRENOS; i++)
    {
        for(j = 0; j < PERIODOS; j++)
        {
            if ((*filho2).sol[i][j] == 0 || (*filho2).sol[i][j] == last_esp)
            {
                continue;
            }
            
            last_esp = (*filho2).sol[i][j];
            (*filho2).demanda_atendida[last_esp-1] += area_terreno[i] * productivity[last_esp-1];
            
        }
    }
    
    
        ///////
    
    (*filho1).f_obj = CalcularFuncaoObj (PERIODOS,
                                         TERRENOS,
                                         area_terreno,
                                         demanda,
                                         lucrativity,
                                         productivity,
                                         (*filho1).sol,
                                         (*filho1).demanda_atendida);
    
    (*filho2).f_obj = CalcularFuncaoObj (PERIODOS,
                                         TERRENOS,
                                         area_terreno,
                                         demanda,
                                         lucrativity,
                                         productivity,
                                         (*filho2).sol,
                                         (*filho2).demanda_atendida);

}
/*****/
void gerarConjuntoPais (individuo *populacao,
                        int *indPai1,
                        int *indPai2,
                        int QTDE)
{
    int i;
    
    for (i=0; i < QTDE; i++)
    {
        indPai1[i] = individuo_aleatorio(QTDE);
        indPai2[i] = individuo_aleatorio(QTDE);
        // caso primeiro pai igual ao segundo pai
        while (indPai1[i] == indPai2[i])
        {
            indPai2[i] = individuo_aleatorio(QTDE);
        }
    }
}
/*****/
int individuo_aleatorio (int n)
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
/*****/
void makeNewPopulation (individuo *new_population,
                        individuo *combined_group,
                        int POPULACAO,
                        int PERIODOS,
                        int TERRENOS,
                        int ESPECIES)
{
    
    double *prob;
    prob = (double *)calloc(2 * POPULACAO, sizeof(double));
    
    int *selected;
    selected = (int *)calloc(2 * POPULACAO, sizeof(int));
 
    int num_elite = POPULACAO * ELITISM_FACTOR;
    int i, sum_fobj = 0;
    int num_items_selected = num_elite;
    double rand_num;
    
    /* Calcula a soma da funcao Obj de todos os individuos do grupo */
    for (i = num_elite; i < 2 * POPULACAO; i++)
    {
        if (combined_group[i].f_obj <= 0)
            continue;
            
        sum_fobj += combined_group[i].f_obj;
    }
    
    
    for (i = 0; i < 2 * POPULACAO; i++)
    {
        /* Se o individuo for um dos melhores (elite), copiar o individuo */
        /* Se nao for, cada individuo tera uma probabilidade de ser escolhido baseado em sua FObj */
        if (i < num_elite)
        {
            selected[i] = 1;
            copyIndividuo(combined_group[i],
                          &new_population[i],
                          PERIODOS,
                          TERRENOS,
                          ESPECIES);
        }
        else
        {
            if (combined_group[i].f_obj <= 0)
            {
                prob[i] = prob[i-1];
            }
            else
            {
                prob[i] = prob[i-1] + ((double)combined_group[i].f_obj / (double)sum_fobj);
            }
        }
    }
    
    while (num_items_selected < POPULACAO)
    {
        rand_num = uniforme(0,1);

        for (i = num_elite; i < 2 * POPULACAO; i++)
            if (prob[i-1] <= rand_num && rand_num < prob[i])
                break;

        if (selected[i])
            continue;
        
        copyIndividuo (combined_group[i],
                       &new_population[num_items_selected],
                       PERIODOS,
                       TERRENOS,
                       ESPECIES);

        num_items_selected++;
    }
    
    free(prob);
    free(selected);
    
    return;
}
/*****/
void mergePopulations (individuo *group_A,
                       individuo *group_B,
                       individuo *combined_group,
                       int PERIODOS,
                       int TERRENOS,
                       int POPULACAO,
                       int ESPECIES)
{
    int i,k;
    for (i = 0; i < POPULACAO; i++)
    {
        copyIndividuo (group_A[i],
                       &combined_group[i],
                       PERIODOS,
                       TERRENOS,
                       ESPECIES);
    }
    for (i = 0, k = POPULACAO; i < POPULACAO; i++, k++)
    {
        copyIndividuo (group_B[i],
                       &combined_group[k],
                       PERIODOS,
                       TERRENOS,
                       ESPECIES);
    }
}
/*****/
void mutation (individuo object,
               int ESPECIES,
               int PERIODOS,
               int TERRENOS,
               int PERIODOS_ANO,
               int *temp_proc,
               int *area_terreno,
               int *demanda,
               int *lucrativity,
               int *productivity,
               int **per_plantio)
{
    int *mutated, i, mutations = 0;
    mutated = (int *)calloc(TERRENOS, sizeof(int));
    double aux = uniforme(0,1);
    
    if (aux <= PROBABILIDADE_MUTATION)
    {
        while (mutations < MUTATION_SIZE)
        {
            i = inteiro(0,TERRENOS);
            
            if (mutated[i])
                continue;
            
            object.f_obj = createRow (object.sol,
                                      object.demanda_atendida,
                                      ESPECIES,
                                      PERIODOS,
                                      TERRENOS,
                                      PERIODOS_ANO,
                                      temp_proc,
                                      area_terreno,
                                      demanda,
                                      lucrativity,
                                      productivity,
                                      per_plantio,
                                      i);

            mutated[i] = 1;
            mutations++;
        }
    }
    free(mutated);
    
    return;
}
/*****/
void ordenarPopulacao (individuo *populacao,
                       int POPULACAO)
{
    qsort(populacao,
          POPULACAO,
          sizeof(individuo),
          compararIndiv);
}
/*****/
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
