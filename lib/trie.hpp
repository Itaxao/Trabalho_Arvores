#ifndef TRIE_HPP
#define TRIE_HPP


#include <string>
#include <unordered_map>
#include <vector>


class Trie {
  private:
    struct Node {
      bool fimPalavra;
      std::unordered_map<char, Node*> filhos;

      Node() : fimPalavra(false) {}
    };
    Node *raiz;

    void liberar(Node *node);
    bool removerRecursivo(Node* no, const std::string& palavra, size_t indice);
    void imprimirRecursivo(const Node* no, int nivel) const;
    void coletarPalavras(const Node* no, const std::string& palavraAtual, std::vector<std::string>& palavras) const;

    public: 
      Trie();
      ~Trie();

      void inserirTrie(const std::string& palavra);
      bool buscarTrie(const std::string& palavra) const; 
      bool buscarPrefixoTrie(const std::string& prefixo) const;
      void removerTrie(const std::string& palavra);
      void imprimirTrie() const;
      std::vector<std::string> palavrasPrefixo(const std::string& prefixo) const;
};





#endif