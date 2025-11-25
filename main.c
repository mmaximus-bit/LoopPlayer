#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "looplayer.h"

void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void exibir_menu() {
    printf("\n=== LoopPlayer Menu ===\n");
    printf("1. Inserir música no final\n");
    printf("2. Inserir música em posição específica\n");
    printf("3. Remover música por posição\n");
    printf("4. Próxima música\n");
    printf("5. Música anterior\n");
    printf("6. Listar músicas\n");
    printf("0. Sair\n");
    printf("\nEscolha uma opção: ");
}

int main() {
    LoopPlayer* player = criar_player();
    int opcao;
    char titulo[100];
    int posicao;

    do {
        limpar_tela();
        if (!esta_vazia(player)) {
            Musica* atual = obter_musica_atual(player);
            printf("\nMúsica atual: %s\n", atual->titulo);
            printf("Total de músicas: %d\n", player->quantidade);
        }
        exibir_menu();
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer

        switch(opcao) {
            case 1:
                printf("Digite o título da música: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = 0;
                if (inserir_musica(player, titulo)) {
                    printf("Música inserida com sucesso!\n");
                }
                break;

            case 2:
                printf("Digite o título da música: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = 0;
                printf("Digite a posição (1 a %d): ", player->quantidade + 1);
                scanf("%d", &posicao);
                getchar();
                if (inserir_na_posicao(player, titulo, posicao)) {
                    printf("Música inserida na posição %d com sucesso!\n", posicao);
                } else {
                    printf("Erro ao inserir música!\n");
                }
                break;

            case 3:
                if (esta_vazia(player)) {
                    printf("Playlist vazia!\n");
                } else {
                    printf("Digite a posição da música a remover (1 a %d): ", player->quantidade);
                    scanf("%d", &posicao);
                    getchar();
                    if (remover_da_posicao(player, posicao)) {
                        printf("Música da posição %d removida com sucesso!\n", posicao);
                    } else {
                        printf("Erro ao remover música! Posição inválida.\n");
                    }
                }
                break;

            case 4:
                if (proxima_musica(player)) {
                    printf("Próxima música selecionada.\n");
                } else {
                    printf("Playlist vazia!\n");
                }
                break;

            case 5:
                if (musica_anterior(player)) {
                    printf("Música anterior selecionada.\n");
                } else {
                    printf("Playlist vazia!\n");
                }
                break;

            case 6:
                printf("\nPlaylist completa:\n");
                listar_musicas(player);
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

        if (opcao != 0 && opcao != 6) {
            printf("Pressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
