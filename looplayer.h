#ifndef LOOPLAYER_H
#define LOOPLAYER_H

// Estrutura para representar uma música na lista duplamente encadeada circular
typedef struct Musica {
    char* titulo;              // Título da música
    char* artista;             // Artista da música
    struct Musica* proxima;   // Ponteiro para próxima música
    struct Musica* anterior;  // Ponteiro para música anterior
} Musica;

// Estrutura do nó da Pilha Encadeada (para histórico de navegação)
typedef struct NoPilha {
    Musica* musica;            // Ponteiro para a música no histórico
    struct NoPilha* proximo;   // Ponteiro para o próximo nó da pilha
} NoPilha;

// Estrutura da Pilha Encadeada
typedef struct {
    NoPilha* topo;             // Ponteiro para o topo da pilha
    int tamanho;               // Quantidade de elementos na pilha
} Pilha;

// Estrutura principal do player
typedef struct {
    Musica* cabeca;          // Ponteiro para o nó cabeça da lista circular
    Musica* atual;           // Música atualmente selecionada
    int quantidade;          // Quantidade de músicas na lista
    Pilha* historico;        // Pilha de histórico de navegação
} LoopPlayer;

/**
 * @brief Cria um novo player de músicas.
 * 
 * @return Retorna um ponteiro para o LoopPlayer criado, ou NULL em caso de falha na alocação.
 */
LoopPlayer* criar_player();

/**
 * @brief Destrói o player e libera toda a memória alocada.
 * 
 * @param player Ponteiro para o LoopPlayer a ser destruído.
 */
void destruir_player(LoopPlayer* player);

/**
 * @brief Insere uma nova música no final da playlist.
 * 
 * @param player Ponteiro para o LoopPlayer onde a música será inserida.
 * @param titulo String contendo o título da música a ser inserida.
 * 
 * @return Retorna 1 se a inserção for bem-sucedida, ou 0 em caso de falha.
 */
int inserir_musica(LoopPlayer* player, const char* titulo, const char* artista);

/**
 * @brief Insere uma nova música no início da playlist.
 * 
 * @param player Ponteiro para o LoopPlayer onde a música será inserida.
 * @param titulo String contendo o título da música a ser inserida.
 * 
 * @return Retorna 1 se a inserção for bem-sucedida, ou 0 em caso de falha.
 */
int inserir_musica_inicio(LoopPlayer* player, const char* titulo);

/**
 * @brief Insere uma nova música após uma música específica na playlist.
 * 
 * @param player Ponteiro para o LoopPlayer onde a música será inserida.
 * @param titulo_ref String com o título da música de referência.
 * @param novo_titulo String com o título da nova música a ser inserida.
 * 
 * @return Retorna 1 se a inserção for bem-sucedida, ou 0 em caso de falha.
 */
int inserir_musica_apos(LoopPlayer* player, const char* titulo_ref, const char* novo_titulo);

/**
 * @brief Insere uma nova música em uma posição específica da playlist.
 * 
 * @param player Ponteiro para o LoopPlayer onde a música será inserida.
 * @param titulo String contendo o título da música a ser inserida.
 * @param artista String contendo o nome do artista.
 * @param posicao Posição onde a música será inserida (começa em 1).
 * 
 * @return Retorna 1 se a inserção for bem-sucedida, ou 0 em caso de falha.
 */
int inserir_na_posicao(LoopPlayer* player, const char* titulo, const char* artista, int posicao);

/**
 * @brief Remove uma música da playlist pelo título.
 * 
 * @param player Ponteiro para o LoopPlayer de onde a música será removida.
 * @param titulo String contendo o título da música a ser removida.
 * 
 * @return Retorna 1 se a remoção for bem-sucedida, ou 0 se a música não for encontrada.
 */
int remover_musica(LoopPlayer* player, const char* titulo);

/**
 * @brief Remove uma música da playlist por posição específica.
 * 
 * @param player Ponteiro para o LoopPlayer de onde a música será removida.
 * @param posicao Posição da música a ser removida (começa em 1).
 * 
 * @return Retorna 1 se a remoção for bem-sucedida, ou 0 em caso de falha.
 */
int remover_da_posicao(LoopPlayer* player, int posicao);

/**
 * @brief Busca uma música na playlist pelo título.
 * 
 * @param player Ponteiro para o LoopPlayer onde a busca será realizada.
 * @param titulo String contendo o título da música a ser buscada.
 * 
 * @return Retorna um ponteiro para a Musica encontrada, ou NULL se não encontrada.
 */
Musica* buscar_musica(LoopPlayer* player, const char* titulo);

/**
 * @brief Busca músicas que contenham o termo (busca parcial, case-insensitive)
 *
 * @param player Ponteiro para o LoopPlayer onde a busca será realizada.
 * @param termo Termo de busca (parte do título ou artista).
 */
void buscar_musicas(LoopPlayer* player, const char* termo);

/**
 * @brief Verifica se a playlist está vazia.
 * 
 * @param player Ponteiro para o LoopPlayer a ser verificado.
 * 
 * @return Retorna 1 se a playlist estiver vazia, ou 0 caso contrário.
 */
