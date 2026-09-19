#ifndef SPLAY_HPP
#define SPLAY_HPP

class Splay {
private:
    struct Node {
        int chave;

        Node* esquerda;
        Node* direita;
        Node* pai;

        Node(int chave)
            : chave(chave),
              esquerda(nullptr),
              direita(nullptr),
              pai(nullptr) {}
    };

    Node* raiz;

    void rotacaoDireita(Node* no);
    void rotacaoEsquerda(Node* no);

    void splay(Node* no);

    Node* buscarNo(int chave);

    Node* maiorNo(Node* no) const;

    void imprimirRecursivo(const Node* no, int nivel) const;

    void liberar(Node* no);

public:
    Splay();
    ~Splay();

    void inserirSplay(int chave);
    bool buscarSplay(int chave);
    void removerSplay(int chave);
    void imprimirSplay() const;
};

#endif