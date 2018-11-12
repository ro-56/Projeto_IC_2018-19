#include <time.h>
#include <sys/time.h>
#include "defs.h"

#define MICRO_PER_SECOND 1000000
#define POPULACAO 2
#define GENERATIONS 2

int main ()
{
    /********************* Declaração de Variáveis ********************/
    int PERIODOS;
    int ESPECIES;
    int TERRENOS;
    int PERIODOS_ANO;
    
    int *area_terreno;
    int *temp_proc;
    int *familia;
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
             &familia,
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
                                      familia,
                                      demanda,
                                      lucrativity,
                                      productivity,
                                      per_plantio,
                                      PERIODOS_ANO);
    }
    
    ordenarPopulacao (populacao,
                      POPULACAO);
    
    resetTxt();
    
    /*************************** GENETICO ******************************/
    
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
                      familia,
                      demanda,
                      lucrativity,
                      productivity,
                      per_plantio);
        
        graphBest (populacao[0]);
        
        graphAverage (populacao,
                      POPULACAO);
        
    }
    
    /*************************** PRINT ******************************/
    gettimeofday(&stop_time, NULL);
    resul = (double)(stop_time.tv_sec - start_time.tv_sec);
    resul += (stop_time.tv_usec - start_time.tv_usec)/(double)MICRO_PER_SECOND;
    printf("Tempo: %lf (s)\n", resul);  // em segundos
    
    return 0;
}
