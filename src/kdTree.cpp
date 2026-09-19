#include "kdTree.hpp"

#include <iostream>
#include <string>


/// Funções básicas de construção, destruição e liberação dos nós.

KDTree::KDTree()
    : raiz(nullptr) {
}

KDTree::~KDTree() {
    liberar(raiz);
}

/// @brief Libera recursivamente todos os nós existentes na KD-Tree.
/// @param no Nó atual que está sendo liberado.
void KDTree::liberar(Node* no) {
    if (no == nullptr) {
        return;
    }

    liberar(no->esquerda);
    liberar(no->direita);

    delete no;
}


/// Funções auxiliares.

/// @brief Retorna a coordenada do ponto correspondente ao eixo atual.
/// O eixo 0 representa X e o eixo 1 representa Y.
/// @param ponto Ponto utilizado.
/// @param eixo Eixo que será acessado.
/// @return Valor da coordenada X ou Y.
double KDTree::valorEixo(const Ponto& ponto, int eixo) const {
    if (eixo == 0) {
        return ponto.x;
    }

    return ponto.y;
}


/// @brief Verifica se dois pontos possuem as mesmas coordenadas.
/// @param a Primeiro ponto.
/// @param b Segundo ponto.
/// @return true caso sejam iguais e false caso contrário.
bool KDTree::pontosIguais(const Ponto& a, const Ponto& b) const {
    return a.x == b.x && a.y == b.y;
}


/// Inserção.

/// @brief Insere um novo ponto na KD-Tree.
/// @param x Coordenada X.
/// @param y Coordenada Y.
void KDTree::inserirKDTree(double x, double y) {
    Ponto ponto(x, y);

    raiz = inserirRecursivo(raiz, ponto, 0);
}


/// @brief Insere recursivamente um ponto, alternando a comparação
/// entre os eixos X e Y de acordo com a profundidade.
/// @param no Nó atual.
/// @param ponto Ponto que será inserido.
/// @param profundidade Profundidade atual da árvore.
/// @return Raiz atualizada da subárvore.
KDTree::Node* KDTree::inserirRecursivo(Node* no, const Ponto& ponto,int profundidade) {
    if (no == nullptr) {
        return new Node(ponto);
    }

    if (pontosIguais(no->ponto, ponto)) {
        return no;
    }

    int eixo = profundidade % 2;

    if (
        valorEixo(ponto, eixo) <
        valorEixo(no->ponto, eixo)
    ) {
        no->esquerda = inserirRecursivo(
            no->esquerda,
            ponto,
            profundidade + 1
        );
    } else {
        no->direita = inserirRecursivo(
            no->direita,
            ponto,
            profundidade + 1
        );
    }

    return no;
}


/// Busca.

/// @brief Busca um ponto específico dentro da KD-Tree.
/// @param x Coordenada X.
/// @param y Coordenada Y.
/// @return true caso o ponto exista e false caso contrário.
bool KDTree::buscarKDTree(
    double x,
    double y
) const {
    Ponto ponto(x, y);

    return buscarRecursivo(
        raiz,
        ponto,
        0
    );
}


/// @brief Busca recursivamente um ponto utilizando o eixo correspondente
/// à profundidade atual.
/// @param no Nó atual.
/// @param ponto Ponto buscado.
/// @param profundidade Profundidade atual.
/// @return true caso encontre o ponto e false caso contrário.
bool KDTree::buscarRecursivo(
    const Node* no,
    const Ponto& ponto,
    int profundidade
) const {
    if (no == nullptr) {
        return false;
    }

    if (pontosIguais(no->ponto, ponto)) {
        return true;
    }

    int eixo = profundidade % 2;

    if (
        valorEixo(ponto, eixo) <
        valorEixo(no->ponto, eixo)
    ) {
        return buscarRecursivo(
            no->esquerda,
            ponto,
            profundidade + 1
        );
    }

    return buscarRecursivo(
        no->direita,
        ponto,
        profundidade + 1
    );
}


/// Remoção.

