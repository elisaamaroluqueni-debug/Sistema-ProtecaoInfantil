#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "Sistema.h"
 
/* =========================================================
   UTILITARIOS DE VALIDACAO E AUXILIARES
   ========================================================= */
 
/* Le um inteiro do teclado e rejeita letras */
static int LerInteiro(int *valor) {
    char buf[64];
    int i = 0;
    if (scanf(" %63s", buf) != 1) return 0;
    if (buf[0] == '-') i = 1;
    for (; buf[i] != '\0'; i++) {
        if (buf[i] < '0' || buf[i] > '9') {
            printf("  Entrada invalida. Insira apenas numeros.\n");
            return 0;
        }
    }
    *valor = atoi(buf);
    return 1;
}
 
/* Valida nome: so letras e espacos */
static int NomeValido(const char *s) {
    int i;
    if (strlen(s) == 0) return 0;
    for (i = 0; s[i] != '\0'; i++)
        if (!isalpha((unsigned char)s[i]) && s[i] != ' ') return 0;
    return 1;
}
 
/* Valida telefone: exactamente 9 digitos */
static int TelefoneValido(const char *s) {
    int i;
    if (strlen(s) != 9) return 0;
    for (i = 0; i < 9; i++)
        if (!isdigit((unsigned char)s[i])) return 0;
    return 1;
}
 
/* Obtem a data actual no formato DD/MM/AAAA */
static void ObterDataAtual(char *buf, int tam) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, tam, "%d/%m/%Y", tm_info);
}
 
/* Converte numero de opcao para string de estado */
static const char *NumeroParaEstado(int n) {
    switch (n) {
        case 1: return "pendente";
        case 2: return "em_analise";
        case 3: return "resolvida";
        case 4: return "arquivada";
        default: return NULL;
    }
}
 
/* =========================================================
   AUTENTICACAO DO ADMINISTRADOR (nome + senha)
   ========================================================= */
int AutenticarAdmin(NoUtilizador *lista) {
    char nome[100], senha[50];
    int tentativas = 3;
 
    printf("\n--- ACESSO ADMINISTRADOR ---\n");
 
    while (tentativas > 0) {
        printf("Nome: ");
        scanf(" %99[^\n]", nome);
        printf("Senha: ");
        scanf(" %49[^\n]", senha);
 
        if (PesquisarUtilizadorPorNomeSenha(lista, nome, senha) != NULL) {
            printf("Autenticacao bem-sucedida. Bem-vindo, %s!\n\n", nome);
            return 1;
        }
 
        tentativas--;
        if (tentativas > 0)
            printf("Credenciais incorrectas. Tentativas restantes: %d\n", tentativas);
    }
 
    printf("Acesso bloqueado. Demasiadas tentativas.\n");
    return 0;
}
 
/* =========================================================
   AUTENTICACAO DO UTILIZADOR (nome + ID como senha)
   ========================================================= */
NoUtilizador *AutenticarUtilizador(NoUtilizador *lista) {
    char nome[100];
    int  id;
    int tentativas = 3;
 
    printf("\n--- ENTRAR COMO UTILIZADOR REGISTADO ---\n");
    printf("(Use o nome e o ID que recebeu no registo)\n");
 
    while (tentativas > 0) {
        printf("Nome: ");
        scanf(" %99[^\n]", nome);
        printf("ID (senha): ");
        if (!LerInteiro(&id)) { tentativas--; continue; }
 
        NoUtilizador *u = PesquisarUtilizadorPorNomeID(lista, nome, id);
        if (u != NULL) {
            printf("Bem-vindo, %s!\n\n", u->dados.nome);
            return u;
        }
 
        tentativas--;
        if (tentativas > 0)
            printf("Dados incorrectos. Tentativas restantes: %d\n", tentativas);
    }
 
    printf("Acesso bloqueado.\n");
    return NULL;
}
 
/* =========================================================
   REGISTO DE NOVO UTILIZADOR
   ========================================================= */
