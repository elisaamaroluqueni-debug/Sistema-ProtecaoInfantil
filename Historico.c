#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

//função Adicionar Historico 
void AdicionarHistorico(Historico **lista, int id_denuncia,
                        const char *anterior,
                        const char *atual,
                        const char *data)
{
    Historico *novo = (Historico*)malloc(sizeof(Historico));

    if(novo == NULL)
    {
        printf("Erro ao alocar memoria para historico!\n");
        return;
    }

    novo->id_denuncia = id_denuncia;

    strcpy(novo->estado_anterior, anterior);
    strcpy(novo->estado_atual, atual);
    strcpy(novo->data, data);

    novo->prox = NULL;

    if(*lista == NULL)
    {
        *lista = novo;
    }
    else
    {
        Historico *atual_h = *lista;

        while(atual_h->prox != NULL)
        {
            atual_h = atual_h->prox;
        }

        atual_h->prox = novo;
    }
}


//Função para Imprimir Historico
void ImprimirHistorico(Historico *lista, int id_denuncia){
	Historico *atual = lista;
	int encontrou = 0;
	printf("\nHistorico de Denuncias %d\n", id_denuncia);
	 
    while(atual!= NULL){
	    if(atual->id_denuncia == id_denuncia){
	 	printf("[%s] %s -> %s\n", atual->data, atual->estado_anterior, atual->estado_atual);
	 	encontrou = 1;
		 }
		atual = atual->prox;
	 }
	if (!encontrou)
	printf("Sem registo de historico para a denuncia");
}

