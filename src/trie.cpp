#include "lib/trie.hpp"

Trie::Trie() {
  raiz = new Node();
}

/// @brief Insere nova palavra na estrutura, faz isso percorrendo e criando novos nós a cada iteração, sendo cada uma das iterações uma das letras da palavra até o final onde ele cria um ponteiro que marca qual o final daquela palavra.
/// @param palavra Palavra a ser inserida
void Trie::inserirTrie(const std::string& palavra) {
  Node* no_atual = raiz;
  for (char c : palavra){
      if (no_atual->filhos[c] == nullptr){
        no_atual->filhos[c] == new Node();
      }

      no_atual = no_atual->filhos[c];
  }

  no_atual->fimPalavra = true;
}

/// @brief Percorre a atual arvore a cada iteração até o fim da palavra pedida, onde ele verifica se o nó representa o fim de uma palavra e se esse fim da palavra é a o fim da palavra que nós queremos, saco?.
/// @param palavra Palavra a ser buscada.
/// @return true, se a palavra está e false caso não exista na árvore.
bool Trie::buscarTrie(const std::string& palavra) const{
  if (raiz == nullptr){
    return 0;
  }

  const Node* no_atual = raiz;
  for (char c : palavra){

    auto procuraLetira = no_atual->filhos.find(c);
    
    if (procuraLetira == no_atual->filhos.end()){ return 0; }
    
    no_atual = procuraLetira->second;
  }

  return no_atual->fimPalavra;
}


/// @brief Basicamente a mesma coisa da função buscarTrie, onde ela pesquisa se existe a palavra na qual queremos, nesse caso um prefixo. O que muda é apenas o retorno pois não precisamos verificar se o final da palavra é um fimPalavra, apenas percorrer ela é o suficiente.
/// @param prefixo Qual prefixo buscar
/// @return true se existe o prefixox na arvore, false se não.
bool Trie::buscarPrefixoTrie(const std::string& prefixo) const {
      if (raiz == nullptr) {
        return false;
    }

    const Node* no_atual = raiz;

    for (char c : prefixo) {
        auto procuraLetra = no_atual->filhos.find(c);

        if (procuraLetra == no_atual->filhos.end()) {
            return false;
        }

        no_atual = procuraLetra->second;
    }

    return true;
}


/// @brief Função que remove uma palavra da Trie, para fazer isso basicamente, busca na árvore assim como as duas funções acima e retira cada letra/no recursivamante, fazendo a cada interação se o no atual tem filhos ou se é fim de uma outra palavra, se qualquer um dos casos for verdade, é falso
/// @param palavra 
void Trie::removerTrie(const std::string& palavra) {
  if (raiz == nullptr){
    return;
  }

  removerRecursivo(raiz, palavra, 0);
}

/// @brief Função auxiliar que remove uma palavra da Trie, para fazer isso basicamente, busca na árvore assim como as duas funções acima e retira cada letra/no recursivamante, fazendo a cada interação se o no atual tem filhos ou se é fim de uma outra palavra, se qualquer um dos casos for verdade, é falso
/// @param no 
/// @param palavra 
/// @param indice 
/// @return 
bool Trie::removerRecursivo(Node* no, const std::string& palavra, size_t indice){

    if (indice == palavra.size()) {

        if (!no->fimPalavra) {
            return false;
        }
        no->fimPalavra = false;

        return no->filhos.empty();
    }

    char c = palavra[indice];

    auto procuraLetra = no->filhos.find(c);

    if (procuraLetra == no->filhos.end()) {
        return false;
    }

    Node* filho = procuraLetra->second;

    bool podeApagarFilho =
        removerRecursivo(filho, palavra, indice + 1);

    if (podeApagarFilho) {
        delete filho;
        no->filhos.erase(c);
    }
    return !no->fimPalavra && no->filhos.empty();
}


