#include "trie.hpp"
#include <iostream>


/// Funções básicas de construção, destrução e liberação de nós.
Trie::Trie() {
  raiz = new Node();
}

Trie::~Trie(){
    liberar(raiz);
}

void Trie::liberar(Node* no) {
    if (no == nullptr) {
        return;
    }

    for (auto& filho : no->filhos) {
        liberar(filho.second);
    }

    delete no;
}



/// Funções da Trie, inserção, busca, ..., Impressão.

/// @brief Insere nova palavra na estrutura, faz isso percorrendo e criando novos nós a cada iteração, sendo cada uma das iterações uma das letras da palavra até o final onde ele cria um ponteiro que marca qual o final daquela palavra.
/// @param palavra Palavra a ser inserida
void Trie::inserirTrie(const std::string& palavra) {
  Node* no_atual = raiz;
  for (char c : palavra){
      if (no_atual->filhos[c] == nullptr){
        no_atual->filhos[c] = new Node();
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

/// @brief Imprime a estrutura atual da Trie, começando pela raiz e chamando uma função recursiva para percorrer todos os filhos da árvore.
/// @return Não possui retorno.
void Trie::imprimirTrie() const {
    if (raiz == nullptr) {
        std::cout << "Trie vazia.\n";
        return;
    }

    std::cout << "[raiz]\n";
    imprimirRecursivo(raiz, 1);
}

/// @brief Função auxiliar da imprimirTrie, percorre recursivamente cada nó da árvore mostrando a letra atual e o nível em que ela se encontra. Caso o nó represente o fim de uma palavra, ele é marcado com *.
/// @param no Nó atual que está sendo percorrido.
/// @param nivel Nível atual do nó dentro da Trie, utilizado para organizar a impressão.
void Trie::imprimirRecursivo(const Node* no, int nivel) const {
    for (const auto& filho : no->filhos) {
        char letra = filho.first;
        const Node* proximoNo = filho.second;

        std::cout << std::string(nivel * 2, ' ');
        std::cout << letra;

        if (proximoNo->fimPalavra) {
            std::cout << " *";
        }

        std::cout << '\n';

        imprimirRecursivo(proximoNo, nivel + 1);
    }
}

/// @brief Busca todas as palavras existentes na Trie que começam com o prefixo informado. Primeiro percorre a árvore até o final do prefixo e, caso ele exista, coleta recursivamente todas as palavras formadas a partir daquele ponto.
/// @param prefixo Prefixo utilizado como base para a busca das palavras.
/// @return Vetor contendo todas as palavras encontradas que possuem o prefixo informado.
std::vector<std::string> Trie::palavrasPrefixo(const std::string& prefixo) const {
    std::vector<std::string> palavras;

    if (raiz == nullptr) {
        return palavras;
    }

    const Node* no_atual = raiz;

    for (char c : prefixo) {
        auto procuraLetra = no_atual->filhos.find(c);

        if (procuraLetra == no_atual->filhos.end()) {
            return palavras;
        }

        no_atual = procuraLetra->second;
    }

    coletarPalavras(no_atual, prefixo, palavras);

    return palavras;
}

/// @brief Função auxiliar que percorre recursivamente os nós a partir de um prefixo já encontrado, montando as palavras e adicionando no vetor sempre que encontra um nó marcado como fimPalavra.
/// @param no Nó atual que está sendo percorrido.
/// @param palavraAtual Palavra que está sendo formada durante o percurso da árvore.
/// @param palavras Vetor onde são armazenadas as palavras encontradas.
void Trie::coletarPalavras(const Node* no, const std::string& palavraAtual, std::vector<std::string>& palavras) const {
    if (no->fimPalavra) {
        palavras.push_back(palavraAtual);
    }

    for (const auto& filho : no->filhos) {
        char letra = filho.first;
        const Node* proximoNo = filho.second;

        coletarPalavras(
            proximoNo,
            palavraAtual + letra,
            palavras
        );
    }
}