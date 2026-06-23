#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

// Função auxiliar recursiva que percorre a B-Tree e escreve cada denúncia no ficheiro. 

void GuardarDenunciasRec(NoBTree *node, FILE *f) {
    if (node == NULL) return;

    for (int i = 0; i < node->n_chaves; i++) {
        fprintf(f, "%d;%s;%s;%d;%s;%s\n",
            node->dados[i].id,
            node->dados[i].descricao,
            node->dados[i].zona,
            node->dados[i].gravidade,
            node->dados[i].data,
            node->dados[i].estado
        );
    }

    if (!node->folha) {
        for (int i = 0; i <= node->n_chaves; i++) {
            GuardarDenunciasRec(node->filhos[i], f);
        }
    }
}
// Função principal que cria/abre o ficheiro e guarda todas as denúncias.

void GuardarDenuncias(BTree *bt, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (f == NULL) return;

    GuardarDenunciasRec(bt->raiz, f);

    fclose(f);
}

// Função para guardar todas as zonas num ficheiro.

void CarregarDenuncias(BTree *bt, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL) return;

    Denuncia d;

    while (fscanf(f, "%d;%255[^;];%63[^;];%d;%10[^;];%19[^\n]\n",
        &d.id,
        d.descricao,
        d.zona,
        &d.gravidade,
        d.data,
        d.estado) == 6) {

        InserirNaBTree(bt, d);
    }

    fclose(f);
}
// Função guardar todos os utilizadores num ficheiro. 

void GuardarUtilizadores(NoUtilizador *lista, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (f == NULL) return;

    NoUtilizador *atual = lista;

    while (atual != NULL) {
        fprintf(f, "%d;%s;%s;%s\n",
            atual->dados.id,
            atual->dados.nome,
            atual->dados.perfil,
            atual->dados.contacto
        );

        atual = atual->prox;
    }

    fclose(f);
}

// Função para Carregar os utilizadores de um ficheiro.

NoUtilizador *CarregarUtilizadores(const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL) return NULL;

    NoUtilizador *lista = NULL;
    Utilizador u;

    while (fscanf(f, "%d;%99[^;];%19[^;];%49[^\n]\n",
        &u.id,
        u.nome,
        u.perfil,
        u.contacto) == 4) {

        lista = AdicionarUtilizador(lista, u);
    }

    fclose(f);
    return lista;
}

// Função para guardar todas as zonas no ficheiro

void GuardarZonas(Grafo *g, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (f == NULL) return;

    for (int i = 0; i < g->n_zonas; i++) {
        fprintf(f, "%d;%s;%d;%d\n",
            g->zonas[i].id,
            g->zonas[i].nome,
            g->zonas[i].n_casos,
            g->zonas[i].nivel_risco
        );
    }

    fclose(f);
}

// Função para carregar todas as zonas no ficheiro

void CarregarZonas(Grafo *g, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL) return;

    Zona z;

    while (fscanf(f, "%d;%99[^;];%d;%d\n",
        &z.id,
        z.nome,
        &z.n_casos,
        &z.nivel_risco) == 4) {

        g->zonas[g->n_zonas++] = z;
    }

    fclose(f);
}

// Função para guardar as conexões no ficheiro

void GuardarConexoes(Grafo *g, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (f == NULL) return;

    for (int i = 0; i < g->n_zonas; i++) {
        Aresta *a = g->adjacentes[i];

        while (a != NULL) {
            fprintf(f, "%d;%d;%d\n",
                i,
                a->id_destino,
                a->peso
            );

            a = a->prox;
        }
    }

    fclose(f);
}

// Função para carregar todas as conexões no ficheiro

void CarregarConexoes(Grafo *g, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL) return;

    int origem, destino, peso;

    while (fscanf(f, "%d;%d;%d\n",
        &origem,
        &destino,
        &peso) == 3) {

        ConectarZonas(g, origem, destino, peso);
    }

    fclose(f);
}

// Funcão para guardar o historico no ficheiro

void GuardarHistorico(Historico *lista, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (f == NULL) return;

    Historico *atual = lista;

    while (atual != NULL) {
        fprintf(f, "%d;%s;%s;%s\n",
            atual->id_denuncia,
            atual->estado_anterior,
            atual->estado_atual,
            atual->data
        );

        atual = atual->prox;
    }

    fclose(f);
}

// Função para carregar o historico no ficheiro

void CarregarHistorico(Historico **lista, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (f == NULL) return;

    int id;
    char anterior[20], atual[20], data[30];

    while (fscanf(f, "%d;%19[^;];%19[^;];%29[^\n]\n",
        &id,
        anterior,
        atual,
        data) == 4) {

        AdicionarHistorico(lista, id, anterior, atual, data);
    }

    fclose(f);
}
