#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "looplayer.h"

// Criar um novo player
LoopPlayer* criar_player() {
    LoopPlayer* player = (LoopPlayer*)malloc(sizeof(LoopPlayer));
    
    player->cabeca = NULL;
    player->atual = NULL;
    player->quantidade = 0;
    
    return player;
}

// Inserir música no final da lista
int inserir_musica(LoopPlayer* player, const char* titulo, const char* artista) {
    Musica* nova = (Musica*)malloc(sizeof(Musica));

    nova->titulo = (char*)malloc(strlen(titulo) + 1);
    strcpy(nova->titulo, titulo);

    nova->artista = (char*)malloc(strlen(artista) + 1);
    strcpy(nova->artista, artista);
    
    // Se é a primeira música
    if (player->cabeca == NULL) {
        player->cabeca = nova;
        player->atual = nova;
        nova->anterior = nova;
        nova->proxima = nova;
    } else {
        // Inserir no final (antes da cabeça)
        nova->anterior = player->cabeca->anterior;
        nova->proxima = player->cabeca;
        player->cabeca->anterior->proxima = nova;
        player->cabeca->anterior = nova;
    }
    
    player->quantidade++;
    return 1;
}

// Verificar se a lista está vazia
int esta_vazia(LoopPlayer* player) {
    return (player->cabeca == NULL);
}

// Listar todas as músicas
void listar_musicas(LoopPlayer* player) {
    if (esta_vazia(player)) {
        printf("Playlist vazia!\n");
        return;
    }
    
    Musica* atual = player->cabeca;
    int indice = 1;
    
    do {
        printf("%d. %s - %s", indice++, atual->titulo, atual->artista);
        if (atual == player->atual) {
            printf(" (atual)");
        }
        printf("\n");
        atual = atual->proxima;
    } while (atual != player->cabeca);
}

// Ir para próxima música
int proxima_musica(LoopPlayer* player) {
    if (esta_vazia(player)) {
        return 0;
    }
    player->atual = player->atual->proxima;
    return 1;
}

// Ir para música anterior
int musica_anterior(LoopPlayer* player) {
    if (esta_vazia(player)) {
        return 0;
    }
    player->atual = player->atual->anterior;
    return 1;
}

// Obter música atual
Musica* obter_musica_atual(LoopPlayer* player) {
    if (esta_vazia(player)) {
        return NULL;
    }
    return player->atual;
}

// Inserir música em uma posição específica
int inserir_na_posicao(LoopPlayer* player, const char* titulo, const char* artista, int posicao) {
    if (player == NULL || titulo == NULL || artista == NULL || posicao < 1) {
        return 0;
    }
    
    // Se posição for 1 ou lista vazia, inserir no início
    if (posicao == 1 || esta_vazia(player)) {
        Musica* nova = (Musica*)malloc(sizeof(Musica));
        nova->titulo = (char*)malloc(strlen(titulo) + 1);
        strcpy(nova->titulo, titulo);
        nova->artista = (char*)malloc(strlen(artista) + 1);
        strcpy(nova->artista, artista);
        
        if (esta_vazia(player)) {
            player->cabeca = nova;
            player->atual = nova;
            nova->anterior = nova;
            nova->proxima = nova;
            player->quantidade++;
            return 1;
        }
        
        // Inserir no início
        nova->proxima = player->cabeca;
        nova->anterior = player->cabeca->anterior;
        player->cabeca->anterior->proxima = nova;
        player->cabeca->anterior = nova;
        player->cabeca = nova;
        player->quantidade++;
        return 1;
    }
    
    // Navegar até a posição desejada
    Musica* atual = player->cabeca;
    int pos_atual = 1;
    
    while (pos_atual < posicao - 1 && atual->proxima != player->cabeca) {
        atual = atual->proxima;
        pos_atual++;
    }
    
    // Criar nova música
    Musica* nova = (Musica*)malloc(sizeof(Musica));
    nova->titulo = (char*)malloc(strlen(titulo) + 1);
    strcpy(nova->titulo, titulo);
    nova->artista = (char*)malloc(strlen(artista) + 1);
    strcpy(nova->artista, artista);
    
    // Inserir após a posição atual
    nova->proxima = atual->proxima;
    nova->anterior = atual;
    atual->proxima->anterior = nova;
    atual->proxima = nova;
    
    player->quantidade++;
    return 1;
}

// Remover música de uma posição específica
int remover_da_posicao(LoopPlayer* player, int posicao) {
    if (esta_vazia(player) || posicao < 1 || posicao > player->quantidade) {
        return 0;
    }
    
    Musica* atual = player->cabeca;
    int pos_atual = 1;
    
    // Navegar até a posição
    while (pos_atual < posicao) {
        atual = atual->proxima;
        pos_atual++;
    }
    
    // Se é o único elemento
    if (player->quantidade == 1) {
        free(atual->titulo);
        free(atual->artista);
        free(atual);
        player->cabeca = NULL;
        player->atual = NULL;
        player->quantidade = 0;
        return 1;
    }
    
    // Ajustar ponteiros dos vizinhos
    atual->anterior->proxima = atual->proxima;
    atual->proxima->anterior = atual->anterior;
    
    // Se estamos removendo a cabeça
    if (atual == player->cabeca) {
        player->cabeca = atual->proxima;
    }
    
    // Se estamos removendo a música atual
    if (atual == player->atual) {
        player->atual = atual->proxima;
    }
    
    free(atual->titulo);
    free(atual->artista);
    free(atual);
    player->quantidade--;
    return 1;
}

// Função auxiliar para converter string para minúsculas
static void str_to_lower(const char* src, char* dst, size_t dst_size) {
    size_t i;
    if (dst_size == 0) return;
    for (i = 0; i < dst_size - 1 && src[i] != '\0'; ++i) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

// Buscar músicas por termo (busca parcial, case-insensitive)
void buscar_musicas(LoopPlayer* player, const char* termo) {
    if (esta_vazia(player)) {
        printf("Playlist vazia!\n");
        return;
    }

    char termo_lower[256];
    str_to_lower(termo, termo_lower, sizeof(termo_lower));

    Musica* atual = player->cabeca;
    int indice = 1;
    int encontrados = 0;

    do {
        char titulo_lower[512];
        char artista_lower[512];
        str_to_lower(atual->titulo, titulo_lower, sizeof(titulo_lower));
        str_to_lower(atual->artista, artista_lower, sizeof(artista_lower));

        if (strstr(titulo_lower, termo_lower) != NULL || strstr(artista_lower, termo_lower) != NULL) {
            printf("%d. %s - %s", indice, atual->titulo, atual->artista);
            if (atual == player->atual) {
                printf(" (atual)");
            }
            printf("\n");
            encontrados++;
        }

        atual = atual->proxima;
        indice++;
    } while (atual != player->cabeca);

    if (encontrados == 0) {
        printf("Nenhuma música encontrada\n");
    }
}
