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

void pressione_enter_para_continuar() {
    printf("\n\n--- Pressione ENTER para continuar ---");
    getchar();
}

void exibir_menu() {
    printf("\n");
    printf("╔════════════════════════════════════╗\n");
    printf("║      🎵 LoopPlayer - Menu 🎵      ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║ 1.  Inserir música no final        ║\n");
    printf("║ 2.  Inserir em posição específica  ║\n");
    printf("║ 3.  Remover música por posição    ║\n");
    printf("║ 4.  Próxima música                 ║\n");
    printf("║ 5.  Música anterior                ║\n");
    printf("║ 6.  Listar músicas                 ║\n");
    printf("║ 7.  Buscar música                  ║\n");
    printf("║ 8.  Mover música                   ║\n");
    printf("║ 9.  Voltar pelo histórico          ║\n");
    printf("║ 10. Embaralhar playlist (Shuffle)  ║\n");
    printf("║ 11. Avançar X músicas              ║\n");
    printf("║ 12. Retroceder X músicas           ║\n");
    printf("║ 0.  Sair                           ║\n");
    printf("╚════════════════════════════════════╝\n");
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
        
        // Exibir status do player
        printf("\n╔════════════════════════════════════╗\n");
        printf("║        STATUS DO PLAYER            ║\n");
        printf("╠════════════════════════════════════╣\n");
        if (!esta_vazia(player)) {
            Musica* atual = obter_musica_atual(player);
            printf("║ Atual: %-30s ║\n", atual->titulo);
            printf("║ Artista: %-28s ║\n", atual->artista);
            printf("║ Total de músicas: %-16d ║\n", player->quantidade);
        } else {
            printf("║ 🎵 Sua playlist está vazia!       ║\n");
        }
        printf("╚════════════════════════════════════╝\n");
        
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
                pressione_enter_para_continuar();
                break;

            case 7: {
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                    pressione_enter_para_continuar();
                    break;
                }
                char termo[100];
                printf("Digite o nome da musica: ");
                fgets(termo, sizeof(termo), stdin);
                termo[strcspn(termo, "\n")] = 0;
                printf("\nResultados da busca:\n");
                buscar_musicas(player, termo);
                pressione_enter_para_continuar();
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
                pressione_enter_para_continuar();
                break;
            }

            case 11: {
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                } else {
                    int quantidade;
                    printf("Quantas músicas deseja avançar? (1 a %d): ", player->quantidade);
                    scanf("%d", &quantidade);
                    getchar();
                    
                    if (avancar_musicas(player, quantidade)) {
                        Musica* atual = obter_musica_atual(player);
                        if (atual) {
                            printf("\n✓ Avançou %d música(s)!\n", 
                                   (quantidade > player->quantidade) ? player->quantidade : quantidade);
                            printf("Agora tocando: '%s' - '%s'\n", atual->titulo, atual->artista);
                        }
                    } else {
                        printf("Erro ao avançar! Verifique a quantidade.\n");
                    }
                }
                break;
            }

            case 12: {
                if (esta_vazia(player)) {
                    printf("\n🎵 Sua playlist está vazia! Adicione algumas músicas primeiro.\n");
                } else {
                    int quantidade;
                    printf("Quantas músicas deseja retroceder? (1 a %d): ", player->quantidade);
                    scanf("%d", &quantidade);
                    getchar();
                    
                    if (retroceder_musicas(player, quantidade)) {
                        Musica* atual = obter_musica_atual(player);
                        if (atual) {
                            printf("\n✓ Retrocedeu %d música(s)!\n", 
                                   (quantidade > player->quantidade) ? player->quantidade : quantidade);
                            printf("Agora tocando: '%s' - '%s'\n", atual->titulo, atual->artista);
                        }
                    } else {
                        printf("Erro ao retroceder! Verifique a quantidade.\n");
                    }
                }
                break;
            }

            case 0:
                printf("\n╔════════════════════════════════════╗\n");
                printf("║   Liberando memória...             ║\n");
                liberar_player(player);
                printf("║   Memória liberada com sucesso!    ║\n");
                printf("║   Saindo... Até mais! 🎵           ║\n");
                printf("╚════════════════════════════════════╝\n\n");
                break;

            default:
                printf("Opção inválida!\n");
                pressione_enter_para_continuar();
        }

    } while (opcao != 0);

    return 0;
}
