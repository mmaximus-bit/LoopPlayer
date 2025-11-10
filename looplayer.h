#ifndef LOOPLAYER_H
#define LOOPLAYER_H

// Estrutura para representar uma música na lista duplamente encadeada circular
typedef struct Musica {
    char* titulo;              // Título da música
    struct Musica* proxima;   // Ponteiro para próxima música
    struct Musica* anterior;  // Ponteiro para música anterior
} Musica;

// Estrutura principal do player
typedef struct {
    Musica* cabeca;          // Ponteiro para o nó cabeça da lista circular
    Musica* atual;           // Música atualmente selecionada
    int quantidade;          // Quantidade de músicas na lista
} LoopPlayer;

// Funções essenciais (criar/destruir)
LoopPlayer* criar_player();
void destruir_player(LoopPlayer* player);

// Funções de manipulação da lista circular
int inserir_musica(LoopPlayer* player, const char* titulo);
int inserir_musica_inicio(LoopPlayer* player, const char* titulo);
int inserir_musica_apos(LoopPlayer* player, const char* titulo_ref, const char* novo_titulo);
int remover_musica(LoopPlayer* player, const char* titulo);

// Funções de consulta
Musica* buscar_musica(LoopPlayer* player, const char* titulo);
int esta_vazia(LoopPlayer* player);
void listar_musicas(LoopPlayer* player);
void listar_musicas_reverso(LoopPlayer* player);

// Funções de controle do player
int tocar_musica(LoopPlayer* player);
int proxima_musica(LoopPlayer* player);
int musica_anterior(LoopPlayer* player);
void parar_reproducao(LoopPlayer* player);

// Funções utilitárias
int obter_quantidade_musicas(LoopPlayer* player);
Musica* obter_musica_atual(LoopPlayer* player);
void limpar_playlist(LoopPlayer* player);

#endif // LOOPLAYER_H