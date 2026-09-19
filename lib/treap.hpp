#ifndef TREAP_HPP
#define TREAP_HPP

#include <random>

class Treap {
private:
    struct Node {
        int chave;
        int prioridade;

        Node* esquerda;
        Node* direita;

        Node(int chave, int prioridade)
            : chave(chave),
              prioridade(prioridade),
              esquerda(nullptr),
              direita(nullptr) {}
    };
    Node* raiz;

    std::mt19937 gerador;

    Node* rotacaoDireita(Node* no);
    Node* rotacaoEsquerda(Node* no);
    Node* inserirRecursivo(Node* no, int chave, int prioridade);
    Node* removerRecursivo(Node* no, int chave);
    const Node* buscarRecursivo(const Node* no, int chave) const;
    void imprimirRecursivo(const Node* no, int nivel) const;
    void liberar(Node* no);

public:
    Treap();
    ~Treap();

    void inserirTreap(int chave);
    void inserirTreap(int chave, int prioridade);
    bool buscarTreap(int chave) const;
    void removerTreap(int chave);
    void imprimirTreap() const;
};

#endif