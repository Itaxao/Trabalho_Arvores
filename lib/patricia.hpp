#ifndef PATRICIA_HPP
#define PATRICIA_HPP

#include <string>
#include <unordered_map>

class Patricia {
private:
    struct Node {
        std::string prefixo;
        bool fimPalavra;

        std::unordered_map<char, Node*> filhos;

        Node(const std::string& prefixo = "")
            : prefixo(prefixo),
              fimPalavra(false) {}
    };

    Node* raiz;

    size_t prefixoComum(
        const std::string& a,
        const std::string& b
    ) const;

    bool removerRecursivo(
        Node* no,
        const std::string& palavra,
        size_t indice,
        bool& removida,
        bool ehRaiz
    );

    void imprimirRecursivo(
        const Node* no,
        int nivel
    ) const;

    void liberar(Node* no);

public:
    Patricia();
    ~Patricia();

    void inserirPatricia(const std::string& palavra);

    bool buscarPatricia(const std::string& palavra) const;

    void removerPatricia(const std::string& palavra);

    void imprimirPatricia() const;
};

#endif