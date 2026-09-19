#include "splay.hpp"

#include <iostream>
#include <string>

/// Funções básicas de construção, destruição e liberação dos nós da árvore.

Splay::Splay()
    : raiz(nullptr) {
}

Splay::~Splay() {
    liberar(raiz);
}

/// @brief Libera recursivamente todos os nós existentes na árvore.
/// @param no Nó atual que está sendo liberado.
void Splay::liberar(Node* no) {
    if (no == nullptr) {
        return;
    }

    liberar(no->esquerda);
    liberar(no->direita);

    delete no;
}


/// Funções necessárias da árvore Splay.

/// @brief Faz uma rotação simples à direita, movendo o filho esquerdo para a posição de raiz da subárvore.
/// @param no Raiz atual da subárvore.
void Splay::rotacaoDireita(Node* no) {

    Node* filho = no->esquerda;

    if (filho == nullptr) {
        return;
    }
    no->esquerda = filho->direita;

    if (filho->direita != nullptr) {
        filho->direita->pai = no;
    }


    filho->pai = no->pai;

    if (no->pai == nullptr) {
        raiz = filho;
    }

    // Se no era filho esquerdo.
    else if (no == no->pai->esquerda) {
        no->pai->esquerda = filho;
    }

    else {
        no->pai->direita = filho;
    }

    filho->direita = no;
    no->pai = filho;
}


/// @brief Faz uma rotação simples à esquerda, movendo o filho direito para a posição de raiz da subárvore.
/// @param no Raiz atual da subárvore.
void Splay::rotacaoEsquerda(Node* no) {

    Node* filho = no->direita;

    if (filho == nullptr) {
        return;
    }

    no->direita = filho->esquerda;

    if (filho->esquerda != nullptr) {
        filho->esquerda->pai = no;
    }

    filho->pai = no->pai;

    if (no->pai == nullptr) {
        raiz = filho;
    }

    else if (no == no->pai->esquerda) {
        no->pai->esquerda = filho;
    }

    else {
        no->pai->direita = filho;
    }

    filho->esquerda = no;
    no->pai = filho;
}


/// @brief Reorganiza a árvore trazendo o nó recebido até a raiz. Para isso verifica a posição do nó em relação ao pai e ao avô, utilizando os casos Zig, Zag, Zig-Zig, Zag-Zag, Zig-Zag e Zag-Zig.
/// @param no Nó que será movido até a raiz.
void Splay::splay(Node* no) {

    if (no == nullptr) {
        return;
    }

    while (no->pai != nullptr) {

        Node* pai = no->pai;
        Node* avo = pai->pai;

        if (avo == nullptr) {

            if (no == pai->esquerda) {
                rotacaoDireita(pai);
            }

            else {
                rotacaoEsquerda(pai);
            }
        }
        else if (
            pai == avo->esquerda &&
            no == pai->esquerda
        ) {

            rotacaoDireita(avo);
            rotacaoDireita(pai);
        }

        else if (
            pai == avo->direita &&
            no == pai->direita
        ) {

            rotacaoEsquerda(avo);
            rotacaoEsquerda(pai);
        }
        else if (
            pai == avo->esquerda &&
            no == pai->direita
        ) {

            rotacaoEsquerda(pai);
            rotacaoDireita(avo);
        }
        else {

            rotacaoDireita(pai);
            rotacaoEsquerda(avo);
        }
    }
}


/// @brief Insere uma nova chave utilizando inicialmente as propriedades de uma BST e após a inserção aplica o Splay, movendo o novo nó para a raiz da árvore.
/// @param chave Chave que será inserida.
void Splay::inserirSplay(int chave) {

    if (raiz == nullptr) {
        raiz = new Node(chave);
        return;
    }

    Node* atual = raiz;
    Node* pai = nullptr;

    while (atual != nullptr) {

        pai = atual;

        if (chave < atual->chave) {
            atual = atual->esquerda;
        }

        else if (chave > atual->chave) {
            atual = atual->direita;
        }
        else {
            splay(atual);
            return;
        }
    }

    Node* novo = new Node(chave);

    novo->pai = pai;

    if (chave < pai->chave) {
        pai->esquerda = novo;
    }

    else {
        pai->direita = novo;
    }

    // O novo nó é levado até a raiz.
    splay(novo);
}


