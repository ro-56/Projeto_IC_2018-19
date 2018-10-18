#include "defs.h"
#include "individuo.h"

#define PENALIDADE_DEMANDA 30

individuo criarIndividuo(int PERIODOS,
                         int ESPECIES,
                         int TERRENOS,
                         int *area_terreno,
                         int *temp_proc,
                         int *demanda,
                         int *lucrativity,
                         int *productivity,
                         int **per_plantio,
                         int PERIODOS_ANO)
{
    individuo novo_ind;
    int i,j,f_obj = 0;

    especies *head = NULL, *cursor = NULL;
    
    /*  Inicializa a matriz solucao de cada individuo  */
    int **matrix = (int **)calloc(TERRENOS, sizeof(int *));
    for (i=0;i<TERRENOS;i++)
        matrix[i] = (int *)calloc(PERIODOS, sizeof(int));
    
    /*  Inicializa o vetor demanda atendida de cada individuo  */
    int *demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));

    
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
                                PERIODOS_ANO,
                                demanda_atendida);

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
                plant_count = temp_proc[esp_selecionada] - 1;
                
                demanda_atendida[esp_selecionada] = area_terreno[i] * productivity[esp_selecionada];
                    //TODO: Mudar modo de calcular fObj. <Perda no lucro por nao atender denanda>
                f_obj += lucrativity[esp_selecionada] * area_terreno[i] * productivity[esp_selecionada];
            }
            
            while (plant_count != 0)
            {
                j++;
                matrix[i][j] = esp_selecionada + 1;
                plant_count--;
            }
            
            last_esp = matrix[i][j] - 1;

            esp_selecionada = -1;
            plant_count = 0;
            freeMemory(head);
            head = NULL;
        }
        last_esp = -1;
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
    novo_ind.demanda_atendida = demanda_atendida;
    novo_ind.f_obj = CalcularFuncaoObj (PERIODOS,
                                        TERRENOS,
                                        area_terreno,
                                        demanda,
                                        lucrativity,
                                        productivity,
                                        matrix,
                                        demanda_atendida);
    
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
int CalcularFuncaoObj (int PERIODOS,
                       int TERRENOS,
                       int *area_terreno,
                       int *demanda,
                       int *lucrativity,
                       int *productivity,
                       int **solucao,
                       int *demanda_atendida)
{
    int f_obj = 0, last_esp = -1;
    int i,j;
    
    for (i = 0; i < TERRENOS; i++)
    {
        for (j = 0; j < PERIODOS; j++)
        {
            /*  */
            if (solucao[i][j] - 1 == last_esp)
            {
                continue;
            }
            last_esp = solucao[i][j] - 1;
            
            f_obj += area_terreno[i] * productivity[last_esp] * lucrativity[last_esp];

            /* Se a damanda dessa especie nao foi atendida */
            if (demanda[last_esp] - demanda_atendida[last_esp] > 0)
            {
                f_obj -= PENALIDADE_DEMANDA;
            }
        }
        last_esp = -1;
    }
    
    return f_obj;
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
               int *lucrativity,
               int **per_plantio)
{
        //TODO: Arrumar demanda atendida.
        // <Pegar demanda atendida anterior, recalcular ou adicionar nova demanda>
    int esp_selecionada = 0, plant_count = 0, last_esp = -1;
    int j,f_obj = 0;
    int *demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
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
                            PERIODOS_ANO,
                            demanda_atendida);
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
            
            f_obj += lucrativity[esp_selecionada];
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
                        int PERIODOS_ANO,
                        int *demanda_atendida)
{
    int i, sum = 0;
    double cumulative = 0;
    especies *aux = NULL, *aux_prob = NULL;
    
    for (i = 0; i < ESPECIES; i++)
    {
        /* Se a sazonalidade esta contida em um ano */
        if (per_plantio[i][0] < per_plantio[i][1])
        {
            if ((periodo % PERIODOS_ANO) >= (per_plantio[i][0] - 1) &&
                ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] - 1) &&
                i != last_esp)
            {
                aux = add(aux,i);
            }
        }
        else
        {
            /* Se esta no ultimo ano */
            if (((float)periodo / (float)PERIODOS) > (1 - ((float)PERIODOS_ANO / (float)PERIODOS)))
            {
                if ((periodo % PERIODOS_ANO) >= (per_plantio[i][0] - 1) &&
                    ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (PERIODOS_ANO - 1) &&
                    i != last_esp)
                {
                    aux = add(aux,i);
                }
            }
            else
            {
                if ((periodo % PERIODOS_ANO) >= 0 &&
                   ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] - 1) &&
                    i != last_esp)
                {
                    aux = add(aux,i);
                }
                else if (((periodo % PERIODOS_ANO) >= 0 &&
                          ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] + PERIODOS_ANO - 1) &&
                          i != last_esp))
                {
                    aux = add(aux,i);
                }
                
//                if (periodo <= per_plantio[i][1] - 1)
//                {
//                    if ((periodo % PERIODOS_ANO) >= 0 &&a
//                       ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] - 1) &&
//                        i != last_esp)
//                    {
//                        aux = add(aux,i);
//                    }
//                }
//                else
//                {
//                    if ((periodo % PERIODOS_ANO) >= 0 &&
//                        ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] + PERIODOS_ANO - 1) &&
//                        i != last_esp)
//                    {
//                        aux = add(aux,i);
//                    }
//                }
            }
        }
    }
    
    /* Se existir alguma especie na lista, montar probabilidade
     de escolha baseada na demanda nao atendida */
    if (aux != NULL)
    {
        aux_prob = aux;
        while (aux_prob != NULL)
        {
            sum += demanda[aux_prob->idx] - demanda_atendida[aux_prob->idx];
            aux_prob = aux_prob->next;
        }
        int total = sum + count(aux);
        
        aux_prob = aux;
        while (aux_prob != NULL)
        {
            cumulative += ((double)(demanda[aux_prob->idx] - demanda_atendida[aux_prob->idx]) + 1) / ((double)total);
            aux_prob->prob = cumulative;
            aux_prob = aux_prob->next;
        }
    }
    
//    aux_prob = aux;
//    while (aux_prob != NULL)
//    {
//        aux_prob = aux_prob->next;
//    }
    
    free(aux_prob);

    return aux;
}