int esta_vazia(LoopPlayer* player);

/**
 * @brief Lista todas as músicas da playlist na ordem normal.
 * 
 * @param player Ponteiro para o LoopPlayer cuja playlist será listada.
 */
void listar_musicas(LoopPlayer* player);

/**
 * @brief Lista todas as músicas da playlist na ordem reversa.
 * 
 * @param player Ponteiro para o LoopPlayer cuja playlist será listada.
 */
void listar_musicas_reverso(LoopPlayer* player);

/**
 * @brief Inicia a reprodução da música atual.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * 
 * @return Retorna 1 se bem-sucedido, ou 0 se a playlist estiver vazia.
 */
int tocar_musica(LoopPlayer* player);

/**
 * @brief Avança para a próxima música na playlist.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * 
 * @return Retorna 1 se bem-sucedido, ou 0 se a playlist estiver vazia.
 */
int proxima_musica(LoopPlayer* player);

/**
 * @brief Retrocede para a música anterior na playlist.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * 
 * @return Retorna 1 se bem-sucedido, ou 0 se a playlist estiver vazia.
 */
int musica_anterior(LoopPlayer* player);

/**
 * @brief Para a reprodução atual.
 * 
 * @param player Ponteiro para o LoopPlayer.
 */
void parar_reproducao(LoopPlayer* player);

/**
 * @brief Obtém a quantidade total de músicas na playlist.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * 
 * @return Retorna o número de músicas na playlist.
 */
int obter_quantidade_musicas(LoopPlayer* player);

/**
 * @brief Obtém a música atualmente selecionada.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * 
 * @return Retorna um ponteiro para a Musica atual, ou NULL se a playlist estiver vazia.
 */
Musica* obter_musica_atual(LoopPlayer* player);

/**
 * @brief Remove todas as músicas da playlist.
 * 
 * @param player Ponteiro para o LoopPlayer cuja playlist será limpa.
 */
void limpar_playlist(LoopPlayer* player);

/**
 * @brief Move uma música de uma posição para outra na playlist.
 * 
 * Desliga o nó da posição de origem (religando seus vizinhos) e o religa
 * na posição de destino. Manipulação cuidadosa de ponteiros em lista
 * duplamente encadeada circular.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * @param pos_origem Posição atual da música (1 a quantidade).
 * @param pos_destino Posição de destino da música (1 a quantidade).
 * 
 * @return Retorna 1 se a movimentação for bem-sucedida, ou 0 em caso de falha.
 */
int mover_musica(LoopPlayer* player, int pos_origem, int pos_destino);

// ============ FUNÇÕES DA PILHA DE HISTÓRICO ============

/**
 * @brief Cria uma nova pilha vazia.
 * 
 * @return Retorna um ponteiro para a Pilha criada.
 */
Pilha* criar_pilha();

/**
 * @brief Destrói a pilha e libera a memória.
 * 
 * @param pilha Ponteiro para a Pilha a ser destruída.
 */
void destruir_pilha(Pilha* pilha);

/**
 * @brief Empilha uma música no topo da pilha (push).
 * 
 * @param pilha Ponteiro para a Pilha.
 * @param musica Ponteiro para a Música a ser empilhada.
 * 
 * @return Retorna 1 se bem-sucedido, ou 0 em caso de falha.
 */
int empilhar(Pilha* pilha, Musica* musica);

/**
 * @brief Desempilha e retorna a música do topo da pilha (pop).
 * 
 * @param pilha Ponteiro para a Pilha.
 * 
 * @return Retorna ponteiro para a Música desempilhada, ou NULL se vazia.
 */
Musica* desempilhar(Pilha* pilha);

/**
 * @brief Verifica se a pilha está vazia.
 * 
 * @param pilha Ponteiro para a Pilha.
 * 
 * @return Retorna 1 se vazia, ou 0 caso contrário.
 */
int pilha_vazia(Pilha* pilha);

/**
 * @brief Volta para a música anterior do histórico.
 * 
 * Dá pop na pilha de histórico e navega o player para essa música.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * 
 * @return Retorna 1 se bem-sucedido, ou 0 se o histórico estiver vazio.
 */
int voltar_historico(LoopPlayer* player);

/**
 * @brief Embaralha a playlist de forma aleatória (modo shuffle).
 * 
 * Utiliza o algoritmo Fisher-Yates para reordenar os nós da lista
 * de forma aleatória. Reconstrói a lista circular religando os
 * ponteiros proxima e anterior de todos os nós.
 * 
 * @param player Ponteiro para o LoopPlayer.
 * 
 * @return Retorna 1 se bem-sucedido, ou 0 em caso de falha.
 */
int embaralhar_playlist(LoopPlayer* player);

/**
 * @brief Libera toda a memória alocada pela playlist.
 * 
 * Percorre a lista circular e libera cada nó (título, artista e struct).
 * Usa ponteiro temporário para não perder a referência durante a liberação.
 * Também libera a pilha de histórico.
 * 
 * @param player Ponteiro para o LoopPlayer a ser liberado.
 */
void liberar_player(LoopPlayer* player);

#endif // LOOPLAYER_H