/// @brief Busca internamente uma chave utilizando o mesmo funcionamento de uma busca em uma BST.
/// @param chave Chave que será buscada.
/// @return Ponteiro para o nó encontrado ou nullptr caso a chave não exista.
Splay::Node* Splay::buscarNo(int chave) {

    Node* atual = raiz;

    while (atual != nullptr) {

        if (chave == atual->chave) {
            return atual;
        }

        if (chave < atual->chave) {
            atual = atual->esquerda;
        }

        else {
            atual = atual->direita;
        }
    }

    return nullptr;
}


/// @brief Busca uma chave na Splay e, caso encontre, move o nó encontrado até a raiz da árvore através da operação de Splay.
/// @param chave Chave que será buscada.
/// @return true caso a chave exista e false caso não exista.
bool Splay::buscarSplay(int chave) {

    Node* encontrado = buscarNo(chave);

    if (encontrado == nullptr) {
        return false;
    }

    splay(encontrado);

    return true;
}


/// @brief Encontra o maior nó existente em uma determinada subárvore. Como a estrutura também segue as propriedades de uma BST, o maior elemento é encontrado percorrendo os filhos direitos.
/// @param no Raiz da subárvore.
/// @return Ponteiro para o maior nó encontrado.
Splay::Node* Splay::maiorNo(Node* no) const {

    if (no == nullptr) {
        return nullptr;
    }

    while (no->direita != nullptr) {
        no = no->direita;
    }

    return no;
}


/// @brief Remove uma chave da árvore. Primeiro procura o nó desejado e aplica Splay para trazê-lo até a raiz. Depois separa as subárvores esquerda e direita, remove a raiz e reorganiza as duas partes.
/// @param chave Chave que será removida.
void Splay::removerSplay(int chave) {

    Node* alvo = buscarNo(chave);

    if (alvo == nullptr) {
        return;
    }

    splay(alvo);

    Node* arvoreEsquerda = raiz->esquerda;
    Node* arvoreDireita = raiz->direita;

    if (arvoreEsquerda != nullptr) {
        arvoreEsquerda->pai = nullptr;
    }

    if (arvoreDireita != nullptr) {
        arvoreDireita->pai = nullptr;
    }

    delete raiz;
    raiz = nullptr;

    if (arvoreEsquerda == nullptr) {
        raiz = arvoreDireita;
        return;
    }

    raiz = arvoreEsquerda;

    Node* maior = maiorNo(raiz);
    splay(maior);
    raiz->direita = arvoreDireita;

    if (arvoreDireita != nullptr) {
        arvoreDireita->pai = raiz;
    }
}


/// @brief Imprime a estrutura atual da Splay de forma lateral.
/// @return Não possui retorno.
void Splay::imprimirSplay() const {

    if (raiz == nullptr) {
        std::cout << "Splay vazia.\n";
        return;
    }

    imprimirRecursivo(raiz, 0);
}

/// @brief Função auxiliar que percorre a árvore recursivamente, imprimindo primeiro a subárvore direita, depois o nó atual epor último a subárvore esquerda.
/// @param no Nó atual que está sendo percorrido.
/// @param nivel Nível atual utilizado para organizar a impressão.
void Splay::imprimirRecursivo(const Node* no, int nivel) const {

    if (no == nullptr) {
        return;
    }

    imprimirRecursivo(no->direita, nivel + 1);

    std::cout << std::string(nivel * 4, ' ') << no->chave << '\n';
    imprimirRecursivo(no->esquerda, nivel + 1);
}