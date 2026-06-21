#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

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

