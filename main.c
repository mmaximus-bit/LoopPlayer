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
    printf("1. Inserir música\n");
    printf("2. Próxima música\n");
    printf("3. Música anterior\n");
    printf("4. Listar músicas\n");
    printf("5. Buscar música\n");
    printf("0. Sair\n");
    printf("\nEscolha uma opção: ");
}

int main() {
    LoopPlayer* player = criar_player();
    int opcao;
    char titulo[100];
    char artista[100];

    do {
        limpar_tela();
        if (!esta_vazia(player)) {
            Musica* atual = obter_musica_atual(player);
            printf("\nMúsica atual: %s - %s\n", atual->titulo, atual->artista);
        }
        exibir_menu();
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer

        switch(opcao) {
            case 1:
                printf("Digite o título da música: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = 0; // Remover o \n
                printf("Digite o artista: ");
                fgets(artista, sizeof(artista), stdin);
                artista[strcspn(artista, "\n")] = 0;

                if (inserir_musica(player, titulo, artista)) {
                    printf("Música '%s' - '%s' adicionada com sucesso!\n", titulo, artista);
                }
                break;

            case 2:
                if (proxima_musica(player)) {
                    {
                        Musica* atual = obter_musica_atual(player);
                        if (atual)
                            printf("Navegando para: '%s' - '%s'...\n", atual->titulo, atual->artista);
                        else
                            printf("Próxima música selecionada.\n");
                    }
                } else {
                    printf("Playlist vazia!\n");
                }
                break;

            case 3:
                if (musica_anterior(player)) {
                    {
                        Musica* atual = obter_musica_atual(player);
                        if (atual)
                            printf("Navegando para: '%s' - '%s'...\n", atual->titulo, atual->artista);
                        else
                            printf("Música anterior selecionada.\n");
                    }
                } else {
                    printf("Playlist vazia!\n");
                }
                break;

            case 4:
                printf("\nPlaylist completa:\n");
                listar_musicas(player);
                printf("\nPressione ENTER para continuar...");
                getchar();
                break;

            case 5: {
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

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
        }

        if (opcao != 0 && opcao != 4) {
            printf("Pressione ENTER para continuar...");
            getchar();
        }

    } while (opcao != 0);

    return 0;
}
