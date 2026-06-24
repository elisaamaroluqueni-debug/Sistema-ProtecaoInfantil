#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

// Função registrar Denuncia

void RegistarDenuncia(BTree *bt,
                      Grafo *g,
                      Historico **hist,
                      Denuncia d)
{
    int i;

    /* Inserir denúncia na B-Tree */
    InserirNaBTree(bt, d);

    /* Procurar a zona correspondente */
    for(i = 0; i < g->n_zonas; i++)
    {
        if(strcmp(g->zonas[i].nome, d.zona) == 0)
        {
            g->zonas[i].n_casos++;

            /* Atualização simples do nível de risco */
            if(g->zonas[i].n_casos >= 10)
                g->zonas[i].nivel_risco = 3;
            else if(g->zonas[i].n_casos >= 5)
                g->zonas[i].nivel_risco = 2;
            else
                g->zonas[i].nivel_risco = 1;

            break;
        }
    }

    /* Registar o primeiro estado no histórico */
    AdicionarHistorico(hist,d.id,"Nenhum", d.estado, d.data );

    printf("Denuncia registada com sucesso!\n");
}
