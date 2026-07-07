#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Sistema.h"

/* Gera ID aleatório de 4 dígitos único na lista */
int GerarIDUtilizador(NoUtilizador *lista) {
    int id;
    int encontrou;
    NoUtilizador *tmp;
    do {
        id = 1000 + rand() % 9000;
        encontrou = 0;
        tmp = lista;
        while (tmp != NULL) {
            if (tmp->dados.id == id) { encontrou = 1; break; }
            tmp = tmp->prox;
        }
    } while (encontrou);
    return id;
}

NoUtilizador *AdicionarUtilizador(NoUtilizador *lista, Utilizador u) {
    NoUtilizador *novo = (NoUtilizador*)malloc(sizeof(NoUtilizador));
    if (!novo) { printf("Erro: memoria insuficiente.\n"); return lista; }
    novo->dados = u;
    novo->prox  = lista;
    return novo;
}

NoUtilizador *RemoverUtilizador(NoUtilizador *lista, int id) {
    NoUtilizador *atual = lista, *anterior = NULL;
    while (atual != NULL) {
        if (atual->dados.id == id) {
            if (anterior == NULL) lista = atual->prox;
            else anterior->prox = atual->prox;
            free(atual);
            printf("Utilizador %d removido.\n", id);
            return lista;
        }
        anterior = atual; atual = atual->prox;
    }
    printf("Utilizador %d nao encontrado.\n", id);
    return lista;
}

/* Para admins: autenticação por nome + senha (texto) */
NoUtilizador *PesquisarUtilizadorPorNomeSenha(NoUtilizador *lista,
                                               const char *nome,
                                               const char *senha) {
    NoUtilizador *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->dados.perfil, "admin") == 0 &&
            strcmp(atual->dados.nome,   nome)    == 0 &&
            strcmp(atual->dados.senha,  senha)   == 0)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

/* Para utilizadores: autenticação por nome + ID numérico (a "senha" deles) */
NoUtilizador *PesquisarUtilizadorPorNomeID(NoUtilizador *lista,
                                            const char *nome, int id) {
    NoUtilizador *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->dados.perfil, "utilizador") == 0 &&
            strcmp(atual->dados.nome,   nome)         == 0 &&
            atual->dados.id == id)
            return atual;
        atual = atual->prox;
    }
    return NULL;
}

void ImprimirUtilizador(NoUtilizador *lista) {
    if (lista == NULL) { printf("Nao existem utilizadores.\n"); return; }
    printf("\n=== LISTA DE UTILIZADORES ===\n");
    while (lista != NULL) {
        printf("  ID     : %d\n",   lista->dados.id);
        printf("  Nome   : %s\n",   lista->dados.nome);
        printf("  Perfil : %s\n",   lista->dados.perfil);
        if (strcmp(lista->dados.perfil, "utilizador") == 0) {
            printf("  Tel    : %s\n",   lista->dados.telefone);
            printf("  Morada : %s\n",   lista->dados.morada);
        }
        printf("\n");
        lista = lista->prox;
    }
}
