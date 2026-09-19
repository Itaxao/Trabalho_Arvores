#include "treap.hpp"

#include <iostream>
#include <string>

/// Funções básicas de construção, destrução e liberação de nós das árvores.
Treap::Treap()
    : raiz(nullptr),
      gerador(std::random_device{}()) {
}

Treap::~Treap() {
    liberar(raiz);
}

void Treap::liberar(Node* no) {
    if (no == nullptr) {
        return;
    }

    liberar(no->esquerda);
    liberar(no->direita);

    delete no;
}

/// Funções necessárias da árvore Treap.

/// @brief Faz uma rotação simples a direita, movendo o filho esquerdo e transformando-o na nova raiz da subarvore.
/// @param no_raiz Raiz atual da subarvore
/// @return nova raiz da subarvore apos rotacao
Treap::Node* Treap::rotacaoDireita(Node* no_raiz){
  Node* filho = no_raiz->esquerda;
  
  no_raiz->esquerda = filho->direita;
 
  filho->direita = no_raiz;

  return filho;
}

/// @brief faz uma rotaçã~o simples a esquerda, movendo o filho direita e transformando-o na nova raiz da subarvore.
/// @param no_raiz raiz atual da subarvores
/// @return nova raiz da subarvore apos rotacao.
Treap::Node* Treap::rotacaoEsquerda(Node* no_raiz){
  Node* filho = no_raiz->direita;

  no_raiz->direita = filho->esquerda;
  filho->esquerda = no_raiz;

  return filho;
}

/// @brief Insere na treap colocando prioridade e chave manualmente, util principalmente para testes controlados que são realizados
/// @param chave chave do no
/// @param prioridade prioridade do no
void Treap::inserirTreap(int chave, int prioridade){
  raiz = inserirRecursivo(raiz, chave, prioridade);
}

/// @brief Insere na treap sem definir prioridade manualmente, de forma que só utiliza como paramentro a chave e a partir de um valor aleatorio gera uma prioridade qualquer.
/// @param chave do no
void Treap::inserirTreap(int chave) {
    std::uniform_int_distribution<int> distribuicao(1,1000000);

    int prioridade = distribuicao(gerador);
    raiz = inserirRecursivo(raiz, chave, prioridade);
}

/// @brief Insere recursivamente na treap, utilizando a inserção simples da BST + um sistema de rotação parecido com a da AVL para manter as propriedades da Heap.
/// @param no_raiz no raiz da subarvore atual
/// @param chave chave do no
/// @param prioridade prioridade do no
/// @return no_raiz
Treap::Node* Treap::inserirRecursivo(Node* no_raiz, int chave, int prioridade){
 
  if (no_raiz == nullptr){
    return new Node(chave, prioridade);
  }

  if (chave < no_raiz->chave){
    no_raiz->esquerda = inserirRecursivo(no_raiz->esquerda,chave, prioridade);
    
    if(no_raiz->esquerda->prioridade > no_raiz->prioridade){
      no_raiz = rotacaoDireita(no_raiz);
    }
  } else if (chave > no_raiz->chave) {
    no_raiz->direita = inserirRecursivo(no_raiz->direita, chave, prioridade);
    
    if (no_raiz->direita->prioridade > no_raiz->prioridade){
      no_raiz = rotacaoEsquerda(no_raiz);
    }
  }

  return no_raiz;
}

/// @brief 
/// @param chave 
/// @return 
bool Treap::buscarTreap(int chave) const {
    return buscarRecursivo(raiz, chave) != nullptr;
}

/// @brief 
/// @param no_raiz 
/// @param chave 
/// @return 
const Treap::Node* Treap::buscarRecursivo(const Node* no_raiz, int chave) const {
  if (no_raiz == nullptr){
    return nullptr;
  }

  if (chave == no_raiz->chave){
    return no_raiz;
  }

  if (chave < no_raiz->chave){
    return buscarRecursivo(no_raiz->esquerda, chave);
  }
  
  return buscarRecursivo(no_raiz->direita, chave);
}

/// @brief 
/// @param chave 
void Treap::removerTreap(int chave) {
    raiz = removerRecursivo(raiz, chave);
}
/// @brief 
/// @param no_raiz 
/// @param chave 
/// @return 
Treap::Node* Treap::removerRecursivo(Node* no_raiz, int chave) {
    if (no_raiz == nullptr) {
        return nullptr;
    }

    if (chave < no_raiz->chave) {
      no_raiz->esquerda = removerRecursivo(no_raiz->esquerda, chave);
    } else if (chave > no_raiz->chave) {
        no_raiz->direita = removerRecursivo(no_raiz->direita, chave);
    } else {
        if (no_raiz->esquerda == nullptr && no_raiz->direita == nullptr) {
          delete no_raiz;
          return nullptr;
        }

        else if (no_raiz->esquerda == nullptr) {
            no_raiz = rotacaoEsquerda(no_raiz);
            no_raiz->esquerda = removerRecursivo( no_raiz->esquerda, chave);
        }

        else if (no_raiz->direita == nullptr) {
            no_raiz = rotacaoDireita(no_raiz);
            no_raiz->direita = removerRecursivo(no_raiz->direita, chave);
        }

        else {
            if (no_raiz->esquerda->prioridade > no_raiz->direita->prioridade) {
                no_raiz = rotacaoDireita(no_raiz);
                no_raiz->direita = removerRecursivo(no_raiz->direita, chave);
            } else {
                no_raiz = rotacaoEsquerda(no_raiz);
                no_raiz->esquerda = removerRecursivo(no_raiz->esquerda, chave);
              }
        }
    }

    return no_raiz;
}

/// @brief 
void Treap::imprimirTreap() const {
    if (raiz == nullptr) {
        std::cout << "Treap vazia.\n";
        return;
    }

    imprimirRecursivo(raiz, 0);
}

/// @brief 
/// @param no 
/// @param nivel 
void Treap::imprimirRecursivo(const Node* no, int nivel) const {
    if (no == nullptr) {
        return;
    }
    
    imprimirRecursivo(no->direita, nivel + 1);
    std::cout<< std::string(nivel * 4, ' ')<< no->chave<< " (p=" << no->prioridade << ")"<< '\n';
    imprimirRecursivo(no->esquerda,nivel + 1);
}