#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	BTree        *bt          = criarBTree();
    Grafo        *g           = CriarGafo();
    Historico    *hist        = NULL;
    NoUtilizador *utilizadores = NULL;
    
    CarregarDenuncias(bt, "denuncias.txt");
    CarregarZonas(g, "zonas.txt");
    CarregarConexoes(g, "conexoes.txt");
    CarregarHistorico(&hist, "historico.txt");
    utilizadores = CarregarUtilizadores("utilizadores.txt");

    MenuExecutar(bt, g, &hist, &utilizadores);
	return 0;
}
