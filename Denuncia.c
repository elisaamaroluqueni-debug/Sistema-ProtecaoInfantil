#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sistema.h"
 
/* O ID ja vem gerado do Menu antes de chamar esta funcao.
   Aqui apenas: insere na BTree, atualiza o grafo e regista no historico. */
static int ZonaTemMesmoTipo(NoBTree *no, const char *zona, const char *tipo, int id_ignorar) {
    int i;
    if (no == NULL) return 0;

    for (i = 0; i < no->n_chaves; i++) {
        if (no->dados[i].id != id_ignorar &&
            strcmp(no->dados[i].zona, zona) == 0 &&
            strcmp(no->dados[i].tipo_violencia, tipo) == 0)
            return 1;
    }

    if (!no->folha) {
        for (i = 0; i <= no->n_chaves; i++)
            if (ZonaTemMesmoTipo(no->filhos[i], zona, tipo, id_ignorar))
                return 1;
    }

    return 0;
}
 
void RegistarDenuncia(BTree *bt, Grafo *g, Historico **hist,
                      Denuncia d, int is_admin)
{
    int i, zona_encontrada = 0;
 
    /* Garantir que o estado esta preenchido */
    if (strlen(d.estado) == 0)
        strcpy(d.estado, "pendente");
 
    /* Inserir a denuncia na B-Tree */
    InserirNaBTree(bt, d);
 
    /* Actualizar o numero de casos e nivel de risco da zona no Grafo */
    for (i = 0; i < g->n_zonas; i++) {
        if (strcmp(g->zonas[i].nome, d.zona) == 0) {
            g->zonas[i].n_casos++;
 
            /* recalcula nivel de risco com base no total de casos */
            if (g->zonas[i].n_casos >= 10)
                g->zonas[i].nivel_risco = 3;   /* ALTO  */
            else if (g->zonas[i].n_casos >= 5)
                g->zonas[i].nivel_risco = 2;   /* MEDIO */
            else
                g->zonas[i].nivel_risco = 1;   /* BAIXO */
 
            zona_encontrada = 1;
            break;
        }
    }
 
    if (!zona_encontrada)
        printf("Aviso: zona '%s' nao encontrada no grafo.\n", d.zona);
 
    /* Registar no historico o estado inicial da denuncia */
    AdicionarHistorico(hist, d.id, "Nenhum", d.estado, d.data);
 
    /* ----------------------------------------------------------------
       CONEXOES AUTOMATICAS POR TIPO DE VIOLENCIA
       Sempre que uma denuncia e registada, o sistema verifica as outras
       zonas do grafo. Se uma zona ja tem denuncia com o mesmo tipo de
       violencia, cria uma aresta entre a zona actual e essa zona
       (peso = 1). Se ja existir uma aresta, incrementa o peso (+1).
       Isto permite mapear automaticamente a propagacao de violencia
       entre zonas geograficas sem intervencao manual do administrador.
       ---------------------------------------------------------------- */
    if (strlen(d.tipo_violencia) > 0 && strlen(d.zona) > 0) {
        int zi, zj;
        int zona_atual = -1;
 
        /* encontra o indice da zona desta denuncia no grafo */
        for (zi = 0; zi < g->n_zonas; zi++) {
            if (strcmp(g->zonas[zi].nome, d.zona) == 0) {
                zona_atual = zi;
                break;
            }
        }
 
        if (zona_atual >= 0) {
            /* percorre todas as outras zonas do grafo */
            for (zj = 0; zj < g->n_zonas; zj++) {
                int ja_existe = 0;
                Aresta *a;
 
                /* nao conecta uma zona consigo propria */
                if (zj == zona_atual) continue;
 
                /* verifica se ja existe aresta entre zona_atual e zj */
                a = g->adjacentes[zona_atual];
                while (a != NULL) {
                    if (a->id_destino == zj) {
                        /* aresta ja existe — incrementa o peso em ambas as direccoes */
                        a->peso++;
                        Aresta *b = g->adjacentes[zj];
                        while (b != NULL) {
                            if (b->id_destino == zona_atual) { b->peso++; break; }
                            b = b->prox;
                        }
                        ja_existe = 1;
                        break;
                    }
                    a = a->prox;
                }
 
                /* se a outra zona tem caso do mesmo tipo, cria nova conexao */
                if (!ja_existe && ZonaTemMesmoTipo(bt->raiz, g->zonas[zj].nome, d.tipo_violencia, d.id)) {
                    ConectarZonas(g, zona_atual, zj, 1);
                    printf("  [Grafo] Conexao criada: %s <-> %s (tipo: %s)\n",
                    d.zona, g->zonas[zj].nome, d.tipo_violencia);
                }
            }
        }
    }
 
    /* Mensagem de confirmacao para o utilizador */
    printf("\n=====================================\n");
    printf(" Denuncia registada com sucesso!\n");
    printf(" ID da denuncia : %d\n", d.id);
    printf(" Zona           : %s\n", d.zona);
    printf(" Tipo de violencia: %s\n", d.tipo_violencia);
    printf(" Estado inicial : %s\n", d.estado);
    if (!is_admin && d.id_utilizador > 0)
        printf(" Denunciante    : %s\n", d.nome_denunciante);
    printf("=====================================\n\n");
}
