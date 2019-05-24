typedef struct especies_disponiveis {
    int idx;
    double prob;
    struct especies_disponiveis* next;
} especies;

crop* addCrop(crop *head,
              int data,
              int inicio,
              int processamento);

especies* addEspecie (especies* head,
                      int data);

void addNewPlantToPlot (int ESPECIES,
                        int PERIODOS,
                        int *area_terreno,
                        int *temp_proc,
                        int *familia,
                        int *demanda,
                        int *productivity,
                        int **per_plantio,
                        int PERIODOS_ANO,
                        int *demanda_atendida,
                        plot *selected_plot);

plot* addPlot (plot *head,
               int idx,
               int free_time,
               plot *next_plot,
               crop *crops);

int countEspecie (especies* head);

crop* createCrop(int data,
                 int processamento,
                 int inicio,
                 crop *next_crop);

especies* createEspecie (int data,
                         especies* next);

plot* createNewPlot (int idx,
                     int free_time,
                     plot *next_plot,
                     crop *crops);

especies* listEspecies (int ESPECIES,
                        int PERIODOS,
                        int periodo,
                        int *temp_proc,
                        int *familia,
                        int *demanda,
                        int **per_plantio,
                        int last_esp,
                        int PERIODOS_ANO,
                        int *demanda_atendida);
