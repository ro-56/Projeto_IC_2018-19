int* calculateMetDemand (plot *solucao,
                         int ESPECIES,
                         int *area_terreno,
                         int *demanda,
                         int *productivity);

plot* clearPlot(plot *p);

plot* clearSolution(plot *p);

void copiarIndividuo (individuo from_ind,
                      individuo *to_ind,
                      int ESPECIES);

plot* copySolution (plot *from_p,
                    plot *to_p);

void preencherTerrenoAleatoriamente (individuo *indiv,
                                     int plot_idx,
                                     int ESPECIES,
                                     int PERIODOS,
                                     int *area_terreno,
                                     int *temp_proc,
                                     int *familia,
                                     int *demanda,
                                     int *productivity,
                                     int **per_plantio,
                                     int PERIODOS_ANO);
