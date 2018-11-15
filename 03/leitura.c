#include "defs.h"

char name[] = "entrada.txt";

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
               int ***per_plantio)
{
    
    FILE *f;
    char trash[255];
    int i, j;
    
    f = fopen(name, "r");
    if (f == NULL)
    {
        perror(name);
        exit (1);
    }
    
    /*****/
    fscanf(f, "%s", trash);
    fscanf(f, "%d", PERIODOS_ANO);
    
    /*****/
    fscanf(f, "%s", trash);
    fscanf(f, "%d", PERIODOS);
    
    /*****/
    fscanf(f, "%s", trash);
    fscanf(f, "%d", ESPECIES);
    
    /*****/
    fscanf(f, "%s", trash);
    fscanf(f, "%d", TERRENOS);
    
    /*****/
    *area_terreno = (int *)malloc(*TERRENOS * sizeof(int));
    
    fscanf(f, "%s", trash);
    for (i = 0; i < *TERRENOS; i++)
        fscanf(f, "%d", &(*area_terreno)[i]);
    
    /*****/
    *temp_proc = (int *)malloc(*ESPECIES * sizeof(int));
    
    fscanf(f, "%s", trash);
    for (i = 0; i < *ESPECIES; i++)
        fscanf(f, "%d", &(*temp_proc)[i]);
    
    /*****/
    *familia = (int *)malloc(*ESPECIES * sizeof(int));
    
    fscanf(f, "%s", trash);
    for (i = 0; i < *ESPECIES; i++)
        fscanf(f, "%d", &(*familia)[i]);
    
    /*****/
    *demanda = (int *)malloc(*ESPECIES * sizeof(int));
    
    fscanf(f, "%s", trash);
    for (i = 0; i < *ESPECIES; i++)
        fscanf(f, "%d", &(*demanda)[i]);
    
    /*****/
    *lucrativity = (int *)malloc(*ESPECIES * sizeof(int));
    
    fscanf(f, "%s", trash);
    for (i = 0; i < *ESPECIES; i++)
        fscanf(f, "%d", &(*lucrativity)[i]);
    
    /*****/
    *productivity = (int *)malloc(*ESPECIES * sizeof(int));
    
    fscanf(f, "%s", trash);
    for (i = 0; i < *ESPECIES; i++)
        fscanf(f, "%d", &(*productivity)[i]);
    
    /*****/
    *per_plantio = (int **)malloc(*ESPECIES * sizeof(int*));
    for(i = 0; i < *ESPECIES; i++)
        (*per_plantio)[i] = (int *)malloc(2 * sizeof(int));
    
    fscanf(f, "%s", trash);
    for(i = 0; i < *ESPECIES; i++)
    {
        for(j = 0; j < 2; j++)
        {
            fscanf(f, "%d", &(*per_plantio)[i][j]);
        }
    }
    /*****/
    
    return;
}
