#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"
#define T 4

BTree *criarBTree(void)
{
    BTree *bt = (BTree*)malloc(sizeof(BTree));

    bt->raiz = NULL;

    return bt;
}

NoBTree *CriarNo(int folha)
{
    int i;

    NoBTree *novo = (NoBTree*)malloc(sizeof(NoBTree));

    novo->folha = folha;
    novo->n_chaves = 0;

    for(i=0;i<2*T;i++)
        novo->filhos[i] = NULL;

    return novo;
}

void ImprimirNo(NoBTree *raiz)
{
    int i;

    if(raiz == NULL)
        return;

    for(i = 0; i < raiz->n_chaves; i++)
    {
        if(!raiz->folha)
            ImprimirNo(raiz->filhos[i]);

        printf("ID: %d\n", raiz->dados[i].id);
        printf("Descricao: %s\n", raiz->dados[i].descricao);
        printf("Zona: %s\n", raiz->dados[i].zona);
        printf("Gravidade: %d\n", raiz->dados[i].gravidade);
        printf("Data: %s\n", raiz->dados[i].data);
        printf("Estado: %s\n\n", raiz->dados[i].estado);
    }

    if(!raiz->folha)
        ImprimirNo(raiz->filhos[i]);
}


void DividirFilho(NoBTree *x, int i)
{
    int j;

    NoBTree *y = x->filhos[i];
    NoBTree *z = CriarNo(y->folha);

    z->n_chaves = T - 1;

    
    for(j = 0; j < T - 1; j++)
        z->dados[j] = y->dados[j + T];

    // Se não for folha, copiar filhos
    if(!y->folha)
    {
        for(j = 0; j < T; j++)
            z->filhos[j] = y->filhos[j + T];
    }

    y->n_chaves = T - 1;

 
    for(j = x->n_chaves; j >= i + 1; j--)
        x->filhos[j + 1] = x->filhos[j];

    x->filhos[i + 1] = z;

   
    for(j = x->n_chaves - 1; j >= i; j--)
        x->dados[j + 1] = x->dados[j];

    x->dados[i] = y->dados[T - 1];

    x->n_chaves++;
}

void InserirNaoCheio(NoBTree *x, Denuncia d)
{
    int i = x->n_chaves - 1;

    if(x->folha)
    {
        while(i >= 0 && d.id < x->dados[i].id)
        {
            x->dados[i + 1] = x->dados[i];
            i--;
        }

        x->dados[i + 1] = d;
        x->n_chaves++;
    }
    else
    {
        while(i >= 0 && d.id < x->dados[i].id)
            i--;

        i++;

        if(x->filhos[i]->n_chaves == 2*T - 1)
        {
            DividirFilho(x, i);

            if(d.id > x->dados[i].id)
                i++;
        }

        InserirNaoCheio(x->filhos[i], d);
    }
}

void InserirNaBTree(BTree *bt, Denuncia d)
{
    NoBTree *r = bt->raiz;

    if(r == NULL)
    {
        bt->raiz = CriarNo(1);
        bt->raiz->dados[0] = d;
        bt->raiz->n_chaves = 1;
        return;
    }

    if(r->n_chaves == 2*T - 1)
    {
        NoBTree *s = CriarNo(0);

        bt->raiz = s;
        s->filhos[0] = r;

        DividirFilho(s, 0);

        InserirNaoCheio(s, d);
    }
    else
    {
        InserirNaoCheio(r, d);
    }
}

//Função remover da B-Tree e suas auxiliares
/* protótipos no topo do ficheiro */
int EncontrarChaves(NoBTree *no, int id);
void RemoverDeInterno(NoBTree *no, int id1);
void RemoverNo(NoBTree *no, int id);
void Merge(NoBTree *no, int id1);
void Preencher(NoBTree *no, int id1);

//Auxiliar para encontrar as Chaves
int EncontrarChaves(NoBTree *no, int id){
  
  int id1=0;
  while (id1 < no->n_chaves && no->dados[id1].id < id)
      id1++;
  return id1;
  
}
// Auxiliar para Remover se a chave for folha
void RemoverDeFolha(NoBTree *no, int id1){
	int i;
	for(i=id1+1 ;i< no->n_chaves; i++)
	no->dados [i-1] = no->dados[i];
	no->n_chaves--;
	
}
//Auxiliar para obter o antecessor
Denuncia ObterProdecessor(NoBTree *no, int id1){
	NoBTree *atual = no->filhos[id1];
	while(!atual->folha)
	atual = atual->filhos[atual->n_chaves];
	return atual->dados[atual->n_chaves - 1];
	
}

//Auxiliar para obter sucessor
Denuncia ObterSucessor(NoBTree *no, int id1){
	NoBTree *atual = no->filhos[id1 +1];
	while(!atual->folha)
	atual = atual->filhos[0];
	return atual->dados[0];
}
//Auxiliar para fazer o merge
void Merge(NoBTree *no, int id1){
	int i;
	NoBTree *filho= no->filhos[id1];
	NoBTree *irmao = no->filhos[id1 +1];
	
	filho->dados[T -1] = no->dados[id1];
	
	for(i=0; i<irmao->n_chaves; i++)
	filho->dados[i +T]= irmao->dados[i];
	
	if(!filho->folha)
	 for(i=0; i<= irmao->n_chaves; i++)
	  filho->filhos[i +T] = irmao->filhos[i];
	  
   for(i=id1 +1; i< no->n_chaves; i++)
     no->dados[i -1] = no->dados[i];
    
	for(i= id1+2; i<= no->n_chaves; i++)
	  no->filhos[i -1] = no->filhos[i];
	  
   filho->n_chaves += irmao->n_chaves +1;
   no->n_chaves--;
   
   free(irmao);	  
}

