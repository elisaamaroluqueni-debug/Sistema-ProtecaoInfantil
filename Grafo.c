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
 // funcão Adiciona uma nova zona ao grafo e inicializa os seus dados básicos
int AdicionarZona(Grafo *g, const char *nome)
{
    if(g == NULL || g->n_zonas >= 50)
        return -1;

    int id = g->n_zonas;

    g->zonas[id].id = id;
    strcpy(g->zonas[id].nome, nome);
    g->zonas[id].n_casos = 0;
    g->zonas[id].nivel_risco = 0;

    g->adjacentes[id] = NULL;

    g->n_zonas++;

    return id;
}
// Função conectar Zonas, cria uma ligação (aresta).
void ConectarZonas(Grafo *g, int id_a, int id_b, int peso)
{
    if(g == NULL)
        return;

    if(id_a >= g->n_zonas || id_b >= g->n_zonas)
        return;

    Aresta *nova = (Aresta*)malloc(sizeof(Aresta));
    nova->id_destino = id_b;
    nova->peso = peso;
    nova->prox = g->adjacentes[id_a];
    g->adjacentes[id_a] = nova;

    Aresta *nova2 = (Aresta*)malloc(sizeof(Aresta));
    nova2->id_destino = id_a;
    nova2->peso = peso;
    nova2->prox = g->adjacentes[id_b];
    g->adjacentes[id_b] = nova2;
}
// Percorre o grafo e imprime as zonas com nível de risco elevado
void ZonasCriticas(Grafo *g)
{
    if(g == NULL)
        return;

    printf("Zonas Criticas:\n");

    for(int i = 0; i < g->n_zonas; i++)
    {
        if(g->zonas[i].nivel_risco >= 7)
        {
            printf("ID: %d\n", g->zonas[i].id);
            printf("Nome: %s\n", g->zonas[i].nome);
            printf("Casos: %d\n", g->zonas[i].n_casos);
            printf("Risco: %d\n\n", g->zonas[i].nivel_risco);
        }
    }
}
