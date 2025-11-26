#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "looplayer.h"

// Criar um novo player
LoopPlayer* criar_player() {
    LoopPlayer* player = (LoopPlayer*)malloc(sizeof(LoopPlayer));
    
    player->cabeca = NULL;
    player->atual = NULL;
    player->quantidade = 0;
    player->historico = criar_pilha();
    
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
    // Empilha a música atual no histórico antes de navegar
    empilhar(player->historico, player->atual);
    player->atual = player->atual->proxima;
    return 1;
}

// Ir para música anterior
int musica_anterior(LoopPlayer* player) {
    if (esta_vazia(player)) {
        return 0;
    }
    // Empilha a música atual no histórico antes de navegar
    empilhar(player->historico, player->atual);
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

// Mover música de uma posição para outra
int mover_musica(LoopPlayer* player, int pos_origem, int pos_destino) {
    // Validações
    if (esta_vazia(player) || pos_origem < 1 || pos_origem > player->quantidade ||
        pos_destino < 1 || pos_destino > player->quantidade || pos_origem == pos_destino) {
        return 0;
    }
    
    // Se só tem uma música, não há o que mover
    if (player->quantidade == 1) {
        return 1;
    }
    
    // Encontrar o nó na posição de origem
    Musica* musica_mover = player->cabeca;
    for (int i = 1; i < pos_origem; i++) {
        musica_mover = musica_mover->proxima;
    }
    
    // PASSO 1: Desligar o nó da sua posição atual (religar vizinhos)
    musica_mover->anterior->proxima = musica_mover->proxima;
    musica_mover->proxima->anterior = musica_mover->anterior;
    
    // Se estamos movendo a cabeça, atualizar para o próximo
    if (musica_mover == player->cabeca) {
        player->cabeca = musica_mover->proxima;
    }
    
    // Ajustar pos_destino se a origem estava antes do destino
    int destino_ajustado = pos_destino;
    if (pos_origem < pos_destino) {
        destino_ajustado--;
    }
    
    // Encontrar o nó na posição de destino (após o desligamento)
    Musica* destino = player->cabeca;
    for (int i = 1; i < destino_ajustado; i++) {
        destino = destino->proxima;
    }
    
    // PASSO 2: Religar o nó na nova posição (após o nó destino)
    musica_mover->proxima = destino->proxima;
    musica_mover->anterior = destino;
    destino->proxima->anterior = musica_mover;
    destino->proxima = musica_mover;
    
    // Se o destino era posição 1, atualizar a cabeça
    if (pos_destino == 1) {
        player->cabeca = musica_mover;
    }
    
    return 1;
}

// ============ IMPLEMENTAÇÃO DA PILHA DE HISTÓRICO ============

// Criar uma nova pilha vazia
Pilha* criar_pilha() {
    Pilha* pilha = (Pilha*)malloc(sizeof(Pilha));
    pilha->topo = NULL;
    pilha->tamanho = 0;
    return pilha;
}

// Destruir a pilha e liberar memória
void destruir_pilha(Pilha* pilha) {
    if (pilha == NULL) return;
    
    NoPilha* atual = pilha->topo;
    while (atual != NULL) {
        NoPilha* proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    free(pilha);
}

// Empilhar música no topo (push)
int empilhar(Pilha* pilha, Musica* musica) {
    if (pilha == NULL || musica == NULL) {
        return 0;
    }
    
    NoPilha* novo = (NoPilha*)malloc(sizeof(NoPilha));
    if (novo == NULL) {
        return 0;
    }
    
    novo->musica = musica;
    novo->proximo = pilha->topo;
    pilha->topo = novo;
    pilha->tamanho++;
    
    return 1;
}

// Desempilhar e retornar música do topo (pop)
Musica* desempilhar(Pilha* pilha) {
    if (pilha == NULL || pilha->topo == NULL) {
        return NULL;
    }
    
    NoPilha* topo = pilha->topo;
    Musica* musica = topo->musica;
    pilha->topo = topo->proximo;
    pilha->tamanho--;
    
    free(topo);
    return musica;
}

// Verificar se a pilha está vazia
int pilha_vazia(Pilha* pilha) {
    return (pilha == NULL || pilha->topo == NULL);
}

// Voltar para a música anterior do histórico
int voltar_historico(LoopPlayer* player) {
    if (player == NULL || pilha_vazia(player->historico)) {
        return 0;
    }
    
    Musica* musica_anterior = desempilhar(player->historico);
    if (musica_anterior != NULL) {
        player->atual = musica_anterior;
        return 1;
    }
    
    return 0;
}

// Embaralhar a playlist usando o algoritmo Fisher-Yates
int embaralhar_playlist(LoopPlayer* player) {
    if (player == NULL || player->quantidade < 2) {
        return 0;
    }
    
    int n = player->quantidade;
    
    // PASSO 1: Criar array de ponteiros para as músicas
    Musica** array = (Musica**)malloc(n * sizeof(Musica*));
    if (array == NULL) {
        return 0;
    }
    
    // PASSO 2: Percorrer a lista e salvar ponteiros no array
    Musica* atual = player->cabeca;
    for (int i = 0; i < n; i++) {
        array[i] = atual;
        atual = atual->proxima;
    }
    
    // PASSO 3: Embaralhar o array usando Fisher-Yates
    srand((unsigned int)time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Trocar array[i] com array[j]
        Musica* temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
    
    // PASSO 4: Reconstruir a lista circular com a nova ordem
    for (int i = 0; i < n; i++) {
        int prox = (i + 1) % n;      // Próximo (último aponta para primeiro)
        int ant = (i - 1 + n) % n;   // Anterior (primeiro aponta para último)
        
        array[i]->proxima = array[prox];
        array[i]->anterior = array[ant];
    }
    
    // Atualizar a cabeça para o primeiro do array embaralhado
    player->cabeca = array[0];
    
    // Liberar o array temporário
    free(array);
    
    return 1;
}

// Libera toda a memória alocada pelo player
void liberar_player(LoopPlayer* player) {
    if (player == NULL) {
        return;
    }
    
    // Liberar a lista de músicas
    if (!esta_vazia(player)) {
        Musica* atual = player->cabeca;
        
        // Quebrar a circularidade para evitar loop infinito
        player->cabeca->anterior->proxima = NULL;
        
        while (atual != NULL) {
            Musica* temp = atual->proxima;  // Guardar próximo antes de liberar
            
            // Liberar strings alocadas
            free(atual->titulo);
            free(atual->artista);
            
            // Liberar o nó
            free(atual);
            
            atual = temp;
        }
    }
    
    // Liberar a pilha de histórico
    if (player->historico != NULL) {
        destruir_pilha(player->historico);
    }
    
    // Liberar a estrutura do player
    free(player);
}
