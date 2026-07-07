#ifndef SISTEMA_H
#define SISTEMA_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
/* =========================================================
   CONSTANTES
   ========================================================= */
#define T          4
#define MAX_ZONAS  50
/* =========================================================
   ESTRUTURAS
   ========================================================= */
typedef struct {
    int  id;               /* ID aleatorio de 4 digitos (1000-9999) */
    char descricao[256];
    char zona[64];
    int  gravidade;        /* 1-5, definido por qualquer utilizador */
    char data[11];
        char estado[20];
    char tipo_violencia[50];  /* tipo de violencia sofrida pela crianca */
    int id_utilizador;
    char nome_denunciante[100];
    char telefone[10];     /* 9 digitos */
    char morada[150];
} Denuncia;
typedef struct NoBTree {
    int      n_chaves;
    int      folha;
    Denuncia dados[2*T - 1];
    struct NoBTree *filhos[2*T];
} NoBTree;
typedef struct {
    NoBTree *raiz;
} BTree;
typedef struct Aresta {
    int           id_destino;
    int           peso;
    struct Aresta *prox;
} Aresta;
typedef struct {
    int  id;
    char nome[100];
    int  n_casos;
    int  nivel_risco;   /* 1=baixo 2=medio 3=alto */
} Zona;
typedef struct {
    int    n_zonas;
    Zona   zonas[MAX_ZONAS];
    Aresta *adjacentes[MAX_ZONAS];
} Grafo;
typedef struct Historico {
    int  id_denuncia;
    char estado_anterior[20];
    char estado_atual[20];
    char data[30];
    struct Historico *prox;
} Historico;
typedef struct {
    int  id;           /* ID aleatorio de 4 digitos a "senha" do utilizador */
    char nome[100];
    char perfil[20];   /* "admin" ou "utilizador" */
    char telefone[10];
    char morada[150];
    char senha[50];    /* senha preenchida para admins */
} Utilizador;
typedef struct NoUtilizador {
    Utilizador          dados;
    struct NoUtilizador *prox;
} NoUtilizador;
/* =========================================================
 BTree
   ========================================================= */
BTree    *criarBTree(void);
NoBTree  *CriarNo(int folha);
void      ImprimirBTree(BTree *bt);
void      ImprimirNo(NoBTree *raiz);
void      InserirNaBTree(BTree *bt, Denuncia d);
void      InserirNaoCheio(NoBTree *x, Denuncia d);
void      DividirFilho(NoBTree *x, int i);
int       RemoverNaBTree(BTree *bt, int id);
void      RemoverNo(NoBTree *no, int id);
Denuncia *PesquisarNaBTree(BTree *bt, int id);
Denuncia *PesquisarNo(NoBTree *no, int id);
int       GerarIDUnico(BTree *bt);   /* ID aleatorio

o */
/* =========================================================
  GRAFO
   ========================================================= */
Grafo *CriarGrafo(void);
int    AdicionarZona(Grafo *g, const char *nome);
void   ConectarZonas(Grafo *g, int id_a, int id_b, int peso);
void   ZonasCriticas(Grafo *g);
void   MenuConexoes(Grafo *g);
void   MenuGerirZonas(Grafo *g);
/* =========================================================
  DENUNCIA
   ========================================================= */
void RegistarDenuncia(BTree *bt, Grafo *g, Historico **hist,
                      Denuncia d, int is_admin);
/* =========================================================
 HISTORICO
   ========================================================= */
void AdicionarHistorico(Historico **lista, int id_denuncia,
                        const char *anterior,
                        const char *atual,
                        const char *data);
void ImprimirHistorico(Historico *lista, int id_denuncia);
void ImprimirHistoricoGeral(Historico *lista);
void ListarDenunciasResolvidas(BTree *bt);
/* =========================================================
 Utilizadores
   ========================================================= */
NoUtilizador *AdicionarUtilizador(NoUtilizador *lista, Utilizador u);
void          ImprimirUtilizador(NoUtilizador *lista);
NoUtilizador *RemoverUtilizador(NoUtilizador *lista, int id);
NoUtilizador *PesquisarUtilizadorPorNomeSenha(NoUtilizador *lista,
                                              const char *nome,
                                              const char *senha);
NoUtilizador *PesquisarUtilizadorPorNomeID(NoUtilizador *lista,
                                           const char *nome, int id);
int           GerarIDUtilizador(NoUtilizador *lista);
/* =========================================================
   PROToTIPOS Autenticao
   ========================================================= */
int            AutenticarAdmin(NoUtilizador *lista);
NoUtilizador  *AutenticarUtilizador(NoUtilizador *lista);
/* =========================================================
   PROToTIPOS Persistencia
   ========================================================= */
void          GuardarDenuncias(BTree *bt, const char *f);
void          CarregarDenuncias(BTree *bt, const char *f);
void          GuardarUtilizadores(NoUtilizador *lista, const char *f);
NoUtilizador *CarregarUtilizadores(const char *f);
void          GuardarZonas(Grafo *g, const char *f);
void          CarregarZonas(Grafo *g, const char *f);
void          GuardarConexoes(Grafo *g, const char *f);
void          CarregarConexoes(Grafo *g, const char *f);
void          GuardarHistorico(Historico *lista, const char *f);
void          CarregarHistorico(Historico **lista, const char *f);
/* =========================================================
   PROToTIPOS Menu
   ========================================================= */
void MenuExecutar(BTree *bt, Grafo *g, Historico **hist,
                  NoUtilizador **utilizadores);
#endif
