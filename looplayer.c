#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "looplayer.h"

// Criar um novo player
LoopPlayer* criar_player() {
    LoopPlayer* player = (LoopPlayer*)malloc(sizeof(LoopPlayer));
    if (player == NULL) {
        printf("Erro: Falha ao alocar memória para o player!\n");
        return NULL;
    }
    
    player->cabeca = NULL;
    player->atual = NULL;
    player->quantidade = 0;
    player->historico = criar_pilha();
    
    return player;
}

// Inserir música no final da lista
int inserir_musica(LoopPlayer* player, const char* titulo, const char* artista) {
    Musica* nova = (Musica*)malloc(sizeof(Musica));
    if (nova == NULL) {
        printf("Erro: Falha ao alocar memória para a música!\n");
        return 0;
    }

    nova->titulo = (char*)malloc(strlen(titulo) + 1);
    if (nova->titulo == NULL) {
        printf("Erro: Falha ao alocar memória para o título!\n");
        free(nova);
        return 0;
    }
    strcpy(nova->titulo, titulo);

    nova->artista = (char*)malloc(strlen(artista) + 1);
    if (nova->artista == NULL) {
        printf("Erro: Falha ao alocar memória para o artista!\n");
        free(nova->titulo);
        free(nova);
        return 0;
    }
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
        if (nova == NULL) {
            printf("Erro: Falha ao alocar memória para a música!\n");
            return 0;
        }
        nova->titulo = (char*)malloc(strlen(titulo) + 1);
        if (nova->titulo == NULL) {
            printf("Erro: Falha ao alocar memória para o título!\n");
            free(nova);
            return 0;
        }
        strcpy(nova->titulo, titulo);
        nova->artista = (char*)malloc(strlen(artista) + 1);
        if (nova->artista == NULL) {
            printf("Erro: Falha ao alocar memória para o artista!\n");
            free(nova->titulo);
            free(nova);
            return 0;
        }
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
    if (nova == NULL) {
        printf("Erro: Falha ao alocar memória para a música!\n");
        return 0;
    }
    nova->titulo = (char*)malloc(strlen(titulo) + 1);
    if (nova->titulo == NULL) {
        printf("Erro: Falha ao alocar memória para o título!\n");
        free(nova);
        return 0;
    }
    strcpy(nova->titulo, titulo);
    nova->artista = (char*)malloc(strlen(artista) + 1);
    if (nova->artista == NULL) {
        printf("Erro: Falha ao alocar memória para o artista!\n");
        free(nova->titulo);
        free(nova);
        return 0;
    }
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
    if (pilha == NULL) {
        printf("Erro: Falha ao alocar memória para a pilha!\n");
        return NULL;
    }
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

// ============ NAVEGAÇÃO INTELIGENTE ============

// Avançar X músicas na playlist
int avancar_musicas(LoopPlayer* player, int quantidade) {
    if (player == NULL || esta_vazia(player) || quantidade <= 0) {
        return 0;
    }
    
    // Limitar para não ultrapassar a playlist
    if (quantidade > player->quantidade) {
        quantidade = player->quantidade;
    }
    
    // Avançar X vezes
    for (int i = 0; i < quantidade; i++) {
        proxima_musica(player);
    }
    
    return 1;
}

// Retroceder X músicas na playlist
int retroceder_musicas(LoopPlayer* player, int quantidade) {
    if (player == NULL || esta_vazia(player) || quantidade <= 0) {
        return 0;
    }
    
    // Limitar para não ultrapassar a playlist
    if (quantidade > player->quantidade) {
        quantidade = player->quantidade;
    }
    
    // Retroceder X vezes
    for (int i = 0; i < quantidade; i++) {
        musica_anterior(player);
    }
    
    return 1;
}

// ============ IMPLEMENTAÇÃO DO GRAFO DE SIMILARIDADE ============

// Cria um novo grafo vazio
GrafoSimilaridade* criar_grafo() {
    GrafoSimilaridade* grafo = (GrafoSimilaridade*)malloc(sizeof(GrafoSimilaridade));
    if (grafo == NULL) {
        return NULL;
    }
    
    grafo->vertices = NULL;
    grafo->quantidade_vertices = 0;
    
    return grafo;
}

// Adiciona um vértice (música) ao grafo
int adicionar_vertice_grafo(GrafoSimilaridade* grafo, Musica* musica) {
    if (grafo == NULL || musica == NULL) {
        return 0;
    }
    
    // Verificar se a música já existe no grafo
    NoGrafo* temp = grafo->vertices;
    while (temp != NULL) {
        if (temp->musica == musica) {
            return 1; // Já existe, não faz nada
        }
        temp = temp->proximo;
    }
    
    // Criar novo vértice
    NoGrafo* novo_vertice = (NoGrafo*)malloc(sizeof(NoGrafo));
    if (novo_vertice == NULL) {
        return 0;
    }
    
    novo_vertice->musica = musica;
    novo_vertice->adjacentes = NULL;
    novo_vertice->proximo = grafo->vertices;
    grafo->vertices = novo_vertice;
    grafo->quantidade_vertices++;
    
    return 1;
}

// Adiciona uma aresta entre duas músicas (não direcionada)
int adicionar_aresta_grafo(GrafoSimilaridade* grafo, Musica* musica1, Musica* musica2) {
    if (grafo == NULL || musica1 == NULL || musica2 == NULL || musica1 == musica2) {
        return 0;
    }
    
    // Adicionar vértices se não existirem
    adicionar_vertice_grafo(grafo, musica1);
    adicionar_vertice_grafo(grafo, musica2);
    
    // Encontrar vértices
    NoGrafo* v1 = grafo->vertices;
    while (v1 != NULL && v1->musica != musica1) {
        v1 = v1->proximo;
    }
    
    NoGrafo* v2 = grafo->vertices;
    while (v2 != NULL && v2->musica != musica2) {
        v2 = v2->proximo;
    }
    
    if (v1 == NULL || v2 == NULL) {
        return 0;
    }
    
    // Verificar se aresta já existe (v1 -> v2)
    NoAresta* temp = v1->adjacentes;
    while (temp != NULL) {
        if (temp->musica_relacionada == musica2) {
            return 1; // Aresta já existe
        }
        temp = temp->proxima;
    }
    
    // Adicionar aresta v1 -> v2
    NoAresta* nova_aresta1 = (NoAresta*)malloc(sizeof(NoAresta));
    if (nova_aresta1 == NULL) {
        return 0;
    }
    nova_aresta1->musica_relacionada = musica2;
    nova_aresta1->proxima = v1->adjacentes;
    v1->adjacentes = nova_aresta1;
    
    // Adicionar aresta v2 -> v1 (não direcionada)
    NoAresta* nova_aresta2 = (NoAresta*)malloc(sizeof(NoAresta));
    if (nova_aresta2 == NULL) {
        free(nova_aresta1);
        return 0;
    }
    nova_aresta2->musica_relacionada = musica1;
    nova_aresta2->proxima = v2->adjacentes;
    v2->adjacentes = nova_aresta2;
    
    return 1;
}

// Verificar se música foi visitada
static int foi_visitada(VisitadosBFS* visitados, Musica* musica) {
    for (int i = 0; i < visitados->total_visitados; i++) {
        if (visitados->visitados[i] == musica) {
            return 1;
        }
    }
    return 0;
}

// Adicionar à lista de visitados
static int adicionar_visitado(VisitadosBFS* visitados, Musica* musica) {
    Musica** novo_array = (Musica**)realloc(visitados->visitados, 
                                           (visitados->total_visitados + 1) * sizeof(Musica*));
    if (novo_array == NULL) {
        return 0;
    }
    novo_array[visitados->total_visitados] = musica;
    visitados->visitados = novo_array;
    visitados->total_visitados++;
    return 1;
}

// DFS recursivo
static int dfs_auxiliar(GrafoSimilaridade* grafo, Musica* musica_atual, 
                        VisitadosBFS* visitados, int profundidade) {
    if (musica_atual == NULL || foi_visitada(visitados, musica_atual)) {
        return 0;
    }
    
    // Marcar como visitada
    adicionar_visitado(visitados, musica_atual);
    
    // Imprimir com indentação
    for (int i = 0; i < profundidade; i++) printf("  ");
    printf("└─ %s - %s\n", musica_atual->titulo, musica_atual->artista);
    
    int contador = 1;
    
    // Encontrar vértice e explorar adjacentes
    NoGrafo* v = grafo->vertices;
    while (v != NULL) {
        if (v->musica == musica_atual) {
            NoAresta* aresta = v->adjacentes;
            while (aresta != NULL) {
                if (!foi_visitada(visitados, aresta->musica_relacionada)) {
                    contador += dfs_auxiliar(grafo, aresta->musica_relacionada, visitados, profundidade + 1);
                }
                aresta = aresta->proxima;
            }
            break;
        }
        v = v->proximo;
    }
    
    return contador;
}

// Busca em profundidade (DFS)
int busca_profundidade(GrafoSimilaridade* grafo, Musica* musica_partida, VisitadosBFS* visitados) {
    if (grafo == NULL || musica_partida == NULL || visitados == NULL) {
        return 0;
    }
    
    return dfs_auxiliar(grafo, musica_partida, visitados, 0);
}

// Busca em largura (BFS) com camadas
int busca_largura_camadas(GrafoSimilaridade* grafo, Musica* musica_partida) {
    if (grafo == NULL || musica_partida == NULL) {
        return 0;
    }
    
    // Alocar fila para BFS
    Musica** fila = (Musica**)malloc(grafo->quantidade_vertices * sizeof(Musica*));
    int* distancia = (int*)malloc(grafo->quantidade_vertices * sizeof(int));
    
    if (fila == NULL || distancia == NULL) {
        free(fila);
        free(distancia);
        return 0;
    }
    
    int inicio = 0, fim = 0;
    
    // Inicializar distâncias com -1 (não visitado)
    for (int i = 0; i < grafo->quantidade_vertices; i++) {
        distancia[i] = -1;
    }
    
    // Mapa para associar música com índice de distância
    Musica** lista_musicas = (Musica**)malloc(grafo->quantidade_vertices * sizeof(Musica*));
    if (lista_musicas == NULL) {
        free(fila);
        free(distancia);
        return 0;
    }
    
    int idx_musica = 0;
    NoGrafo* temp = grafo->vertices;
    while (temp != NULL && idx_musica < grafo->quantidade_vertices) {
        lista_musicas[idx_musica] = temp->musica;
        if (temp->musica == musica_partida) {
            distancia[idx_musica] = 0;
        }
        idx_musica++;
        temp = temp->proximo;
    }
    
    // Enfileirar música inicial
    fila[fim++] = musica_partida;
    
    printf("\n📊 Exploração em CAMADAS (BFS) a partir de: %s - %s\n", 
           musica_partida->titulo, musica_partida->artista);
    printf("═══════════════════════════════════════════════════════════\n");
    printf("Camada 0 (origem):\n");
    printf("  └─ %s - %s\n", musica_partida->titulo, musica_partida->artista);
    
    int total_visitados = 1;
    int proxima_camada = 1;
    int musicas_camada_atual = 1;
    int musicas_proxima_camada = 0;
    
    while (inicio < fim) {
        Musica* musica_atual = fila[inicio++];
        
        // Encontrar vértice
        NoGrafo* v = grafo->vertices;
        while (v != NULL) {
            if (v->musica == musica_atual) {
                // Explorar adjacentes
                NoAresta* aresta = v->adjacentes;
                while (aresta != NULL) {
                    Musica* adj = aresta->musica_relacionada;
                    
                    // Encontrar índice
                    int idx = -1;
                    for (int i = 0; i < idx_musica; i++) {
                        if (lista_musicas[i] == adj) {
                            idx = i;
                            break;
                        }
                    }
                    
                    if (idx != -1 && distancia[idx] == -1) {
                        distancia[idx] = distancia[idx_musica] == -1 ? proxima_camada : distancia[idx_musica] + 1;
                        fila[fim++] = adj;
                        musicas_proxima_camada++;
                        total_visitados++;
                    }
                    
                    aresta = aresta->proxima;
                }
                break;
            }
            v = v->proximo;
        }
        
        musicas_camada_atual--;
        
        // Se terminou camada, imprimir próxima
        if (musicas_camada_atual == 0 && musicas_proxima_camada > 0) {
            printf("\nCamada %d (distância %d):\n", proxima_camada, proxima_camada);
            for (int i = inicio; i < fim && i < grafo->quantidade_vertices; i++) {
                printf("  └─ %s - %s\n", fila[i]->titulo, fila[i]->artista);
            }
            musicas_camada_atual = musicas_proxima_camada;
            musicas_proxima_camada = 0;
            proxima_camada++;
        }
    }
    
    free(fila);
    free(distancia);
    free(lista_musicas);
    
    return total_visitados;
}

// Lista músicas relacionadas (vizinhos diretos)
void listar_relacionadas(GrafoSimilaridade* grafo, Musica* musica) {
    if (grafo == NULL || musica == NULL) {
        return;
    }
    
    // Encontrar vértice
    NoGrafo* v = grafo->vertices;
    while (v != NULL) {
        if (v->musica == musica) {
            printf("\n🎵 Músicas relacionadas a: %s - %s\n", musica->titulo, musica->artista);
            printf("═══════════════════════════════════════════════════════════\n");
            
            if (v->adjacentes == NULL) {
                printf("Nenhuma música relacionada encontrada.\n");
            } else {
                int contador = 1;
                NoAresta* aresta = v->adjacentes;
                while (aresta != NULL) {
                    printf("%d. %s - %s\n", contador++, 
                           aresta->musica_relacionada->titulo, 
                           aresta->musica_relacionada->artista);
                    aresta = aresta->proxima;
                }
            }
            return;
        }
        v = v->proximo;
    }
    
    printf("Música não encontrada no grafo.\n");
}

// Libera o grafo
void liberar_grafo(GrafoSimilaridade* grafo) {
    if (grafo == NULL) {
        return;
    }
    
    NoGrafo* v = grafo->vertices;
    while (v != NULL) {
        NoGrafo* v_temp = v;
        v = v->proximo;
        
        // Liberar adjacentes
        NoAresta* a = v_temp->adjacentes;
        while (a != NULL) {
            NoAresta* a_temp = a;
            a = a->proxima;
            free(a_temp);
        }
        
        free(v_temp);
    }
    
    free(grafo);
}
