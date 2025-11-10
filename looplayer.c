#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int inserir_musica(LoopPlayer* player, const char* titulo) {
    Musica* nova = (Musica*)malloc(sizeof(Musica));
    
    nova->titulo = (char*)malloc(strlen(titulo) + 1);
    strcpy(nova->titulo, titulo);
    
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
        printf("%d. %s", indice++, atual->titulo);
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
