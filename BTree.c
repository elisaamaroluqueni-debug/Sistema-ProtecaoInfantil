#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

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