//Auxiliar para remover uma chave que não é uma folha
void RemoverDeInterno(NoBTree *no, int id1){
	int id = no->dados[id1].id;
	if(no->filhos[id1]->n_chaves>= T){
		Denuncia pred = ObterProdecessor(no,id1);
		no->dados[id1] = pred;
		RemoverNo(no->filhos[id1], pred.id);
	}
	
	else if(no->filhos[id1 +1]->n_chaves>=T){
		Denuncia suc = ObterSucessor(no,id1);
	    no->dados[id1] = suc;
	    RemoverNo(no->filhos[id1 +1], suc.id);
	}
	else
	{
        Merge(no, id1);
        RemoverNo(no->filhos[id1], id);
    }
}

//Auxiliar para remover um nó
void RemoverNo(NoBTree *no, int id){
	int id1 = EncontrarChaves(no, id);
	if (id1 < no->n_chaves && no->dados[id1].id== id){
		if(no->folha)
		RemoverDeFolha(no, id1);
		else
		RemoverDeInterno(no, id1);
		
	}
	else{
		if(no->folha){
			printf("ID %d nao encontrado.\n", id);
			return;
		}
		int ultimo = (id1 == no->n_chaves);
		if (no->filhos[id1]->n_chaves< T)
		 Preencher(no,id1);
		
		if( ultimo && id1 > no->n_chaves)
		 RemoverNo(no->filhos[id1 -1], id);
	    else
	     RemoverNo(no->filhos[id1],id);
	}
}


//Auxiliar para emprestar ao maior a esquerda
void EmprestarDoAnterior(NoBTree *no, int id1){
	int i;
	NoBTree *filho= no->filhos[id1];
	NoBTree *irmao= no->filhos[id1-1];
	for (i=filho->n_chaves-1; i>=0;i--)
	   filho->dados[i+1] = filho->dados[i];
	
	if(!filho->folha)
	 for(i= filho->n_chaves; i>=0; i--)
	  filho->filhos[i+1] = filho->filhos[i];
    
	filho->dados[0] = no->dados[id1 -1];
	
	if(!filho->folha)
	 filho->filhos[0] = irmao->filhos[irmao->n_chaves];

    no->dados[id1 -1] = irmao->dados[irmao->n_chaves -1];
    
    filho->n_chaves++;
    irmao->n_chaves--;
}

//Auxiliar para emprestar o menor a direita
void EmprestarSeguinte(NoBTree *no, int id1){
	int i;
	NoBTree *filho= no->filhos[id1];
	NoBTree *irmao= no->filhos[id1 +1];
	
	filho->dados[filho->n_chaves]= no->dados[id1];
	
	if (!filho->folha)
	filho->filhos[filho->n_chaves+1] = irmao->filhos[0];
	
	no->dados[id1] = irmao->dados[0];
	
	for(i=1; i< irmao->n_chaves; i++)
	irmao->dados[i -1] = irmao->dados[i];
	
	if (!irmao->folha)
	for(i=1; i<= irmao->n_chaves;i++)
	irmao->filhos[i -1] =irmao->filhos[i];
	
	filho->n_chaves++;
	irmao->n_chaves--;
}

//Auxiliar para preencher nos que têm apenas uma chave
void Preencher(NoBTree *no, int id1){
	if (id1 != 0 && no->filhos[id1 - 1]->n_chaves >= T)
        EmprestarDoAnterior(no, id1);
    else if (id1 != no->n_chaves && no->filhos[id1 + 1]->n_chaves >= T)
        EmprestarSeguinte(no, id1);
    else
    {
        if (id1 != no->n_chaves)
            Merge(no, id1);
        else
            Merge(no, id1 - 1);
    }
}


//Função Remover
int RemoverNaBTree(BTree *bt, int id){
	if(bt->raiz == NULL)
	  return 0;
    if (PesquisarNaBTree(bt, id) == NULL){
    	printf("Denuncia %d nao encontrada.\n", id);
    	return 0;
	}
	RemoverNo(bt->raiz, id);
	if (bt->raiz->n_chaves == 0 && !bt->raiz->folha){
		NoBTree *antigaRaiz = bt->raiz;
		bt->raiz = bt->raiz->filhos[0];
		free(antigaRaiz);
	}
	printf("Denuncia %d removida com sucesso\n", id);
	return 1;
}
// Função pesquisar
Denuncia *PesquisarNo(NoBTree *no, int id)
{
    int i = 0;

    while(i < no->n_chaves && id > no->dados[i].id)
        i++;

    if(i < no->n_chaves && no->dados[i].id == id)
        return &no->dados[i];

    if(no->folha)
        return NULL;

    return PesquisarNo(no->filhos[i], id);
}
Denuncia *PesquisarNaBTree(BTree *bt, int id)
{
    if(bt == NULL || bt->raiz == NULL)
        return NULL;

    return PesquisarNo(bt->raiz, id);
}

//Fun��o para imprimir a BTree
void      ImprimirBTree(BTree *bt){
	if (bt == NULL || bt->raiz == NULL){
		printf("Arvore vazia\n");
		return;
	}
	ImprimirNo(bt->raiz);
}
