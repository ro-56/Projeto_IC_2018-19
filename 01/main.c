#include <time.h>
#include <sys/time.h>
#include "defs.h"

#define MICRO_PER_SECOND 1000000
#define POPULACAO 50
#define GENERATIONS 1000

int main ()
{
    /********************* Declaração de Variáveis ********************/
    int PERIODOS;
    int ESPECIES;
    int TERRENOS;
    int PERIODOS_ANO;
    
    int *area_terreno;
    int *temp_proc; // tempos de precessamento
    int *demanda;
    int *productivity;
    int *lucrativity;

    int **per_plantio;  // intervalos de plantio [Ei,Ti]
    
    /* variáveis referentes ao algoritmo genético */
    individuo *populacao;
    
    /* variáveis utilizadas no cálculo do tempo de execução */
    struct timeval start_time;
    struct timeval stop_time;
    double resul;

    gettimeofday(&start_time, NULL);
    srand((double) time(NULL)); // rand() baseada na hora do computador

    /* variáveis auxiliares */
    int i;
    
    /*************************** LEITURA ******************************/
    lerDados(&PERIODOS_ANO,
             &PERIODOS,
             &ESPECIES,
             &TERRENOS,
             &area_terreno,
             &temp_proc,
             &demanda,
             &lucrativity,
             &productivity,
             &per_plantio);
    
    /*************************** INIT ******************************/
    
    populacao = (individuo *)malloc(POPULACAO * sizeof(individuo));
    for (i = 0; i < POPULACAO; i++)
    {
        populacao[i] = criarIndividuo(PERIODOS,
                                      ESPECIES,
                                      TERRENOS,
                                      area_terreno,
                                      temp_proc,
                                      demanda,
                                      lucrativity,
                                      productivity,
                                      per_plantio,
                                      PERIODOS_ANO);
    }
    
    /*************************** GENETICO ******************************/
    
    ordenarPopulacao(populacao, POPULACAO);
//
    printf("Primeira Geracao");
    displayIndividuo(populacao[0],
                     PERIODOS,
                     TERRENOS);
    displayBestFObj(populacao,
                    POPULACAO);
//
    for (i = 0; i < GENERATIONS; i++)
    {
        runGeneration(populacao,
                      POPULACAO,
                      PERIODOS,
                      TERRENOS,
                      ESPECIES,
                      PERIODOS_ANO,
                      area_terreno,
                      temp_proc,
                      demanda,
                      lucrativity,
                      productivity,
                      per_plantio);
    }
//
    printf("Ultima Geracao");
    displayIndividuo (populacao[0],
                      PERIODOS,
                      TERRENOS);
    displayBestFObj (populacao,
                     POPULACAO);
    displayDemandaAtendida (populacao[0],
                            ESPECIES);
    /*************************** TEST ******************************/
    
//    graphBest ();
    
    /*************************** PRINT ******************************/
    gettimeofday(&stop_time, NULL);
    resul = (double)(stop_time.tv_sec - start_time.tv_sec);
    resul += (stop_time.tv_usec - start_time.tv_usec)/(double)MICRO_PER_SECOND;
    printf("Tempo: %lf (s)\n", resul);  // em segundos

//    printf("%d %d %d %d\n",PERIODOS_ANO,PERIODOS,ESPECIES,TERRENOS);
//
//    for(i = 0; i<ESPECIES;i++)
//        printf("%d ",demanda[i]);
//    printf("\n");
//
//    for (i = 0;i<ESPECIES;i++)
//    {
//        for (j=0;j<2;j++)
//        {
//            printf("%d ",per_plantio[i][j]);
//        }
//        printf("\n");
//    }
//
//    for (i = 0;i<ESPECIES;i++)
//    {
//        printf("%d ",lucrativity[i]);
//    }
//    printf("\n");
//
//    printf("\nP_INDIVIDUO i: \n");
//    for(i=0;i<TERRENOS;i++)
//    {
//        for(j=0;j<PERIODOS;j++)
//        {
//            printf("%d ", populacao[0].sol[i][j]);
//        }
//        printf("\n");
//    }
//    for (i=0;i<POPULACAO;i++)
//        printf("%d ",populacao[i].f_obj);
//    printf("\n");

    return 0;
}
