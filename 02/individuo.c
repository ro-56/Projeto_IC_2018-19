#include "defs.h"
#include "individuo.h"

#define DEMAND_PENALTY_PER_UNIT 2
#define DEMAND_PENALTY 50


individuo criarIndividuo(int PERIODOS,
                         int ESPECIES,
                         int TERRENOS,
                         int *area_terreno,
                         int *temp_proc,
                         int *familia,
                         int *demanda,
                         int *lucrativity,
                         int *productivity,
                         int **per_plantio,
                         int PERIODOS_ANO)
{
    
    individuo novo_ind;
    int i, f_obj = 0;

    /*  Inicializa o vetor demanda atendida de cada individuo  */
    int *demanda_atendida = (int *)calloc(ESPECIES, sizeof(int));
    
    /*  Inicializa a lista de terrenos  */
    plot *solucao = NULL;
    plot *aux = solucao;
    for (i = 0; i < TERRENOS; i++)
    {
        plot* new_item = createNewPlot (i,
                                        0,
                                        NULL,
                                        NULL);
        
        if (solucao != NULL)
        {
            aux->next_plot = new_item;
        }
        else
        {
            solucao = new_item;
        }
        
        aux = new_item;
    }
    
    /*  Preenche os terrenos com plantas  */
    int can_accept_new_plants = 1, min_free_time = 1000, plot_idx_min_free_time;
    plot *selected_plot = NULL, *plot_walker = NULL;
    
    while (can_accept_new_plants)
    {
        /*  Achar Terreno com menor tempo livre  */
        plot_walker = solucao;
        selected_plot = NULL;
        min_free_time = 1000;
        while (plot_walker != NULL)
        {
            if (plot_walker->free_time == 0)
            {
                selected_plot = plot_walker;
                break;
            }

            if (plot_walker->free_time < min_free_time)
            {
                plot_idx_min_free_time = plot_walker->idx;
                min_free_time = plot_walker->free_time;
            }

            plot_walker = plot_walker->next_plot;
        }
        if (!selected_plot)
        {
            plot_walker = solucao;
            while (plot_walker->idx != plot_idx_min_free_time)
            {
                plot_walker = plot_walker->next_plot;
            }
            selected_plot = plot_walker;
        }

        /*  Adicionar uma planta ao terreno selecionado  */
        addNewPlantToPlot (ESPECIES,
                           PERIODOS,
                           area_terreno,
                           temp_proc,
                           familia,
                           demanda,
                           productivity,
                           per_plantio,
                           PERIODOS_ANO,
                           demanda_atendida,
                           selected_plot);
        
        /*  Checar se existe algum terreno que ainda aceita plantas  */
        plot_walker = solucao;
        while (plot_walker != NULL)
        {
            if (plot_walker->free_time < PERIODOS)
            {
                break;
            }

            if (plot_walker->next_plot == NULL)
            {
                can_accept_new_plants = 0;
            }

            plot_walker = plot_walker->next_plot;
        }
    }
    
    /*  Calcular Funcao Objetivo (lucro) do individuo  */
    f_obj = calculateFObj (ESPECIES,
                           demanda,
                           lucrativity,
                           demanda_atendida);
    
    
    
    novo_ind.solucao = solucao;
    novo_ind.demanda_atendida = demanda_atendida;
    novo_ind.f_obj = f_obj;
    
    return novo_ind;
}

/* ------ */

crop* addCrop(crop *head,
              int data,
              int inicio,
              int processamento)
{
    crop* new_item = createCrop(data,
                                processamento,
                                inicio,
                                NULL);
    crop *aux = head;
    
    
    if (head != NULL)
    {
        while(aux->next_crop != NULL)
            aux = aux->next_crop;
        
        aux->next_crop = new_item;
    }
    else
    {
        head = new_item;
    }
    
    return head;
}

/* ------ */
especies* addEspecie(especies *head, int data)
{
    especies* new_item = createEspecie(data,NULL);
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

/* ------ */
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
                        plot *selected_plot)
{
    especies *list_possible_species = NULL, *list_cursor = NULL;;
    
    /*  Pegar a ultima especie plantada no terreno selecionado  */
    crop *crop_walker = selected_plot->crops;
    int last_esp = -1;
    
    if (crop_walker)
    {
        while (crop_walker)
        {
            last_esp = crop_walker->idx;
            crop_walker = crop_walker->next_crop;
        }
    }
    
    /*  Listar todas as possiveis especies que podem ser plantados  */
    list_possible_species = listEspecies (ESPECIES,
                                          PERIODOS,
                                          selected_plot->free_time,
                                          temp_proc,
                                          familia,
                                          demanda,
                                          per_plantio,
                                          last_esp,
                                          PERIODOS_ANO,
                                          demanda_atendida);
    
    int esp_selecionada = -1;
    
    if (list_possible_species != NULL)
    {
        list_cursor = list_possible_species;
        
        float random_especie = uniforme(0,1);
        while (random_especie > list_cursor->prob && list_cursor->next != NULL)
        {
            list_cursor = list_cursor->next;
        }
        
        esp_selecionada = list_cursor->idx;
        
        selected_plot->crops = addCrop(selected_plot->crops,
                                       esp_selecionada,
                                       selected_plot->free_time,
                                       temp_proc[esp_selecionada]);
        
        demanda_atendida[esp_selecionada] += area_terreno[selected_plot->idx] * productivity[esp_selecionada];
        
        selected_plot->free_time += temp_proc[esp_selecionada];
    }
    else
    {
        selected_plot->crops = addCrop(selected_plot->crops,
                                       -1,
                                       selected_plot->free_time,
                                       1);
        
        selected_plot->free_time += 1;
    }
    
    return;
}

