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
    printf("7. Buscar música\n");
    printf("8. Mover música\n");
    printf("9. Voltar pelo histórico\n");
    printf("10. Embaralhar playlist (Shuffle)\n");
    printf("0. Sair\n");
    printf("\nEscolha uma opção: ");
}

int main() {
    LoopPlayer* player = criar_player();
    int opcao;
    char titulo[100];
    char artista[100];
    int posicao;

    do {
        limpar_tela();
        if (!esta_vazia(player)) {
            Musica* atual = obter_musica_atual(player);
            printf("\nMúsica atual: %s - %s\n", atual->titulo, atual->artista);
            printf("Total de músicas: %d\n", player->quantidade);
        }
        exibir_menu();
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1:
                printf("Digite o título da música: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = 0;
                printf("Digite o artista: ");
                fgets(artista, sizeof(artista), stdin);
                artista[strcspn(artista, "\n")] = 0;
                if (inserir_musica(player, titulo, artista)) {
                    printf("Música '%s' - '%s' adicionada com sucesso!\n", titulo, artista);
                }
                break;

            case 2:
                printf("Digite o título da música: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = 0;
                printf("Digite o artista: ");
                fgets(artista, sizeof(artista), stdin);
                artista[strcspn(artista, "\n")] = 0;
                printf("Digite a posição (1 a %d): ", player->quantidade + 1);
                scanf("%d", &posicao);
                getchar();
                if (inserir_na_posicao(player, titulo, artista, posicao)) {
                    printf("Música inserida na posição %d com sucesso!\n", posicao);
                } else {
                    printf("Erro ao inserir música!\n");
                }
                break;

            case 3:
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
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
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                } else if (proxima_musica(player)) {
                    Musica* atual = obter_musica_atual(player);
                    if (atual)
                        printf("Navegando para: '%s' - '%s'...\n", atual->titulo, atual->artista);
                    else
                        printf("Próxima música selecionada.\n");
                }
                break;

            case 5:
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                } else if (musica_anterior(player)) {
                    Musica* atual = obter_musica_atual(player);
                    if (atual)
                        printf("Navegando para: '%s' - '%s'...\n", atual->titulo, atual->artista);
                    else
                        printf("Música anterior selecionada.\n");
                }
                break;

            case 6:
                printf("\nPlaylist completa:\n");
                listar_musicas(player);
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;

            case 7: {
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                    printf("\nPressione ENTER para continuar...");
                    getchar();
                    break;
                }
                char termo[100];
                printf("Digite o nome da musica: ");
                fgets(termo, sizeof(termo), stdin);
                termo[strcspn(termo, "\n")] = 0;
                printf("\nResultados da busca:\n");
                buscar_musicas(player, termo);
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
            }

            case 8: {
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                } else if (player->quantidade < 2) {
                    printf("É necessário ter pelo menos 2 músicas para mover!\n");
                } else {
                    int pos_origem, pos_destino;
                    printf("\nPlaylist atual:\n");
                    listar_musicas(player);
                    printf("\nDigite a posição da música a mover (1 a %d): ", player->quantidade);
                    scanf("%d", &pos_origem);
                    getchar();
                    printf("Digite a posição de destino (1 a %d): ", player->quantidade);
                    scanf("%d", &pos_destino);
                    getchar();
                    
                    if (mover_musica(player, pos_origem, pos_destino)) {
                        printf("Música movida da posição %d para %d com sucesso!\n", pos_origem, pos_destino);
                    } else {
                        printf("Erro ao mover música! Verifique as posições.\n");
                    }
                }
                break;
            }

            case 9: {
                if (voltar_historico(player)) {
                    Musica* atual = obter_musica_atual(player);
                    if (atual) {
                        printf("Voltando para: '%s' - '%s'\n", atual->titulo, atual->artista);
                    }
                } else {
                    printf("Histórico vazio! Nenhuma música anterior para voltar.\n");
                }
                break;
            }

            case 10: {
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                } else if (player->quantidade < 2) {
                    printf("É necessário ter pelo menos 2 músicas para embaralhar!\n");
                } else {
                    printf("\nPlaylist antes do shuffle:\n");
                    listar_musicas(player);
                    
                    if (embaralhar_playlist(player)) {
                        printf("\n*** Playlist embaralhada com sucesso! ***\n");
                        printf("\nNova ordem:\n");
                        listar_musicas(player);
                    } else {
                        printf("Erro ao embaralhar a playlist!\n");
                    }
                }
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;
            }

            case 0:
                printf("Liberando memória...\n");
                liberar_player(player);
                printf("Memória liberada com sucesso!\n");
                printf("Saindo... Até mais! 🎵\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

        if (opcao != 0 && opcao != 6 && opcao != 7 && opcao != 8 && opcao != 9 && opcao != 10) {
            printf("Pressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
