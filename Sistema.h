typedef struct Denuncia Denuncia;
typedef struct Utilizador Utilizador;
typedef struct Historico Historico;
typedef struct NoBTree NoBTree;
typedef struct BTree BTree;
typedef struct Zona Zona;
typedef struct Aresta Aresta;
typedef struct Grafo Grafo;
typedef struct NoUtilizador NoUtilizador;
 
struct Denuncia {
    int  id;
    char descricao[256];
    char zona[64];
    int  gravidade;
    char data[11];
    char estado[20];
};
 
struct Utilizador {
    int  id;
    char nome[100];
    char perfil[20];
    char contacto[50];
};

struct Historico{
	int id_denuncia;
	char estado_anterior[20];
	char estado_atual[20];
	char data[30];
	Historico *prox;
};

struct NoBTree{
	Denuncia dados[7];
	NoBTree *filhos [8];
	int n_chaves;
	int folha;
};

struct BTree{
	NoBTree *raiz;
}; 

struct Zona{
	int id; 
	char nome[100];
	int n_casos;
	int nivel_risco;
};

struct Aresta {
    int     id_destino;
    int     peso;
    Aresta *prox;
};
 
struct Grafo {
    Zona    zonas[50];
    Aresta *adjacentes[50];
    int     n_zonas;
};
 
struct NoUtilizador {
    Utilizador    dados;
    NoUtilizador *prox;
};

// Declaração das funções para a B-Tree
BTree    *criarBTree(void);
void      InserirNaBTree(BTree *bt, Denuncia d);
Denuncia *PesquisarNaBTree(BTree *bt, int id);
int       RemoverNaBTree(BTree *bt, int id);
void      ImprimirBTree(BTree *bt);

// Declaração das funções do Grafo
Grafo *CriarGafo(void);
int    AdicionarZona(Grafo *g, const char *nome);
void   ConectarZonas(Grafo *g, int id_a, int id_b, int peso);
void   ZonasCriticas(Grafo *g);

// Funções para adicionar histórico
void AdicionarHistorico(Historico **lista, int id_denuncia, const char *anterior, const char *atual, const char *data);
void ImprimirHistorico(Historico *lista, int id_denuncia);

// Funções para utilizadores
NoUtilizador *AdicionarUtilizador(NoUtilizador *lista, Utilizador u);
void          ImprimirUtilizador(NoUtilizador *lista);

// Função para denuncias
void RegistarDenuncia(BTree *bt, Grafo *g, Historico **hist, Denuncia d);

// Função para executar o menu
void MenuExecutar(BTree *bt, Grafo *g, Historico **hist, NoUtilizador **utilizadores);
