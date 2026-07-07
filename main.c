#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Sistema.h"

int main(void) {
    srand((unsigned int)time(NULL));   /* inicializar gerador de números aleatórios */

    BTree        *bt           = criarBTree();
    Grafo        *g            = CriarGrafo();
    Historico    *hist         = NULL;
    NoUtilizador *utilizadores = NULL;

    /* Carregar dados guardados */
    CarregarZonas(g,         "zonas.txt");
    CarregarConexoes(g,      "conexoes.txt");
    CarregarDenuncias(bt,    "denuncias.txt");
    CarregarHistorico(&hist, "historico.txt");
    utilizadores = CarregarUtilizadores("utilizadores.txt");

    /* Se não há nenhum admin, criar o admin padrão */
    {
        int tem_admin = 0;
        NoUtilizador *tmp = utilizadores;
        while (tmp != NULL) {
            if (strcmp(tmp->dados.perfil, "admin") == 0) { tem_admin = 1; break; }
            tmp = tmp->prox;
        }
        if (!tem_admin) {
            Utilizador admin_padrao;
            memset(&admin_padrao, 0, sizeof(admin_padrao));
            admin_padrao.id = 9999;
            strcpy(admin_padrao.nome,   "Silvia Antonio");
            strcpy(admin_padrao.perfil, "admin");
            strcpy(admin_padrao.senha,  "1369");
            strcpy(admin_padrao.telefone, "000000000");
            strcpy(admin_padrao.morada,   "Sistema");
            utilizadores = AdicionarUtilizador(utilizadores, admin_padrao);
            printf("[Sistema] Admin padrao criado: nome='Silvia Antonio' senha='1369'\n");
        }
    }

    /* Zonas de exemplo se não houver nenhuma guardada */
    if (g->n_zonas == 0) {
        int talatona = AdicionarZona(g, "Talatona");
        int isptec   = AdicionarZona(g, "ISPTEC");
        int oscar    = AdicionarZona(g, "Oscar");
        int isaf     = AdicionarZona(g, "ISAF");
        int patriota = AdicionarZona(g, "Patriota");
        int kilamba  = AdicionarZona(g, "Kilamba");

        ConectarZonas(g, talatona, isptec,   2);
        ConectarZonas(g, talatona, oscar,    3);
        ConectarZonas(g, talatona, isaf,     4);
        ConectarZonas(g, isptec,   oscar,    1);
        ConectarZonas(g, oscar,    isaf,     2);
        ConectarZonas(g, patriota, kilamba,  5);
        ConectarZonas(g, talatona, patriota, 6);
    }

    MenuExecutar(bt, g, &hist, &utilizadores);
    return 0;
}
