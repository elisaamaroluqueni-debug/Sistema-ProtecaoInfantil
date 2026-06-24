#include <stdio.h>
#include <stdlib.h>
#include "Sistema.h"

//Função para executar o menu
void MenuExecutar(BTree *bt, Grafo *g, Historico **hist, NoUtilizador **utilizadores)
{
    int op;

    do {
        printf("\nSISTEMA DE DENUNCIA E PROTECAO INFANTIL\n");
        printf("1. Registar Denuncia\n");
        printf("2. Pesquisar Denuncia por Id\n");
        printf("3. Actualizar o estado do caso\n");
        printf("4. Ver Zonas Criticas\n");
        printf("5. Ver conexoes entre casos \n");
        printf("6. Ver Historico de uma Zona \n");
        printf("7. Resolver Denuncia Resolvida \n");
        printf("8. Gerir utilizadores \n");
        printf("0. Sair \n");
        printf("Opcao:\n");

        scanf("%d", &op);

        switch (op)
        {
            case 1:
            {
            	Denuncia d;
            	printf("ID:");
            	scanf("%d", &d.id);
            	printf("Descricao:");
            	scanf("%[^\n]", &d.descricao);
            	printf("Zona:");
            	scanf("%[^\n]", &d.zona);
            	printf("Gravidade (1-5):");
            	scanf("%d", &d.gravidade);
            	printf("Data (DD/MM/AAAA):");
                scanf("%[^\n]", &d.data);
                strcpy(d.estado, "pendente");
                
                RegistarDenuncia(bt, g, hist, d);
           	 break;
			}
             

            case 2:
            {
            	int id;
            	printf("ID da denuncia: ");
            	scanf("%d", &id);
            	
            	Denuncia *d = PesquisarNaBTree(bt, id);
            	if(d!= NULL){
            		printf("\n--- DENUNCIA %d ---\n", d->id);
                    printf("Descricao: %s\n", d->descricao);
                    printf("Zona     : %s\n", d->zona);
                    printf("Gravidade: %d\n", d->gravidade);
                    printf("Data     : %s\n", d->data);
                    printf("Estado   : %s\n", d->estado);
				}
				else
				printf("Denuncia %d nao encontrada \n", id);
            	break;
			}  

            case 3:
            {
            	int id;
            	char novo_estado[20];
            	  printf("ID da denuncia: ");
                scanf("%d", &id);
 
                Denuncia *d = PesquisarNaBTree(bt, id);
                if (d != NULL)
                {
                    printf("Estado actual: %s\n", d->estado);
                    printf("Novo estado (pendente/em_analise/resolvida/arquivada): ");
                    scanf(" %[^\n]", novo_estado);
                    AdicionarHistorico(hist, id, d->estado, novo_estado, d->data);
                    strcpy(d->estado, novo_estado);
                    printf("Estado actualizado com sucesso.\n");
                }
                else
                    printf("Denuncia %d nao encontrada.\n", id);
            	break;
			}   

            case 4:
            	ZonasCriticas(g);
            break;
           
            case 5:
             {
                int id_zona;
                printf("ID da zona: ");
                scanf("%d", &id_zona);
                if (id_zona >= 0 && id_zona < g->n_zonas)
                {
                    Aresta *a = g->adjacentes[id_zona];
                    printf("\n=== CONEXOES DA ZONA '%s' ===\n", g->zonas[id_zona].nome);
                    if (a == NULL)
                        printf("  Sem conexoes.\n");
                    while (a != NULL)
                    {
                        printf("  -> %s (peso: %d)\n", g->zonas[a->id_destino].nome, a->peso);
                        a = a->prox;
                    }
                }
                else
                    printf("Zona invalida.\n");
                    break;
            }

            case 6:
           {
           	int id;
           	printf("ID da denuncia:  ");
           	scanf("%d", &id);
           	ImprimirHistorico(*hist, id);
           	break;
		   }

            case 7:
           {
           	int id;
           	printf("ID da denuncia:  ");
           	scanf("%d", &id);
           	RemoverNaBTree(bt, id);
           	break;
		   }

            case 8:
            	ImprimirUtilizador(*utilizadores);
                break;

            case 0:
            	printf("A guardar dados...\n");
                GuardarDenuncias(bt, "denuncias.txt");
                GuardarZonas(g, "zonas.txt");
                GuardarUtilizadores(*utilizadores, "utilizadores.txt");
                printf("Saindo do programa\n");
                break;
                
            default:
               	printf("Opcao invalida\n");
                break;
        }
    } while (op != 0);
}
