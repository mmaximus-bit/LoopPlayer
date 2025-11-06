# 🎵 Contribuindo para o LoopPlayer

Olá! Ficamos muito felizes com seu interesse em contribuir para o LoopPlayer.

Este é um projeto de estudo focado na implementação e uso de listas circulares duplamente encadeadas em C. Nosso objetivo é manter um código limpo, funcional e que sirva como um bom material de aprendizado.

Toda ajuda é bem-vinda, seja relatando bugs, sugerindo melhorias ou enviando código!

## 🤝 Como Você Pode Ajudar

Existem algumas formas principais de contribuir:

* **Relatando Bugs:** Se você encontrar um comportamento inesperado, um *segmentation fault* ou qualquer erro, por favor, abra uma "Issue".
* **Sugerindo Melhorias:** Tem uma ideia para uma nova funcionalidade (como as que listamos no README) ou uma forma de otimizar o código? Abra uma "Issue" para discutirmos.
* **Enviando Código:** Se você quer corrigir um bug ou implementar uma funcionalidade, você pode enviar um "Pull Request".

## 🐛 Relatando Bugs (Issues)

Ao abrir uma [Issue](https://github.com/mmaximus-bit/LoopPlayer/issues) para relatar um bug, por favor, inclua o máximo de detalhes possível:

1.  **Título Claro:** Ex: "Erro de segmentação ao remover a última música da lista".
2.  **Passos para Reproduzir:** O que você fez para o erro acontecer?
3.  **Comportamento Esperado:** O que deveria ter acontecido?
4.  **Comportamento Atual:** O que de fato aconteceu? (Inclua mensagens de erro, se houver).
5.  **Seu Ambiente:** Qual seu sistema operacional e qual versão do GCC (ou outro compilador) você está usando.

## 🚀 Enviando Alterações (Pull Requests)

Se você quer enviar código, este é o fluxo que seguimos:

1.  **Faça o "Fork"** do repositório para a sua própria conta GitHub.
2.  **Clone o seu "Fork"** para a sua máquina local (`git clone ...`).
3.  **Crie uma nova "Branch"** para suas alterações. Use um nome descritivo.
    ```bash
    # Para uma nova funcionalidade
    git checkout -b feature/nome-da-feature
    
    # Para uma correção de bug
    git checkout -b fix/descricao-do-bug
    ```
4.  **Faça suas Alterações.** Escreva seu código seguindo o guia de estilo abaixo.
5.  **Compile e Teste.** Certifique-se de que o programa compila sem erros ou *warnings* (use a flag `-Wall`).
6.  **Faça o "Commit"** das suas alterações com uma mensagem clara.
    ```bash
    git commit -m "feat: Adiciona a funcionalidade X"
    ```
7.  **Envie (Push)** sua branch para o seu fork no GitHub.
    ```bash
    git push origin feature/nome-da-feature
    ```
8.  **Abra um "Pull Request"** (PR) no repositório `mmaximus-bit/LoopPlayer`. No PR, descreva o que você mudou e por quê.

## ⚙️ Guia de Estilo de Código (C)

Para manter a coesão do projeto, pedimos que siga estas regras:

* **Compilação Limpa:** Seu código **DEVE** compilar sem *warnings* usando `gcc -Wall`.
* **Gerenciamento de Memória:** Se você usar `malloc()`, certifique-se de que há um `free()` correspondente. Verifique sempre se `malloc()` não retornou `NULL`.
* **Nomenclatura:**
    * Funções e variáveis: use `snake_case` (ex: `adicionar_musica`).
    * Structs: use `PascalCase` (ex: `struct Musica`).
* **Indentação:** Use **4 espaços** para indentação, não tabs.
* **Comentários:** Comente partes do código que não são óbvias. Explique o "porquê" da sua lógica, não o "o quê".
* **Headers (.h):** Declare protótipos de funções nos arquivos `.h` e inclua as *include guards* (`#ifndef`, `#define`, `#endif`).

Obrigado por ajudar a tornar o LoopPlayer um projeto de estudo ainda melhor!
