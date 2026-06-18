#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"


void ImprimirUtilizador(NoUtilizador *lista)
{
    if(lista == NULL)
    {
        printf("Nao existem utilizadores.\n");
        return;
    }

    while(lista != NULL)
    {
        printf("ID: %d\n", lista->dados.id);
        printf("Nome: %s\n", lista->dados.nome);
        printf("Perfil: %s\n", lista->dados.perfil);
        printf("Contacto: %s\n\n", lista->dados.contacto);

        lista = lista->prox;
    }
}
