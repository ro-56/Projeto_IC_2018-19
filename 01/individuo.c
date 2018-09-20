#include "defs.h"
#include "individuo.h"


individuo criarIndividuo(int PERIODOS,
                         int ESPECIES,
                         int TERRENOS,
                         int *temp_proc,
                         int *demanda,
                         int **per_plantio,
                         int PERIODOS_ANO,
                         int *lucratividade_especies)
{
    individuo novo_ind;
    int i,j,f_obj = 0;

    especies *head = NULL, *cursor = NULL;
    
    /*  Inicializa a matriz solucao de cada individuo  */
    int **matrix = (int **)calloc(TERRENOS, sizeof(int *));
    for (i=0;i<TERRENOS;i++)
        matrix[i] = (int *)calloc(PERIODOS, sizeof(int));
    
    /*  Inicializa o vetor demanda de cada individuo  */
    int *vector = (int *)calloc(ESPECIES, sizeof(int));
    for (i=0;i<ESPECIES;i++)
        vector[i] = demanda[i];
    
    /*  Cria a matriz solucao  */
    int esp_selecionada = -1, plant_count = 0, last_esp = -1;
    for (i = 0; i < TERRENOS; i++)
    {
        for (j = 0; j < PERIODOS; j++)
        {
            head = listEspecies(ESPECIES,
                                PERIODOS,
                                j,
                                temp_proc,
                                demanda,
                                per_plantio,
                                last_esp,
                                PERIODOS_ANO);
//            int auxj;
            if (head != NULL)
            {
                cursor = head;
//                printf(".%f.",cursor->prob);
//                int random_especie = rand() % count(head);
////                printf(".%d ", random_especie);
//                for (auxj = 0; auxj < random_especie; auxj++)
//                {
//                    cursor = cursor->next;
//                }
                
                float random_especie = uniforme(0,1);
                if (random_especie > cursor->prob)
                {
                    cursor = cursor->next;
                }
                
                esp_selecionada = cursor->idx;
                
                matrix[i][j] = esp_selecionada + 1;
//                last_esp = esp_selecionada;
                plant_count = temp_proc[esp_selecionada] - 1;
                
                f_obj += lucratividade_especies[esp_selecionada];
            }
//            else
//            {
//                last_esp = -1;
//                printf("NO: %d\n",j);
//            }
            
            while (plant_count != 0)
            {
                j++;
                matrix[i][j] = esp_selecionada + 1;
                plant_count--;
            }
            
            last_esp = matrix[i][j] - 1;
            printf("NO: %d\n",last_esp);

            esp_selecionada = -1;
            plant_count = 0;
            freeMemory(head);
            head = NULL;
        }
    }
    
    /********** PRINT **********/

//    printf("\nINDIVIDUO: \n");
//    for(i=0;i<ESPECIES;i++)
//    {
//        printf("%d ", vector[i]);
//    }
//    for(i=0;i<TERRENOS;i++)
//    {
//        for(j=0;j<PERIODOS;j++)
//        {
//            printf("%d ", matrix[i][j]);
//        }
//        printf("\n");
//    }
//    printf("f_obj: %d\n",f_obj);
//    printf("\n");
    
    /**********       **********/
    
    novo_ind.sol = matrix;
    novo_ind.f_obj = f_obj;
    novo_ind.demanda_n_atendida = vector;
    
    return novo_ind;
}