static NoUtilizador *RegistarNovoUtilizador(NoUtilizador **utilizadores) {
    Utilizador u;
    memset(&u, 0, sizeof(u));
 
    printf("\n--- CRIAR CONTA DE UTILIZADOR ---\n");
 
    /* Nome: so letras e espacos */
    do {
        printf("Nome completo: ");
        scanf(" %99[^\n]", u.nome);
        if (!NomeValido(u.nome))
            printf("  Nome invalido. Nao pode conter numeros ou simbolos.\n");
    } while (!NomeValido(u.nome));
 
    /* Telefone: exactamente 9 digitos */
    do {
        printf("Numero de telefone (9 digitos): ");
        scanf(" %9s", u.telefone);
        if (!TelefoneValido(u.telefone))
            printf("  Telefone invalido. Insira exactamente 9 digitos numericos.\n");
    } while (!TelefoneValido(u.telefone));
 
    /* Morada: aceita qualquer texto */
    printf("Morada: ");
    scanf(" %149[^\n]", u.morada);
 
    /* Gerar ID unico — sera a senha do utilizador */
    u.id = GerarIDUtilizador(*utilizadores);
    strcpy(u.perfil, "utilizador");
 
    *utilizadores = AdicionarUtilizador(*utilizadores, u);
 
    printf("\n========================================\n");
    printf("  Conta criada com sucesso!\n");
    printf("  Nome : %s\n", u.nome);
    printf("  O seu ID (guarde, e a sua senha): %d\n", u.id);
    printf("  Guarde este numero - e necessario para entrar.\n");
    printf("========================================\n\n");
 
    return PesquisarUtilizadorPorNomeID(*utilizadores, u.nome, u.id);
}
 
/* =========================================================
   RECOLHA DE DADOS DO DENUNCIANTE (identificado)
   ========================================================= */
static void PreencherDadosDenunciante(Denuncia *d) {
 
    /* Nome: so letras e espacos */
    do {
        printf("  Nome completo : ");
        scanf(" %99[^\n]", d->nome_denunciante);
        if (!NomeValido(d->nome_denunciante))
            printf("  Nome invalido. Sem numeros ou simbolos.\n");
    } while (!NomeValido(d->nome_denunciante));
 
    /* Telefone: exactamente 9 digitos */
    do {
        printf("  Numero de telefone (9 digitos): ");
        scanf(" %9s", d->telefone);
        if (!TelefoneValido(d->telefone))
            printf("  Telefone invalido. Insira exactamente 9 digitos numericos.\n");
    } while (!TelefoneValido(d->telefone));
 
    /* Morada: aceita letras e numeros */
    printf("  Morada: ");
    scanf(" %149[^\n]", d->morada);
}
 
/* =========================================================
   FLUXO DE REGISTO DE DENUNCIA (partilhado entre utilizador e admin)
   ========================================================= */