/* ------ */
plot* addPlot(plot *head,
              int idx,
              int free_time,
              plot *next_plot,
              crop *crops)
{
    plot* new_item = createNewPlot (idx,
                                    free_time,
                                    next_plot,
                                    crops);
    plot *aux = head;
    
    if (head != NULL)
    {
        while(aux->next_plot != NULL)
            aux = aux->next_plot;
        
        aux->next_plot = new_item;
    }
    else
    {
        head = new_item;
    }
    
    return head;
}

/* ------ */
int calculateFObj (int ESPECIES,
                   int *demanda,
                   int *lucrativity,
                   int *demanda_atendida)
{
    int i, result = 0;
    
    for (i = 0; i < ESPECIES; i++)
    {
        if (demanda_atendida[i] - demanda[i] >= 0)
        {
            /*  Calcular a lucratividade da solução  */
            result +=  demanda_atendida[i] * lucrativity[i];
        }
        else
        {
            /*  Retirar penalidade unitaria por nao atender a demanda  */
            result -= DEMAND_PENALTY_PER_UNIT * (demanda[i] - demanda_atendida[i]);
            
            /*  Retirar penalidade geral por nao atender a demanda  */
            result -= DEMAND_PENALTY;
        }
    }
    
    return result;
}

/* ------ */
int countEspecie(especies *head)
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

/* ------ */
crop* createCrop(int data,
                 int processamento,
                 int inicio,
                 crop *next_crop)
{
    crop* new_item = (crop*)malloc(sizeof(crop));
    
    if(new_item == NULL)
    {
        printf("Error creating a new node. [createCrop]\n");
        exit(0);
    }
    
    new_item->idx = data;
    new_item->inicio = inicio;
    new_item->next_crop = next_crop;
    new_item->free_time = inicio + processamento;

        
    
    return new_item;
}

/* ------ */
especies* createEspecie(int data,especies *next)
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

/* ------ */
plot* createNewPlot (int idx,
                     int free_time,
                     plot *next_plot,
                     crop *crops)
{
    plot *new_item = (plot*)malloc(sizeof(plot));
    
    if(new_item == NULL)
    {
        printf("Error creating a new node. [createNewPlot]\n");
        exit(0);
    }
    
    new_item->idx = idx;
    new_item->free_time = free_time;
    new_item->next_plot = next_plot;
    new_item->crops = crops;
    
    return new_item;
}

/* ------ */
especies* listEspecies (int ESPECIES,
                        int PERIODOS,
                        int periodo,
                        int *temp_proc,
                        int *familia,
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
                familia[i] != familia[last_esp])
            {
                aux = addEspecie(aux,i);
            }
        }
        else
        {
            /* Se esta no ultimo ano */
            if (((float)periodo / (float)PERIODOS) > (1 - ((float)PERIODOS_ANO / (float)PERIODOS)))
            {
                if ((periodo % PERIODOS_ANO) >= (per_plantio[i][0] - 1) &&
                    ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (PERIODOS_ANO - 1) &&
                    familia[i] != familia[last_esp])
                {
                    aux = addEspecie(aux,i);
                }
            }
            else
            {
                if ((periodo % PERIODOS_ANO) >= 0 &&
                    ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] - 1) &&
                    familia[i] != familia[last_esp])
                {
                    aux = addEspecie(aux,i);
                }
                else if (((periodo % PERIODOS_ANO) >= 0 &&
                          ((periodo % PERIODOS_ANO) + (temp_proc[i] - 1)) <= (per_plantio[i][1] + PERIODOS_ANO - 1) &&
                          familia[i] != familia[last_esp]))
                {
                    aux = addEspecie(aux,i);
                }
                
            }
        }
    }
    
        // TODO: Necessario??? <TESTAR E RETIRAR>
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
        int total = sum + countEspecie(aux);
        
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


/*  ANTIGO  */
    //int calculateFObj (plot *solucao,
    //                   int ESPECIES,
    //                   int *area_terreno,
    //                   int *demanda,
    //                   int *lucrativity,
    //                   int *productivity,
    //                   int *demanda_atendida)
    //{
    //    plot *p_walker = solucao;
    //    crop *c_walker = NULL;
    //    int i, result = 0;
    //
    //    /*  Calcular a lucratividade da solução  */
    //    while (p_walker)
    //    {
    //        c_walker = p_walker->crops;
    //        while (c_walker)
    //        {
    //            result += area_terreno[p_walker->idx] * productivity[c_walker->idx] * lucrativity[c_walker->idx];
    //            c_walker = c_walker->next_crop;
    //        }
    //        p_walker = p_walker->next_plot;
    //    }
    //
    //    /*  Retirar penalidade unitaria por nao atender a demanda  */
    //    for (i = 0; i < ESPECIES; i++)
    //    {
    //        if (demanda_atendida[i] - demanda[i] < 0)
    //        {
    //            result -= DEMAND_PENALTY_PER_UNIT * (demanda[i] - demanda_atendida[i]);
    //        }
    //    }
    //
    //
    //    /*  Retirar penalidade geral por nao atender a demanda  */
    //    for (i = 0; i < ESPECIES; i++)
    //    {
    //        if (demanda_atendida[i] - demanda[i] < 0)
    //        {
    //            result -= DEMAND_PENALTY;
    //        }
    //    }
    //
    //    return result;
    //}
