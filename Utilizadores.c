#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

//Função para adicionar utilizadores
NoUtilizador *AdicionarUtilizador(NoUtilizador *lista, Utilizador u)
{
	NoUtilizador *novo = (NoUtilizador*)malloc(sizeof (NoUtilizador));
	
	if (!novo)
    {
        printf("Erro: memoria insuficiente.\n");
        return lista;  
    }
    
	novo->dados = u;
	novo->prox = lista;
	return novo;
}

//Função para imprimir Utilizadores
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
