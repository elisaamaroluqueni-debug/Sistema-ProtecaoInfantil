#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sistema.h"

/* =========================================================
   DENUNCIAS
   Formato novo:
   id;descricao;zona;gravidade;data;estado;tipo_violencia;nome_denunciante;telefone;morada
   ========================================================= */
static void GuardarDenunciasRec(NoBTree *no, FILE *f) {
    int i;
    if (no == NULL) return;
    for (i = 0; i < no->n_chaves; i++) {
        fprintf(f, "%d;%s;%s;%d;%s;%s;%s;%s;%s;%s\n",
            no->dados[i].id,
            no->dados[i].descricao,
            no->dados[i].zona,
            no->dados[i].gravidade,
            no->dados[i].data,
            no->dados[i].estado,
            no->dados[i].tipo_violencia,
            no->dados[i].nome_denunciante,
            no->dados[i].telefone,
            no->dados[i].morada);
    }
    if (!no->folha)
        for (i = 0; i <= no->n_chaves; i++)
            GuardarDenunciasRec(no->filhos[i], f);
}
void GuardarDenuncias(BTree *bt, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (!f) return;
    GuardarDenunciasRec(bt->raiz, f);
    fclose(f);
}
void CarregarDenuncias(BTree *bt, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f) return;
    Denuncia d;
    char linha[900];
    while (fgets(linha, sizeof(linha), f) != NULL) {
        memset(&d, 0, sizeof(d));

        if (sscanf(linha, "%d;%255[^;];%63[^;];%d;%10[^;];%19[^;];%49[^;];%99[^;];%9[^;];%149[^\n]",
                   &d.id, d.descricao, d.zona, &d.gravidade,
                   d.data, d.estado, d.tipo_violencia,
                   d.nome_denunciante, d.telefone, d.morada) == 10) {
            InserirNaBTree(bt, d);
        } else if (sscanf(linha, "%d;%255[^;];%63[^;];%d;%10[^;];%19[^;];%99[^;];%9[^;];%149[^\n]",
                          &d.id, d.descricao, d.zona, &d.gravidade,
                          d.data, d.estado,
                          d.nome_denunciante, d.telefone, d.morada) == 9) {
            strcpy(d.tipo_violencia, "Nao informado");
            InserirNaBTree(bt, d);
        }
    }
    fclose(f);
}

/* =========================================================
   UTILIZADORES
   Formato: id;nome;perfil;telefone;morada;senha
   ========================================================= */
void GuardarUtilizadores(NoUtilizador *lista, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (!f) return;
    while (lista != NULL) {
        fprintf(f, "%d;%s;%s;%s;%s;%s\n",
            lista->dados.id,
            lista->dados.nome,
            lista->dados.perfil,
            lista->dados.telefone,
            lista->dados.morada,
            lista->dados.senha);
        lista = lista->prox;
    }
    fclose(f);
}
NoUtilizador *CarregarUtilizadores(const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f) return NULL;
    NoUtilizador *lista = NULL;
    Utilizador u;
    char linha[500];
    while (fgets(linha, sizeof(linha), f) != NULL) {
        memset(&u, 0, sizeof(u));
        if (sscanf(linha, "%d;%99[^;];%19[^;];%9[^;];%149[^;];%49[^\n]",
                   &u.id, u.nome, u.perfil,
                   u.telefone, u.morada, u.senha) < 5)
            continue;
        lista = AdicionarUtilizador(lista, u);
    }
    fclose(f);
    return lista;
}

/* =========================================================
   ZONAS
   ========================================================= */
void GuardarZonas(Grafo *g, const char *nome_ficheiro) {
    int i;
    FILE *f = fopen(nome_ficheiro, "w");
    if (!f) return;
    for (i = 0; i < g->n_zonas; i++)
        fprintf(f, "%d;%s;%d;%d\n",
            g->zonas[i].id, g->zonas[i].nome,
            g->zonas[i].n_casos, g->zonas[i].nivel_risco);
    fclose(f);
}
void CarregarZonas(Grafo *g, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f) return;
    Zona z;
    while (fscanf(f, "%d;%99[^;];%d;%d\n",
                  &z.id, z.nome, &z.n_casos, &z.nivel_risco) == 4)
        g->zonas[g->n_zonas++] = z;
    fclose(f);
}

/* =========================================================
   CONEXÕES
   ========================================================= */
void GuardarConexoes(Grafo *g, const char *nome_ficheiro) {
    int i;
    FILE *f = fopen(nome_ficheiro, "w");
    if (!f) return;
    for (i = 0; i < g->n_zonas; i++) {
        Aresta *a = g->adjacentes[i];
        while (a != NULL) {
            if (i < a->id_destino)
                fprintf(f, "%d;%d;%d\n", i, a->id_destino, a->peso);
            a = a->prox;
        }
    }
    fclose(f);
}
void CarregarConexoes(Grafo *g, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f) return;
    int origem, destino, peso;
    while (fscanf(f, "%d;%d;%d\n", &origem, &destino, &peso) == 3) {
        int existe = 0;
        Aresta *a;
        if (origem < 0 || destino < 0 || origem >= g->n_zonas || destino >= g->n_zonas)
            continue;
        a = g->adjacentes[origem];
        while (a != NULL) {
            if (a->id_destino == destino) { existe = 1; break; }
            a = a->prox;
        }
        if (!existe)
            ConectarZonas(g, origem, destino, peso);
    }
    fclose(f);
}

/* =========================================================
   HISTORICO
   ========================================================= */
void GuardarHistorico(Historico *lista, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "w");
    if (!f) return;
    while (lista != NULL) {
        fprintf(f, "%d;%s;%s;%s\n",
            lista->id_denuncia, lista->estado_anterior,
            lista->estado_atual, lista->data);
        lista = lista->prox;
    }
    fclose(f);
}
void CarregarHistorico(Historico **lista, const char *nome_ficheiro) {
    FILE *f = fopen(nome_ficheiro, "r");
    if (!f) return;
    int id;
    char anterior[20], atual[20], data[30];
    while (fscanf(f, "%d;%19[^;];%19[^;];%29[^\n]\n",
                  &id, anterior, atual, data) == 4)
        AdicionarHistorico(lista, id, anterior, atual, data);
    fclose(f);
}
