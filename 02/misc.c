#include "defs.h"

/* ------ */
int compararIndividuos (const void *i,
                        const void *j)
{
    individuo *a,*b;
    a = (individuo *)i;
    b = (individuo *)j;
    if(a->f_obj > b->f_obj)    // decrescente
        return -1;
    else if(a->f_obj < b->f_obj)
        return 1;
    else
        return 0;
}

/* ------ */
int inteiro (int a,
             int b)
{
    double aux;
    int i;
    
    aux = uniforme(0,1);
    
    i = (int)(a + aux*(b - a));
    
    return i;
}
/* ------ */

void ordenarPopulacao (individuo *populacao,
                       int POPULACAO)
{
    qsort(populacao,
          POPULACAO,
          sizeof(individuo),
          compararIndividuos);
}

/* ------ */
double uniforme (double a, double b)
{
    double beta, aux;
    aux = (double)rand()/((double)RAND_MAX + 1);
    beta = a + aux*(b - a);
    return beta;
}

/* ------ */
/* Retorna o maior valor entre dois inteiros, e o primeiro caso iguais */
int intMAX (int A,
            int B)
{
    if (A >= B)
    {
        return A;
    }
    else
    {
        return B;
    }
}

/* ------ */
/* Retorna o menor valor entre dois inteiros, e o primeiro caso iguais */
int intMIN (int A,
            int B)
{
    if (A <= B)
    {
        return A;
    }
    else
    {
        return B;
    }
}
