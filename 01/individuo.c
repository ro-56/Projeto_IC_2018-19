#include "defs.h"
#include "individuo.h"

#define PENALIDADE_DEMANDA 100

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
    int i, f_obj = 0;
    
    /*  Inicializa a matriz solucao de cada individuo  */
    int **matrix = (int **)calloc(TERRENOS, sizeof(int *));
    for (i=0;i<TERRENOS;i++)
        matrix[i] = (int *)calloc(PERIODOS, sizeof(int));
    
    /*  Inicializa o vetor demanda atendida de cada individuo  */
    int *demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
    
    for (i = 0; i < TERRENOS; i++)
    {
        
        f_obj = createRow (matrix,
                           demanda_atendida,
                           ESPECIES,
                           PERIODOS,
                           TERRENOS,
                           PERIODOS_ANO,
                           temp_proc,
                           area_terreno,
                           demanda,
                           lucrativity,
                           productivity,
                           per_plantio,
                           i);
    }

    /**********       **********/
    
    novo_ind.sol = matrix;
    novo_ind.demanda_atendida = demanda_atendida;
    novo_ind.f_obj = f_obj;

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
            if (solucao[i][j] == 0 || solucao[i][j] - 1 == last_esp)
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
int createRow (int **matrix,
               int *demanda_atendida,
               int ESPECIES,
               int PERIODOS,
               int TERRENOS,
               int PERIODOS_ANO,
               int *temp_proc,
               int *area_terreno,
               int *demanda,
               int *lucrativity,
               int *productivity,
               int **per_plantio,
               int idx_terreno_novo)
{
    int esp_selecionada = -1, plant_count = 0, last_esp = -1;
    int j,f_obj = 0;
    especies *head = NULL, *cursor = NULL;
    
    //Reset row
    for(j = 0; j < PERIODOS; j++)
    {
        matrix[idx_terreno_novo][j] = 0;
    }
    
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
            
            float random_especie = uniforme(0,1);
            while (random_especie > cursor->prob && cursor->next != NULL)
            {				
                cursor = cursor->next;
            }
            
            esp_selecionada = cursor->idx;
            
            matrix[idx_terreno_novo][j] = esp_selecionada + 1;
            plant_count = temp_proc[esp_selecionada] - 1;
            
            demanda_atendida[esp_selecionada] += area_terreno[idx_terreno_novo] * productivity[esp_selecionada];
        }
        
        while (plant_count != 0)
        {
            j++;
            matrix[idx_terreno_novo][j] = esp_selecionada + 1;
            plant_count--;
        }
        
        last_esp = matrix[idx_terreno_novo][j] - 1;
        
        esp_selecionada = -1;
        plant_count = 0;
        freeMemory(head);
        head = NULL;
    }
    
    f_obj = CalcularFuncaoObj (PERIODOS,
                               TERRENOS,
                               area_terreno,
                               demanda,
                               lucrativity,
                               productivity,
                               matrix,
                               demanda_atendida);
    
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
    double cumulative1 = 0, cumulative2 = 0;
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
                
            }
        }
    }
    aux_prob = aux;
    while (aux_prob != NULL)
    {
        aux_prob = aux_prob->next;
    }
    
    /* Se existir alguma especie na lista, montar probabilidade
     de escolha baseada na demanda nao atendida */
    if (aux != NULL)
    {
        aux_prob = aux;
        while (aux_prob != NULL)
        {
            if ( (demanda[aux_prob->idx] - demanda_atendida[aux_prob->idx]) > 0)
            {
                sum += demanda[aux_prob->idx] - demanda_atendida[aux_prob->idx];
            }
            
            aux_prob = aux_prob->next;
        }
        int total = sum + count(aux);
        
        aux_prob = aux;
        while (aux_prob != NULL)
        {
			if ( (demanda[aux_prob->idx] - demanda_atendida[aux_prob->idx]) > 0)
            {
				cumulative1 += demanda[aux_prob->idx] - demanda_atendida[aux_prob->idx]; 
            }
            cumulative2 = (double)(cumulative1 + 1)/((double)total);
            aux_prob->prob = cumulative2;
            aux_prob = aux_prob->next;
        }
    }
    
    
    free(aux_prob);

    return aux;
}