static void FluxoRegistarDenuncia (BTree *bt, Grafo *g, Historico **hist, int is_admin, NoUtilizador *utilizador_logado){
    Denuncia d;
    char data_auto[11];
    int i, op_zona;
 
    memset(&d, 0, sizeof(d));
 
    /* Data gerada automaticamente pelo sistema */
    ObterDataAtual(data_auto, sizeof(data_auto));
    strcpy(d.data, data_auto);
    printf("Data registada automaticamente: %s\n", d.data);
 
    /* --- TIPO DE VIOLENCIA (obrigatorio, aparece antes da descricao) ---
       O utilizador escolhe de uma lista pre-definida. O tipo escolhido
       sera usado pelo grafo para criar conexoes automaticas entre zonas
       que registem o mesmo tipo de violencia.                            */
    {
        int op_tipo = 0;
        const char *tipos[] = {
            "Maus-tratos fisicos",
            "Abuso psicologico",
            "Negligencia",
            "Abuso sexual",
            "Trabalho infantil",
            "Abandono",
            "Outro"
        };
        int n_tipos = 7, t;
 
        do {
            printf("\nTipo de violencia sofrida pela crianca:\n");
            for (t = 0; t < n_tipos; t++)
                printf("  %d - %s\n", t + 1, tipos[t]);
            printf("Escolha (1-%d): ", n_tipos);
 
            if (!LerInteiro(&op_tipo) || op_tipo < 1 || op_tipo > n_tipos) {
                printf("  Opcao invalida. Tente novamente.\n");
                op_tipo = 0;
            }
        } while (op_tipo < 1 || op_tipo > n_tipos);
 
        strncpy(d.tipo_violencia, tipos[op_tipo - 1], sizeof(d.tipo_violencia) - 1);
        d.tipo_violencia[sizeof(d.tipo_violencia) - 1] = '\0';
        printf("  Tipo registado: %s\n", d.tipo_violencia);
    }
 
    /* Descricao obrigatoria */
    do {
        printf("Descricao do ocorrido (obrigatorio): ");
        scanf(" %255[^\n]", d.descricao);
        if (strlen(d.descricao) == 0)
            printf("  A descricao nao pode estar vazia.\n");
    } while (strlen(d.descricao) == 0);
 
    /* Zona: seleccao por lista numerada */
    d.zona[0] = '\0';
    do {
        printf("\nZonas disponiveis:\n");
        for (i = 0; i < g->n_zonas; i++)
            printf("  %d - %s\n", i + 1, g->zonas[i].nome);
        printf("Escolha o numero da zona: ");
 
        if (!LerInteiro(&op_zona) || op_zona < 1 || op_zona > g->n_zonas)
            printf("  Zona invalida.\n");
        else
            strcpy(d.zona, g->zonas[op_zona - 1].nome);
    } while (strlen(d.zona) == 0);
 
    /* Gravidade: todos os perfis definem (1 a 5) */
    d.gravidade = 0;
    do {
        printf("Gravidade do caso (1=baixa ... 5=muito grave): ");
        if (!LerInteiro(&d.gravidade) || d.gravidade < 1 || d.gravidade > 5)
            printf("  Valor invalido. Insira entre 1 e 5.\n");
    } while (d.gravidade < 1 || d.gravidade > 5);
 
    /* Identificacao opcional para utilizador comum */
    if (!is_admin && utilizador_logado != NULL) {
    d.id_utilizador = utilizador_logado->dados.id;
    strcpy(d.nome_denunciante, utilizador_logado->dados.nome);
    strcpy(d.telefone, utilizador_logado->dados.telefone);
    strcpy(d.morada, utilizador_logado->dados.morada);
    }
 
    strcpy(d.estado, "pendente");
 
    /* Gerar ID aleatorio unico de 4 digitos */
    d.id = GerarIDUnico(bt);
 
    RegistarDenuncia(bt, g, hist, d, is_admin);
 
    printf("\n  GUARDE O SEU ID DE DENUNCIA: %d\n", d.id);
    printf("  (necessario para consultar o estado)\n\n");
}
 
/* =========================================================
   AREA DO UTILIZADOR (com ou sem conta)
   ========================================================= */
