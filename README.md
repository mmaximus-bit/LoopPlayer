# 🎵 LoopPlayer

Um gerenciador de playlist de músicas simples, desenvolvido em linguagem C, como um projeto de estudo de estruturas de dados. O núcleo do sistema utiliza uma **lista circular duplamente encadeada** para permitir navegação fluida e repetição contínua da playlist.

---

## 🚀 Sobre o Projeto

Este projeto demonstra a aplicação prática de uma lista circular duplamente encadeada. Em vez de simplesmente implementar a estrutura de dados, este repositório a utiliza para construir uma aplicação de console funcional: um gerenciador de músicas.

A natureza **circular** da lista é perfeita para a função "Repetir", e a capacidade **duplamente encadeada** é essencial para as funções "Próxima" e "Anterior".

## 🛠️ Funcionalidades Implementadas

* **Adicionar Música:** Insere uma nova música na playlist.
* **Remover Música:** Remove uma música específica da lista.
* **Listar Músicas:** Exibe todas as músicas na ordem.
* **Navegação:**
    * `Próxima`: Avança para a próxima música.
    * `Anterior`: Retorna para a música anterior.
* **Repetição (Looping):** Ao chegar ao fim da lista, "Próxima" leva automaticamente de volta à primeira música (e vice-versa com "Anterior").

*(Opcional: Adicione se planeja fazer)*
* [ ] Tocar música (simulação com `sleep`)
* [ ] Buscar música por nome

## ⚙️ Estrutura de Dados

O coração do projeto é a `struct Node` (ou `Musica`), que se conecta da seguinte forma:

* Cada nó (música) possui um ponteiro `next` (próximo) e `prev` (anterior).
* O nó "cabeça" (head) tem seu `prev` apontando para o último elemento da lista.
* O último elemento da lista tem seu `next` apontando de volta para o "cabeça".

Isso elimina a necessidade de ponteiros `NULL` de início/fim e simplifica a lógica de navegação.

## 💻 Como Compilar e Executar

*(Instruções básicas para um compilador C)*

1.  Clone o repositório:
    ```bash
    git clone [https://github.com/seu-usuario/seu-repositorio.git](https://github.com/seu-usuario/seu-repositorio.git)
    cd seu-repositorio
    ```

2.  Compile o projeto (usando GCC como exemplo):
    ```bash
    gcc -o player *.c -Wall
    ```

3.  Execute o programa:
    ```bash
    ./player
    ```

## 🧑‍💻 Desenvolvedores

* Miguel Maximus
* Bruna Cayres
* Mateus Mello
* Maria Eduarda

---
Feito com C puro e um 🎧!
