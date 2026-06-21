#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

//Função para executar o menu
void MenuExecutar(BTree *bt, Grafo *g, Historico **hist, NoUtilizador **utilizadores)
{
    int op;

    do {
        printf("\nSISTEMA DE DENUNCIA E PROTECAO INFANTIL\n");
        printf("1. Registar Denuncia\n");
        printf("2. Pesquisar Denuncia por Id\n");
        printf("3. Actualizar o estado do caso\n");
        printf("4. Ver Zonas Criticas\n");
        printf("5. Ver conexoes entre casos \n");
        printf("6. Ver Historico de uma Zona \n");
        printf("7. Resolver Denuncia Resolvida \n");
        printf("8. Gerir utilizadores \n");
        printf("0. Sair \n");

        scanf("%d", &op);

        switch (op)
        {
            case 1:
                break;

            case 2:
                break;

            case 3:
                break;

            case 4:
                break;

            case 5:
                break;

            case 6:
                break;

            case 7:
                break;

            case 8:
                break;

            case 0:
                printf("Saindo do programa\n");
                break;
        }
    } while (op != 0);
}
