
plot* alocarSolucao (plot *solucao,
                     int TERRENOS);

int* calculateMetDemand (plot *solucao,
                         int ESPECIES,
                         int *area_terreno,
                         int *demanda,
                         int *productivity);

individuo clearIndividuo (individuo indiv,
                          int ESPECIES);

plot* clearPlot(plot *p);

plot* clearSolution(plot *p);

void copyIndividuo (individuo from_ind,
                    individuo *to_ind,
                    int ESPECIES);

plot* copyPlot (plot *from_p,
                plot *to_p);

plot* copySolution (plot *from_p,
                    plot *to_p);

void preencherTerreno (individuo *indiv,
                       plot *p,
                       int ESPECIES,
                       int PERIODOS,
                       int *area_terreno,
                       int *temp_proc,
                       int *familia,
                       int *demanda,
                       int *productivity,
                       int **per_plantio,
                       int PERIODOS_ANO);
