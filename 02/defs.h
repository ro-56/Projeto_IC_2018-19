#include <stdio.h>
#include <stdlib.h>
//#include <math.h>
//#include <string.h>
//#include <malloc/malloc.h>
//#include <malloc.h>
//#include <limits.h>


/*** STRUCT ***/

typedef struct crops
{
    int idx;
    int free_time;
    int inicio;
    struct crops* next_crop;
} crop;

typedef struct plots
{
    int idx;
    int free_time;
    struct crops* crops;
    struct plots* next_plot;
} plot;

typedef struct individuo
{
    int f_obj;
    int *demanda_atendida;
    struct plots* solucao;
} individuo;


/*** LEITURA ***/

void lerDados (int *PERIODOS_ANO,
               int *PERIODOS,
               int *ESPECIES,
               int *TERRENOS,
               int **area_terreno,
               int **temp_proc,
               int **familia,
               int **demanda,
               int **lucrativity,
               int **productivity,
               int ***per_plantio);


/*** INDIVIDUO ***/

int calculateFObj (int ESPECIES,
                   int *demanda,
                   int *lucrativity,
                   int *demanda_atendida);

individuo criarIndividuo(int PERIODOS,
                         int ESPECIES,
                         int TERRENOS,
                         int *area_terreno,
                         int *temp_proc,
                         int *familia,
                         int *demanda,
                         int *lucrativity,
                         int *productivity,
                         int **per_plantio,
                         int PERIODOS_ANO);
/*** GENETICO ***/

void runGeneration(individuo *populacao,
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
                   int **per_plantio);

/*** MISC ***/
int compararIndividuos (const void *i,
                        const void *j);

int inteiro (int a,
             int b);

void ordenarPopulacao (individuo *populacao,
                       int POPULACAO);

double uniforme (double a,
                 double b);

/*** PRINTS ***/
void printIndividuo (individuo indiv);

void printIndividuoSolution (individuo indiv,
                             int *temp_proc);

void printTerrenos (plot *t);

/*** GRAFICOS ***/
void graphBest (individuo indiv);

void graphAverage (individuo *group,
                   int POPULACAO);

void resetTxt();