/*****/
especies* add(especies *head, int data)
{
    especies* new_item = create(data,NULL);
    especies *aux = head;
    
    if (head != NULL)
    {
        while(aux->next != NULL)
            aux = aux->next;
        
        aux->next = new_item;
    }
    else
    {
        head = new_item;
    }
    
    return head;
}
/*****/
int count(especies *head)
{
    especies *aux = head;
    int c = 0;
    while(aux != NULL)
    {
        c++;
        aux = aux->next;
    }
    return c;
}
/*****/
especies* create(int data,especies *next)
{
    especies* new_item = (especies*)malloc(sizeof(especies));
    
    if(new_item == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    
    new_item->idx = data;
    new_item->prob = 0;
    new_item->next = next;
    
    return new_item;
}
/*****/
int createRow (int *row,
               int ESPECIES,
               int PERIODOS,
               int PERIODOS_ANO,
               int *temp_proc,
               int *demanda,
               int **per_plantio,
               int *lucratividade_especies)
{
    int esp_selecionada = 0, plant_count = 0, last_esp = -1;
    int j,f_obj = 0;
    especies *head = NULL, *cursor = NULL;
    
    //Reset row
    for(j = 0; j < PERIODOS; j++)
    {
        row[j] = 0;
    }
    
    //Create new row
    for (j = 0; j < PERIODOS; j++)
    {
        head = listEspecies(ESPECIES,
                            PERIODOS,
                            j,
                            temp_proc,
                            demanda,
                            per_plantio,
                            last_esp,
                            PERIODOS_ANO);
        int auxj;
        if (head != NULL){
            cursor = head;
            int random_especie = rand() % count(head);
            for (auxj = 0; auxj < random_especie; auxj++)
            {
                cursor = cursor->next;
            }
            esp_selecionada = cursor->idx ;
            
            row[j]= esp_selecionada + 1;
            last_esp = esp_selecionada;
            plant_count = temp_proc[esp_selecionada] - 1;
            
            f_obj += lucratividade_especies[esp_selecionada];
        }
        else
        {
            last_esp = -1;
        }
        
        while (plant_count != 0)
        {
            j++;
            row[j] = esp_selecionada + 1;
            plant_count--;
        }
        
        freeMemory(head);
        head = NULL;
    }
    
    return f_obj;
}
/*****/
void freeMemory (especies *head)
{
    especies *aux;
    while(head)
    {
        aux = head;
        head = head->next;
        free(aux);
    }
}
/*****/
especies* listEspecies (int ESPECIES,
                        int PERIODOS,
                        int periodo,
                        int *temp_proc,
                        int *demanda,
                        int **per_plantio,
                        int last_esp,
                        int PERIODOS_ANO)
{
    int i, sum = 0;
    double cumulative = 0;
    especies *aux = NULL, *aux_prob = NULL;
    
    for (i = 0; i < ESPECIES; i++)
    {
        printf("-%d:%f<=%f-",periodo,((float)(periodo + 1) / (float)PERIODOS), (((float)PERIODOS / (float)PERIODOS_ANO) - 1));
        if (per_plantio[i][0] < per_plantio[i][1])
        {
            if ((periodo % PERIODOS_ANO) >= (per_plantio[i][0] - 1) &&
                ((periodo % PERIODOS_ANO) + (temp_proc[i])) <= (per_plantio[i][1] - 1) &&///  add (temp_proc[i]-1)
                i != last_esp)
            {
                aux = add(aux,i);
            }
        }
        else
        {
            printf(".%d.",i);

//            if (((periodo + 1) / PERIODOS) < ((PERIODOS / PERIODOS_ANO) - 1) &&
//                (periodo % PERIODOS_ANO) >= (per_plantio[i][0] - 1) &&
//                ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] + PERIODOS_ANO - 1) &&
//                i != last_esp)
//            {
            
            /* Se esta no ultimo ano */
            if ((periodo / PERIODOS) > (1 - (PERIODOS_ANO / PERIODOS)))
            {//TODO - corrigir error em anos consecutivos
                if ((periodo % PERIODOS_ANO) >= (per_plantio[i][0] - 1) &&
                    ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (PERIODOS_ANO - 1) &&
                    i != last_esp)
                {
                    aux = add(aux,i);
                }
            }
            else
            {
                if ((periodo % PERIODOS_ANO) >= (per_plantio[i][0] - 1) &&
                    ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] + PERIODOS_ANO - 1) &&
                    i != last_esp)
                {
                    aux = add(aux,i);
                }
            }
        }
    }
    
    
    if (aux != NULL)
    {
        aux_prob = aux;
        while (aux_prob != NULL)
        {
            sum += demanda[aux_prob->idx];
            aux_prob = aux_prob->next;
        }
        int total = sum + count(aux);
        
        aux_prob = aux;
        while (aux_prob != NULL)
        {
            cumulative += ((double)demanda[aux_prob->idx] + 1) / ((double)total);
            aux_prob->prob = cumulative;
            aux_prob = aux_prob->next;
        }
    }
    
    printf(":%d:\n",periodo);
    aux_prob = aux;
    while (aux_prob != NULL)
    {
        printf("%d,", aux_prob->idx);
        aux_prob = aux_prob->next;
    }
    printf("\n");
    
    
    free(aux_prob);

    return aux;
}
