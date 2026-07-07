#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sistema.h"

void AdicionarHistorico(Historico **lista, int id_denuncia,
                        const char *anterior,
                        const char *atual,
                        const char *data)
{
    Historico *novo = (Historico*)malloc(sizeof(Historico));
    if (novo == NULL) {
        printf("Erro ao alocar memoria para historico!\n");
        return;
    }
    novo->id_denuncia = id_denuncia;
    strcpy(novo->estado_anterior, anterior);
    strcpy(novo->estado_atual,    atual);
    strcpy(novo->data,            data);
    novo->prox = NULL;

    if (*lista == NULL) {
        *lista = novo;
    } else {
        Historico *atual_h = *lista;
        while (atual_h->prox != NULL)
            atual_h = atual_h->prox;
        atual_h->prox = novo;
    }
}

void ImprimirHistorico(Historico *lista, int id_denuncia) {
    Historico *atual = lista;
    int encontrou = 0;

    printf("\nHistorico da Denuncia %d:\n", id_denuncia);

    while (atual != NULL) {
        if (atual->id_denuncia == id_denuncia) {
            printf("  [%s] %s -> %s\n",
                   atual->data,
                   atual->estado_anterior,
                   atual->estado_atual);
            encontrou = 1;
        }
        atual = atual->prox;
    }

    if (!encontrou)
        printf("  Sem registo de historico para esta denuncia.\n");
}

/* =========================================================
   HISTÓRICO GERAL — mostra todos os registos de todas as
   denúncias agrupados por ID
   ========================================================= */
void ImprimirHistoricoGeral(Historico *lista) {
    Historico *atual = lista;
    int ultimo_id = -1;

    if (lista == NULL) {
        printf("  Historico vazio.\n");
        return;
    }

    printf("\n=== HISTORICO GERAL ===\n");

    while (atual != NULL) {
        /* Cabeçalho por ID (só imprime quando muda de denúncia) */
        if (atual->id_denuncia != ultimo_id) {
            printf("\n  Denuncia ID %d:\n", atual->id_denuncia);
            ultimo_id = atual->id_denuncia;
        }
        printf("    [%s] %s -> %s\n",
               atual->data,
               atual->estado_anterior,
               atual->estado_atual);
        atual = atual->prox;
    }
}

/* =========================================================
   LISTAR DENÚNCIAS RESOLVIDAS — percorre a B-Tree e imprime
   apenas as denúncias com estado "resolvida", com o respectivo
   ID para facilitar a remoção posterior
   ========================================================= */

/* auxiliar recursivo */
static void ListarResolvidasNo(NoBTree *no, int *encontrou) {
    int i;
    if (no == NULL) return;

    for (i = 0; i < no->n_chaves; i++) {
        /* visita subárvore esquerda antes de imprimir (ordem crescente de ID) */
        if (!no->folha)
            ListarResolvidasNo(no->filhos[i], encontrou);

        if (strcmp(no->dados[i].estado, "resolvida") == 0) {
            printf("  ID: %-5d | Zona: %-20s | Data: %s | %s\n",
                   no->dados[i].id,
                   no->dados[i].zona,
                   no->dados[i].data,
                   no->dados[i].descricao);
            *encontrou = 1;
        }
    }
    /* última subárvore direita */
    if (!no->folha)
        ListarResolvidasNo(no->filhos[no->n_chaves], encontrou);
}

void ListarDenunciasResolvidas(BTree *bt) {
    int encontrou = 0;

    printf("\n=== DENUNCIAS RESOLVIDAS ===\n");
    printf("  (Use o ID abaixo para remover uma denuncia)\n");
    printf("  %-5s | %-20s | %-10s | %s\n", "ID", "Zona", "Data", "Descricao");
    printf("  %s\n", "------------------------------------------------------------");

    if (bt == NULL || bt->raiz == NULL) {
        printf("  Nenhuma denuncia registada.\n");
        return;
    }

    ListarResolvidasNo(bt->raiz, &encontrou);

    if (!encontrou)
        printf("  Nenhuma denuncia com estado 'resolvida'.\n");
}
