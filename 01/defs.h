#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
//#include <string.h>
//#include <malloc/malloc.h>
//#include <malloc.h>
//#include <limits.h>
/*********************** FUNCTIONS ***********************/

/*** INDIVIDUO ***/
typedef struct individuo
{
    int f_obj;
    int *demanda_atendida;
    int **sol;
} individuo;

int CalcularFuncaoObj (int PERIODOS,
                       int TERRENOS,
                       int *area_terreno,
                       int *demanda,
                       int *lucrativity,
                       int *productivity,
                       int **solucao,
                       int *demanda_atendida);

individuo criarIndividuo (int PERIODOS,
                          int ESPECIES,
                          int TERRENOS,
                          int *area_terreno,
                          int *temp_proc,
                          int *demanda,
                          int *lucrativity,
                          int *productivity,
                          int **per_plantio,
                          int PERIODOS_ANO);

int createRow (int **matrix,
               int *demanda_atendida,
               int ESPECIES,
               int PERIODOS,
               int TERRENOS,
               int PERIODOS_ANO,
               int *temp_proc,
               int *area_terreno,
               int *demanda,
               int *lucrativity,
               int *productivity,
               int **per_plantio,
               int idx_terreno_novo);

/*** LEITURA ***/

void lerDados (int *PERIODOS_ANO, 
               int *PERIODOS,
               int *ESPECIES,
               int *TERRENOS,
               int **area_terreno,
               int **temp_proc,
               int **demanda,
               int **lucrativity,
               int **productivity,
               int ***per_plantio);


/*** GENETICO ***/

void ordenarPopulacao(individuo *populacao,
                      int POPULACAO);

void runGeneration(individuo *populacao,
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
                   int **per_plantio);

/*** RESULTADOS ***/

void displayAverageFObj(individuo *group,
                        int POPULACAO);

void displayBestFObj(individuo *group,
                     int POPULACAO);

void displayDemandaAtendida (individuo indv,
                             int ESPECIES);

/*
void display_best_individuo(individuo *group,
                            int POPULACAO,
                            int PERIODOS,
                            int TERRENOS);
*/

void displayIndividuo(individuo object,
                       int PERIODOS,
                       int TERRENOS);

float getAverageFObj(individuo *group,
                     int POPULACAO);

int getBestFObj(individuo *group,
                int POPULACAO);

/*** MISC ***/

int inteiro(int a,
            int b);

double uniforme(double a,
                double b);

/*** GRAPHS ***/

void graphBest ();
