#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Sistema.h"

/* variável local para LerInteiro — declarada aqui para evitar dependência circular */
static int LerInteiroGrafo(int *valor) {
    char buf[64]; int i = 0;
    if (scanf(" %63s", buf) != 1) return 0;
    if (buf[0] == '-') i = 1;
    for (; buf[i] != '\0'; i++) {
        if (buf[i] < '0' || buf[i] > '9') {
            printf("Entrada invalida. Insira apenas numeros.\n");
            return 0;
        }
    }
    *valor = atoi(buf);
    return 1;
}

Grafo *CriarGrafo(void) {
    int i;
    Grafo *g = (Grafo*)malloc(sizeof(Grafo));
    if (g == NULL) return NULL;
    g->n_zonas = 0;
    for (i = 0; i < MAX_ZONAS; i++) g->adjacentes[i] = NULL;
    return g;
}

int AdicionarZona(Grafo *g, const char *nome) {
    int id;
    if (g == NULL || g->n_zonas >= MAX_ZONAS) return -1;
    id = g->n_zonas;
    g->zonas[id].id          = id;
    strcpy(g->zonas[id].nome, nome);
    g->zonas[id].n_casos     = 0;
    g->zonas[id].nivel_risco = 0;
    g->adjacentes[id]        = NULL;
    g->n_zonas++;
    return id;
}

void ConectarZonas(Grafo *g, int id_a, int id_b, int peso) {
    Aresta *nova, *nova2;
    if (g == NULL || id_a >= g->n_zonas || id_b >= g->n_zonas) return;

    nova             = (Aresta*)malloc(sizeof(Aresta));
    nova->id_destino = id_b;
    nova->peso       = peso;
    nova->prox       = g->adjacentes[id_a];
    g->adjacentes[id_a] = nova;

    nova2             = (Aresta*)malloc(sizeof(Aresta));
    nova2->id_destino = id_a;
    nova2->peso       = peso;
    nova2->prox       = g->adjacentes[id_b];
    g->adjacentes[id_b] = nova2;
}

/* =========================================================
   ZONAS CRÍTICAS — só para admin (limiar >= 2)
   ========================================================= */
void ZonasCriticas(Grafo *g) {
    int i, encontrou = 0;
    if (g == NULL) return;
    printf("\n=== ZONAS CRITICAS ===\n");
    printf("  %-4s | %-20s | %-6s | %s\n", "ID", "Nome", "Casos", "Risco");
    printf("  %s\n", "--------------------------------------------------");
    for (i = 0; i < g->n_zonas; i++) {
        if (g->zonas[i].nivel_risco >= 2) {
            printf("  %-4d | %-20s | %-6d | %s\n",
                   g->zonas[i].id,
                   g->zonas[i].nome,
                   g->zonas[i].n_casos,
                   g->zonas[i].nivel_risco == 3 ? "ALTO" : "MEDIO");
            encontrou = 1;
        }
    }
    if (!encontrou) printf("  Nenhuma zona critica de momento.\n");
}

/* =========================================================
   VER CONEXÕES — só para admin
   Mostra zonas numeradas; utilizador escolhe pelo número.
   O peso representa a proximidade geográfica entre zonas.
   ========================================================= */
void MenuConexoes(Grafo *g) {
    int op, id_zona;
    if (g == NULL || g->n_zonas == 0) { printf("Nenhuma zona registada.\n"); return; }

    printf("\n=== CONEXOES ENTRE ZONAS ===\n");
    printf("Seleccione uma zona:\n");
    for (id_zona = 0; id_zona < g->n_zonas; id_zona++)
        printf("  %d - %s\n", id_zona + 1, g->zonas[id_zona].nome);
    printf("  0 - Voltar\n");
    printf("Opcao: ");

    if (!LerInteiroGrafo(&op)) return;
    if (op == 0) return;
    id_zona = op - 1;
    if (id_zona < 0 || id_zona >= g->n_zonas) { printf("Opcao invalida.\n"); return; }

    {
        Aresta *a = g->adjacentes[id_zona];
        printf("\n--- Zonas ligadas a '%s' ---\n", g->zonas[id_zona].nome);
        printf("(peso = proximidade geografica; menor = mais proximo)\n");
        if (a == NULL) { printf("  Sem conexoes registadas.\n"); return; }
        while (a != NULL) {
            printf("  -> %-20s (peso: %d)\n", g->zonas[a->id_destino].nome, a->peso);
            a = a->prox;
        }
    }
}

/* =========================================================
   GERIR ZONAS E CONEXÕES — só para admin
   Permite adicionar zonas e criar/ver conexões entre elas.
   ========================================================= */
void MenuGerirZonas(Grafo *g) {
    int op;
    do {
        printf("\n--- GERIR ZONAS E CONEXOES ---\n");
        printf("1. Adicionar zona\n");
        printf("2. Listar zonas\n");
        printf("3. Criar conexao entre zonas\n");
        printf("4. Ver conexoes de uma zona\n");
        printf("0. Voltar\n");
        printf("Opcao: ");

        if (!LerInteiroGrafo(&op)) continue;

        switch (op) {
            case 1: {
                char nome[100];
                printf("Nome da nova zona: ");
                scanf(" %99[^\n]", nome);
                int id = AdicionarZona(g, nome);
                if (id >= 0)
                    printf("Zona '%s' adicionada com ID %d.\n", nome, id);
                else
                    printf("Nao foi possivel adicionar (limite atingido).\n");
                break;
            }
            case 2: {
                int i;
                if (g->n_zonas == 0) { printf("Nenhuma zona registada.\n"); break; }
                printf("\n%-4s | %-20s | %-6s | %s\n", "ID","Nome","Casos","Risco");
                printf("%s\n","---------------------------------------------");
                for (i = 0; i < g->n_zonas; i++) {
                    const char *risco;
                    if      (g->zonas[i].nivel_risco == 3) risco = "ALTO";
                    else if (g->zonas[i].nivel_risco == 2) risco = "MEDIO";
                    else if (g->zonas[i].nivel_risco == 1) risco = "BAIXO";
                    else                                    risco = "SEM CASOS";
                    printf("%-4d | %-20s | %-6d | %s\n",
                           g->zonas[i].id, g->zonas[i].nome,
                           g->zonas[i].n_casos, risco);
                }
                break;
            }
            case 3: {
                int i, id_a, id_b, peso;
                if (g->n_zonas < 2) { printf("Precisa de pelo menos 2 zonas.\n"); break; }
                printf("\nZonas disponiveis:\n");
                for (i = 0; i < g->n_zonas; i++)
                    printf("  %d - %s\n", i+1, g->zonas[i].nome);
                printf("Zona A (numero): "); if (!LerInteiroGrafo(&id_a)) break;
                printf("Zona B (numero): "); if (!LerInteiroGrafo(&id_b)) break;
                id_a--; id_b--;
                if (id_a < 0 || id_a >= g->n_zonas || id_b < 0 || id_b >= g->n_zonas) {
                    printf("Zonas invalidas.\n"); break;
                }
                if (id_a == id_b) { printf("Nao pode ligar uma zona a si mesma.\n"); break; }
                printf("Peso/proximidade (1=muito proximas, 10=distantes): ");
                if (!LerInteiroGrafo(&peso) || peso < 1) break;
                ConectarZonas(g, id_a, id_b, peso);
                printf("Conexao criada: %s <-> %s (peso %d)\n",
                       g->zonas[id_a].nome, g->zonas[id_b].nome, peso);
                break;
            }
            case 4:
                MenuConexoes(g);
                break;
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}