static void MenuUtilizador(BTree *bt, Grafo *g, Historico **hist,
                            NoUtilizador **utilizadores) {
    int op;
 
    do {
        printf("\n--- AREA DO UTILIZADOR ---\n");
        printf("1. Entrar com a minha conta\n");
        printf("2. Criar conta\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
 
        if (!LerInteiro(&op)) continue;
 
        switch (op) {
 
            /* Utilizador com conta registada */
            case 1: {
                NoUtilizador *u = AutenticarUtilizador(*utilizadores);
                if (u == NULL) break;
 
                int op2;
                do {
                    printf("\n--- MEU PERFIL (%s) ---\n", u->dados.nome);
                    printf("1. Fazer denuncia\n");
                    printf("2. Ver estado de uma denuncia\n");
                    printf("3. Ver historico de uma denuncia\n");
                    printf("0. Sair do perfil\n");
                    printf("Opcao: ");
 
                    if (!LerInteiro(&op2)) continue;
 
                    switch (op2) {
                        case 1:
                            FluxoRegistarDenuncia(bt, g, hist, 0, u);
                            break;
 
                        case 2: {
                            int id;
                            printf("ID da denuncia: ");
                            if (!LerInteiro(&id)) break;
                            Denuncia *d = PesquisarNaBTree(bt, id);
                            if (d) {
                                printf("\n--- DENUNCIA %d ---\n", d->id);
                                printf("Tipo Violencia: %s\n", d->tipo_violencia);
                                printf("Descricao     : %s\n", d->descricao);
                                printf("Zona          : %s\n", d->zona);
                                printf("Data          : %s\n", d->data);
                                printf("Estado        : %s\n", d->estado);
                            } else {
                                printf("Denuncia %d nao encontrada.\n", id);
                            }
                            break;
                        }
 
                        case 3: {
                            int id;
                            printf("ID da denuncia: ");
                            if (!LerInteiro(&id)) break;
                            ImprimirHistorico(*hist, id);
                            break;
                        }
 
                        case 0: break;
                        default: printf("Opcao invalida.\n");
                    }
                } while (op2 != 0);
                break;
            }
 
            /* Criar nova conta de utilizador */
            case 2:
                RegistarNovoUtilizador(utilizadores);
                break;
 
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}
 
/* =========================================================
   GESTAO DE UTILIZADORES (so para administrador)
   ========================================================= */
static void MenuGerirUtilizadores(NoUtilizador **utilizadores) {
    int op;
 
    do {
        printf("\n--- GERIR UTILIZADORES ---\n");
        printf("1. Listar utilizadores\n");
        printf("2. Adicionar administrador\n");
        printf("3. Remover utilizador\n");
        printf("0. Voltar\n");
        printf("Opcao: ");
 
        if (!LerInteiro(&op)) continue;
 
        switch (op) {
 
            case 1:
                ImprimirUtilizador(*utilizadores);
                break;
 
            case 2: {
                Utilizador u;
                memset(&u, 0, sizeof(u));
                u.id = GerarIDUtilizador(*utilizadores);
                strcpy(u.perfil, "admin");
 
                do {
                    printf("Nome (so letras): ");
                    scanf(" %99[^\n]", u.nome);
                    if (!NomeValido(u.nome))
                        printf("  Nome invalido.\n");
                } while (!NomeValido(u.nome));
 
                printf("Senha: ");
                scanf(" %49[^\n]", u.senha);
                printf("Telefone de contacto: ");
                scanf(" %9s", u.telefone);
 
                *utilizadores = AdicionarUtilizador(*utilizadores, u);
                printf("Admin '%s' criado (ID: %d).\n", u.nome, u.id);
                break;
            }
 
            case 3: {
                int id;
                ImprimirUtilizador(*utilizadores);
                printf("ID a remover: ");
                if (!LerInteiro(&id)) break;
                *utilizadores = RemoverUtilizador(*utilizadores, id);
                break;
            }
 
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}
 
/* =========================================================
   MENU DO ADMINISTRADOR
   ========================================================= */
static void MenuAdmin(BTree *bt, Grafo *g, Historico **hist,
                      NoUtilizador **utilizadores) {
    int op;
 
    do {
        printf("\n--- MENU ADMINISTRADOR ---\n");
        printf("1.  Registar Denuncia\n");
        printf("2.  Pesquisar Denuncia por ID\n");
        printf("3.  Actualizar Estado do Caso\n");
        printf("4.  Ver Zonas Criticas\n");
        printf("5.  Ver Conexoes entre Zonas\n");
        printf("6.  Ver Historico de uma Denuncia\n");
        printf("7.  Remover Denuncia Resolvida\n");
        printf("8.  Gerir Utilizadores\n");
        printf("9.  Listar todas as Denuncias\n");
        printf("10. Ver Historico Geral\n");
        printf("11. Gerir Zonas e Conexoes\n");
        printf("0.  Sair\n");
        printf("Opcao: ");
 
        if (!LerInteiro(&op)) continue;
 
        switch (op) {
 
            case 1:
                FluxoRegistarDenuncia(bt, g, hist, 1,NULL);
                break;
 
            case 2: {
                int id;
                printf("ID da denuncia: ");
                if (!LerInteiro(&id)) break;
                Denuncia *d = PesquisarNaBTree(bt, id);
                if (d) {
                    printf("\n--- DENUNCIA %d ---\n", d->id);
                    printf("Tipo Violencia: %s\n", d->tipo_violencia);
                    printf("Descricao     : %s\n", d->descricao);
                    printf("Zona          : %s\n", d->zona);
                    printf("Gravidade     : %d\n", d->gravidade);
                    printf("Data          : %s\n", d->data);
                    printf("Estado        : %s\n", d->estado);
                    printf("Denunciante   : %s\n", d->nome_denunciante);
                    if (strcmp(d->nome_denunciante, "Anonimo") != 0) {
                        printf("Telefone      : %s\n", d->telefone);
                        printf("Morada        : %s\n", d->morada);
                    }
                } else {
                    printf("Denuncia %d nao encontrada.\n", id);
                }
                break;
            }
 
            case 3: {
                int id, op_estado;
                printf("ID da denuncia: ");
                if (!LerInteiro(&id)) break;
 
                Denuncia *d = PesquisarNaBTree(bt, id);
                if (!d) { printf("Denuncia %d nao encontrada.\n", id); break; }
 
                printf("Estado actual: %s\n", d->estado);
                printf("  1 - pendente\n");
                printf("  2 - em_analise\n");
                printf("  3 - resolvida\n");
                printf("  4 - arquivada\n");
                printf("Novo estado (1-4): ");
                if (!LerInteiro(&op_estado)) break;
 
                const char *novo = NumeroParaEstado(op_estado);
                if (!novo) { printf("Opcao invalida.\n"); break; }
 
                AdicionarHistorico(hist, id, d->estado, novo, d->data);
                strcpy(d->estado, novo);
                printf("Estado actualizado para '%s'.\n", novo);
                break;
            }
 
            case 4: ZonasCriticas(g); break;
            case 5: MenuConexoes(g); break;
 
            case 6: {
                int id;
                printf("ID da denuncia: ");
                if (!LerInteiro(&id)) break;
                ImprimirHistorico(*hist, id);
                break;
            }
 
            case 7: {
                int id;
                ListarDenunciasResolvidas(bt);
                printf("\nID a remover (0 para cancelar): ");
                if (!LerInteiro(&id) || id == 0) { printf("Cancelado.\n"); break; }
 
                Denuncia *d = PesquisarNaBTree(bt, id);
                if (!d) { printf("Denuncia %d nao encontrada.\n", id); break; }
 
                if (strcmp(d->estado, "resolvida") != 0) {
                    int conf;
                    printf("Estado e '%s'. Confirma remocao? (1-Sim / 0-Nao): ", d->estado);
                    if (!LerInteiro(&conf) || conf != 1) { printf("Cancelado.\n"); break; }
                }
                RemoverNaBTree(bt, id);
                break;
            }
 
            case 8:  MenuGerirUtilizadores(utilizadores); break;
            case 9:  ImprimirBTree(bt); break;
            case 10: ImprimirHistoricoGeral(*hist); break;
            case 11: MenuGerirZonas(g); break;
 
            case 0: break;
            default: printf("Opcao invalida.\n");
        }
    } while (op != 0);
}
 
/* =========================================================
   MENU PRINCIPAL — escolha entre utilizador e administrador
   ========================================================= */
void MenuExecutar(BTree *bt, Grafo *g, Historico **hist,
                  NoUtilizador **utilizadores) {
    int op;
 
    do {
        printf("\n========================================\n");
        printf("  SISTEMA DE DENUNCIA E PROTECAO INFANTIL\n");
        printf("========================================\n");
        printf("1. Area do Utilizador\n");
        printf("2. Area do Administrador\n");
        printf("0. Sair\n");
        printf("Opcao: ");
 
        if (!LerInteiro(&op)) continue;
 
        switch (op) {
 
            case 1:
                MenuUtilizador(bt, g, hist, utilizadores);
                break;
 
            case 2:
                if (AutenticarAdmin(*utilizadores))
                    MenuAdmin(bt, g, hist, utilizadores);
                break;
 
            case 0:
                printf("A guardar dados...\n");
                GuardarDenuncias(bt,               "denuncias.txt");
                GuardarZonas(g,                    "zonas.txt");
                GuardarConexoes(g,                 "conexoes.txt");
                GuardarUtilizadores(*utilizadores, "utilizadores.txt");
                GuardarHistorico(*hist,            "historico.txt");
                printf("Dados guardados. Ate logo!\n");
                break;
 
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);
}
