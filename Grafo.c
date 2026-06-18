#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

Grafo *CriarGafo(void)
{
    int i;

    Grafo *g = (Grafo*)malloc(sizeof(Grafo));

    if(g == NULL)
        return NULL;

    g->n_zonas = 0;

    for(i = 0; i < 50; i++)
        g->adjacentes[i] = NULL;

    return g;
}
