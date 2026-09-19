#ifndef KDTREE_HPP
#define KDTREE_HPP

class KDTree {
public:
    struct Ponto {
        double x;
        double y;

        Ponto(double x = 0, double y = 0)
            : x(x), y(y) {}
    };

private:
    struct Node {
        Ponto ponto;

        Node* esquerda;
        Node* direita;

        Node(const Ponto& ponto)
            : ponto(ponto),
              esquerda(nullptr),
              direita(nullptr) {}
    };

    Node* raiz;

    Node* inserirRecursivo(
        Node* no,
        const Ponto& ponto,
        int profundidade
    );

    bool buscarRecursivo(
        const Node* no,
        const Ponto& ponto,
        int profundidade
    ) const;

    Node* removerRecursivo(
        Node* no,
        const Ponto& ponto,
        int profundidade
    );

    Node* encontrarMinimo(
        Node* no,
        int eixo,
        int profundidade
    ) const;

    double valorEixo(
        const Ponto& ponto,
        int eixo
    ) const;

    bool pontosIguais(
        const Ponto& a,
        const Ponto& b
    ) const;

    void imprimirRecursivo(
        const Node* no,
        int nivel,
        int profundidade
    ) const;

    void liberar(Node* no);

public:
    KDTree();
    ~KDTree();

    void inserirKDTree(double x, double y);

    bool buscarKDTree(double x, double y) const;

    void removerKDTree(double x, double y);

    void imprimirKDTree() const;
};

#endif