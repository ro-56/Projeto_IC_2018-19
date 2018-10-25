//int calcularNovaFObj(individuo solucao,
//                     int PERIODOS,
//                     int TERRENOS,
//                     int *lucratividade_especies);

int compararIndiv(const void *i,
                  const void *j);

void copyIndividuo(individuo from_ind,
                   individuo *to_ind,
                   int PERIODOS,
                   int TERRENOS,
                   int ESPECIES);

void crossover(individuo pai1,
               individuo pai2,
               individuo *filho1,
               individuo *filho2,
               int PERIODOS,
               int TERRENOS,
               int *area_terreno,
               int *demanda,
               int *lucrativity,
               int *productivity);

void gerarConjuntoPais(individuo *populacao,
                       int *indPai1,
                       int *indPai2,
                       int QTDE);

int individuo_aleatorio(int n);

void makeNewPopulation(individuo *new_population,
                       individuo *combined_group,
                       int POPULACAO,
                       int PERIODOS,
                       int TERRENOS,
                       int ESPECIES);

void mergePopulations(individuo *group_A,
                      individuo *group_B,
                      individuo *combined_group,
                      int PERIODOS,
                      int TERRENOS,
                      int POPULACAO,
                      int ESPECIES);

void mutation(individuo object,
              int ESPECIES,
              int PERIODOS,
              int TERRENOS,
              int PERIODOS_ANO,
              int *temp_proc,
              int *area_terreno,
              int *demanda,
              int *lucrativity,
              int *productivity,
              int **per_plantio);

individuo torneio(individuo pai1,
                  individuo pai2);

