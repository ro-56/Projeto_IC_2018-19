#include <time.h>
#include <sys/time.h>
#include "defs.h"
#include "constantes.h"


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
    

    runGenerations(populacao,
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
    
    
    /*************************** PRINT ******************************/
    gettimeofday(&stop_time, NULL);
    resul = (double)(stop_time.tv_sec - start_time.tv_sec);
    resul += (stop_time.tv_usec - start_time.tv_usec)/(double)MICRO_PER_SECOND;
    printf("Tempo: %lf (s)\n", resul);  // em segundos
    
    printRelatorio(populacao[0],
                   ESPECIES,
                   temp_proc,
                   resul);
    
    return 0;
}
