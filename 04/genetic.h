
void crossover (individuo pai1,
                individuo pai2,
                individuo *filho1,
                individuo *filho2,
                int TERRENOS,
                int ESPECIES,
                int ADJ_EDGES,
                int *area_terreno,
                int *demanda,
                int *lucrativity,
                int *productivity,
                int **ter_adjacent);

void gerarConjuntoPais (individuo *populacao,
                        int *indPai1,
                        int *indPai2,
                        int QTDE);

int individuoAleatorio (int n);

void makeNewPopulation (individuo *new_population,
                        individuo *combined_group,
                        int POPULACAO,
                        int ESPECIES);

void mergePopulations (individuo *group_A,
                       individuo *group_B,
                       individuo *combined_group,
                       int POPULACAO,
                       int ESPECIES);

void mutation (individuo *indiv,
               int TERRENOS,
               int ESPECIES,
               int PERIODOS,
               int ADJ_EDGES,
               int *area_terreno,
               int *temp_proc,
               int *familia,
               int *demanda,
               int *lucrativity,
               int *productivity,
               int **ter_adjacent,
               int **per_plantio,
               int PERIODOS_ANO);

individuo torneio (individuo pai1,
                   individuo pai2);
