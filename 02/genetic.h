void crossover (individuo pai1,
                individuo pai2,
                individuo *filho1,
                individuo *filho2,
                int TERRENOS,
                int ESPECIES,
                int *area_terreno,
                int *demanda,
                int *lucrativity,
                int *productivity);

void gerarConjuntoPais (individuo *populacao,
                        int *indPai1,
                        int *indPai2,
                        int QTDE);

int individuoAleatorio (int n);

void mutation (individuo *indiv,
               int ESPECIES,
               int PERIODOS,
               int *area_terreno,
               int *temp_proc,
               int *familia,
               int *demanda,
               int *productivity,
               int **per_plantio,
               int PERIODOS_ANO);

individuo torneio (individuo pai1,
                   individuo pai2);
