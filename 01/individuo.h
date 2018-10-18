typedef struct especies_disponiveis {
    int idx;
    double prob;
    struct especies_disponiveis* next;
} especies;

int count (especies* head);

int search (especies* head,
            int data);

especies* add (especies* head,
               int data);

especies* create (int data,
                  especies* next);

void freeMemory (especies *head);

especies* listEspecies (int ESPECIES,
                        int PERIODOS,
                        int periodo,
                        int *temp_proc,
                        int *demanda,
                        int **per_plantio,
                        int last_esp,
                        int PERIODOS_ANO,
                        int *demanda_atendida);