/// @brief Remove um ponto da KD-Tree.
/// @param x Coordenada X.
/// @param y Coordenada Y.
void KDTree::removerKDTree( double x, double y) {
    Ponto ponto(x, y);
    raiz = removerRecursivo(raiz, ponto,0);
}


/// @brief Encontra o nó que possui o menor valor em um determinado eixo.
/// @param no Raiz da subárvore atual.
/// @param eixo Eixo utilizado na comparação.
/// @param profundidade Profundidade atual.
/// @return Nó que possui o menor valor no eixo desejado.
KDTree::Node* KDTree::encontrarMinimo(
    Node* no,
    int eixo,
    int profundidade
) const {
    if (no == nullptr) {
        return nullptr;
    }

    int eixoAtual = profundidade % 2;

    if (eixoAtual == eixo) {
        if (no->esquerda == nullptr) {
            return no;
        }

        return encontrarMinimo(no->esquerda, eixo, profundidade + 1);
    }
    Node* menor = no;

    Node* menorEsquerda = encontrarMinimo(no->esquerda, eixo, profundidade + 1 );

    Node* menorDireita = encontrarMinimo( no->direita, eixo, profundidade + 1);

    if (menorEsquerda != nullptr && valorEixo(menorEsquerda->ponto, eixo) < valorEixo(menor->ponto, eixo)) {
      menor = menorEsquerda;
    }

    if (menorDireita != nullptr && valorEixo(menorDireita->ponto, eixo) < valorEixo(menor->ponto, eixo)) {
        menor = menorDireita;
    }

    return menor;
}


/// @brief Remove recursivamente um ponto mantendo o particionamento
/// da KD-Tree.
/// @param no Nó atual.
/// @param ponto Ponto que será removido.
/// @param profundidade Profundidade atual.
/// @return Raiz atualizada da subárvore.
KDTree::Node* KDTree::removerRecursivo(Node* no,const Ponto& ponto,int profundidade) {
    if (no == nullptr) {
        return nullptr;
    }

    int eixo = profundidade % 2;

    if (pontosIguais(no->ponto, ponto)) {
        if (no->direita != nullptr) {

            Node* minimo = encontrarMinimo( no->direita, eixo,profundidade + 1);

            no->ponto = minimo->ponto;

            no->direita = removerRecursivo(no->direita, minimo->ponto, profundidade + 1);

            return no;
        }
        if (no->esquerda != nullptr) {

            Node* minimo = encontrarMinimo(no->esquerda, eixo, profundidade + 1);

            no->ponto = minimo->ponto;

            no->direita = removerRecursivo( no->esquerda, minimo->ponto, profundidade + 1);

            no->esquerda = nullptr;

            return no;
        }
        delete no;

        return nullptr;
    }

    if (valorEixo(ponto, eixo) < valorEixo(no->ponto, eixo)) {
        no->esquerda = removerRecursivo(no->esquerda, ponto, profundidade + 1);
    } else {
        no->direita = removerRecursivo(no->direita, ponto, profundidade + 1);
    }

    return no;
}


/// Impressão.

/// @brief Imprime a estrutura atual da KD-Tree de forma lateral,
/// mostrando o ponto e o eixo utilizado naquele nível.
void KDTree::imprimirKDTree() const {
    if (raiz == nullptr) {
        std::cout << "KD-Tree vazia.\n";
        return;
    }

    imprimirRecursivo(raiz,0,0);
}


/// @brief Função auxiliar que percorre recursivamente a árvore para
/// realizar sua impressão.
/// @param no Nó atual.
/// @param nivel Nível utilizado para a indentação.
/// @param profundidade Profundidade utilizada para identificar o eixo.
void KDTree::imprimirRecursivo(const Node* no, int nivel, int profundidade) const {
    if (no == nullptr) {
        return;
    }

    imprimirRecursivo(no->direita, nivel + 1, profundidade + 1);

    std::cout << std::string(nivel * 4, ' ') << "(" << no->ponto.x << ", " << no->ponto.y << ") [" << ((profundidade % 2 == 0) ? "X" : "Y") << "]" << '\n';

    imprimirRecursivo(no->esquerda,nivel + 1,profundidade + 1 );
}