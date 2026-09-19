#include "patricia.hpp"

#include <iostream>
#include <string>
#include <utility>


/// Funções básicas de construção, destruição e liberação dos nós.

Patricia::Patricia() {
    raiz = new Node();
}

Patricia::~Patricia() {
    liberar(raiz);
}

/// @brief Libera recursivamente todos os nós existentes na árvore.
/// @param no Nó atual que está sendo liberado.
void Patricia::liberar(Node* no) {
    if (no == nullptr) {
        return;
    }

    for (auto& filho : no->filhos) {
        liberar(filho.second);
    }

    delete no;
}


/// Funções auxiliares.

/// @brief Calcula quantos caracteres iniciais duas strings possuem em comum.
/// @param a Primeira string.
/// @param b Segunda string.
/// @return Quantidade de caracteres iguais no início das duas strings.
size_t Patricia::prefixoComum(
    const std::string& a,
    const std::string& b
) const {

    size_t tamanho = 0;

    while (
        tamanho < a.size() &&
        tamanho < b.size() &&
        a[tamanho] == b[tamanho]
    ) {
        tamanho++;
    }

    return tamanho;
}


/// Inserção.

/// @brief Insere uma nova palavra na Patricia. Diferente da Trie,
/// cada nó pode armazenar um conjunto de caracteres. Caso duas palavras
/// possuam apenas parte do prefixo em comum, o nó existente é dividido.
/// @param palavra Palavra que será inserida.
void Patricia::inserirPatricia(
    const std::string& palavra
) {

    if (palavra.empty()) {
        raiz->fimPalavra = true;
        return;
    }

    Node* no_atual = raiz;

    size_t indice = 0;

    while (indice < palavra.size()) {

        char primeiraLetra = palavra[indice];

        auto procura =
            no_atual->filhos.find(primeiraLetra);

        // Não existe nenhum caminho começando
        // por essa letra.
        if (procura == no_atual->filhos.end()) {

            Node* novo =
                new Node(palavra.substr(indice));

            novo->fimPalavra = true;

            no_atual->filhos[primeiraLetra] =
                novo;

            return;
        }

        Node* filho = procura->second;

        std::string restante =
            palavra.substr(indice);

        size_t comum =
            prefixoComum(
                restante,
                filho->prefixo
            );


        // Todo o prefixo do nó atual foi encontrado.
        // Então continuamos descendo pela árvore.
        if (comum == filho->prefixo.size()) {

            indice += comum;
            no_atual = filho;

            continue;
        }


        // Caso apenas uma parte seja igual,
        // precisamos dividir o nó.
        Node* intermediario =
            new Node(
                filho->prefixo.substr(
                    0,
                    comum
                )
            );

        // Remove do filho a parte que passou
        // para o nó intermediário.
        filho->prefixo =
            filho->prefixo.substr(comum);

        // O antigo nó passa a ser filho
        // do intermediário.
        intermediario->filhos[
            filho->prefixo[0]
        ] = filho;

        // Substitui o antigo nó pelo intermediário.
        no_atual->filhos[primeiraLetra] =
            intermediario;

        indice += comum;


        // A nova palavra termina exatamente
        // no ponto onde ocorreu a divisão.
        if (indice == palavra.size()) {

            intermediario->fimPalavra = true;
        }

        else {

            Node* novo =
                new Node(
                    palavra.substr(indice)
                );

            novo->fimPalavra = true;

            intermediario->filhos[
                novo->prefixo[0]
            ] = novo;
        }

        return;
    }

    // Caso a palavra já possua todo seu caminho
    // representado na árvore.
    no_atual->fimPalavra = true;
}


/// Busca.

/// @brief Busca uma palavra na Patricia percorrendo os prefixos
/// armazenados em cada nó.
/// @param palavra Palavra que será buscada.
/// @return true caso a palavra exista e false caso contrário.
bool Patricia::buscarPatricia(
    const std::string& palavra
) const {

    if (raiz == nullptr) {
        return false;
    }

    if (palavra.empty()) {
        return raiz->fimPalavra;
    }

    const Node* no_atual = raiz;

    size_t indice = 0;

    while (indice < palavra.size()) {

        auto procura =
            no_atual->filhos.find(
                palavra[indice]
            );

        if (procura == no_atual->filhos.end()) {
            return false;
        }

        const Node* filho = procura->second;

        // Se o prefixo do nó for maior que
        // o pedaço restante da palavra,
        // ela não pode ser igual.
        if (
            indice + filho->prefixo.size()
            > palavra.size()
        ) {
            return false;
        }

        // Verifica se todo o prefixo armazenado
        // no nó corresponde à palavra buscada.
        if (
            palavra.compare(
                indice,
                filho->prefixo.size(),
                filho->prefixo
            ) != 0
        ) {
            return false;
        }

        indice += filho->prefixo.size();

        no_atual = filho;
    }

    return no_atual->fimPalavra;
}


/// Remoção.

/// @brief Remove uma palavra da Patricia e reorganiza os nós,
/// compactando novamente caminhos que deixarem de precisar de divisões.
/// @param palavra Palavra que será removida.
void Patricia::removerPatricia(
    const std::string& palavra
) {

    if (raiz == nullptr) {
        return;
    }

    bool removida = false;

    removerRecursivo(
        raiz,
        palavra,
        0,
        removida,
        true
    );
}


/// @brief Função auxiliar responsável pela remoção e pela compactação
/// dos nós depois que uma palavra é retirada.
/// @param no Nó atual.
/// @param palavra Palavra que será removida.
/// @param indice Posição atual dentro da palavra.
/// @param removida Indica se a palavra realmente foi encontrada e removida.
/// @param ehRaiz Informa se o nó atual é a raiz da Patricia.
/// @return true caso o nó atual tenha se tornado desnecessário e possa
/// ser apagado pelo nó pai.
bool Patricia::removerRecursivo(
    Node* no,
    const std::string& palavra,
    size_t indice,
    bool& removida,
    bool ehRaiz
) {

    // Chegamos ao final da palavra.
    if (indice == palavra.size()) {

        if (!no->fimPalavra) {
            return false;
        }

        no->fimPalavra = false;

        removida = true;
    }

    else {

        auto procura =
            no->filhos.find(
                palavra[indice]
            );

        if (procura == no->filhos.end()) {
            return false;
        }

        Node* filho = procura->second;

        // Verifica se o prefixo inteiro do filho
        // corresponde ao trecho atual da palavra.
        if (
            indice + filho->prefixo.size()
            > palavra.size()
        ) {
            return false;
        }

        if (
            palavra.compare(
                indice,
                filho->prefixo.size(),
                filho->prefixo
            ) != 0
        ) {
            return false;
        }

        bool apagarFilho =
            removerRecursivo(
                filho,
                palavra,
                indice + filho->prefixo.size(),
                removida,
                false
            );

        if (apagarFilho) {

            delete filho;

            no->filhos.erase(procura);
        }
    }


    // Se nada foi removido, não precisamos
    // modificar a estrutura.
    if (!removida) {
        return false;
    }


    // A raiz nunca deve ser apagada ou compactada
    // com um filho.
    if (ehRaiz) {
        return false;
    }


    // Nó que não representa palavra e não possui
    // filhos deixou de ser necessário.
    if (
        !no->fimPalavra &&
        no->filhos.empty()
    ) {
        return true;
    }


    // Se o nó não representa o fim de uma palavra
    // e possui apenas um filho, podemos juntar
    // os dois prefixos novamente.
    if (
        !no->fimPalavra &&
        no->filhos.size() == 1
    ) {

        Node* unico =
            no->filhos.begin()->second;

        no->prefixo += unico->prefixo;

        no->fimPalavra =
            unico->fimPalavra;

        no->filhos =
            std::move(unico->filhos);

        delete unico;
    }

    return false;
}


/// Impressão.

/// @brief Imprime a estrutura atual da Patricia mostrando os prefixos
/// armazenados em cada nó. Nós marcados com * representam o fim de uma palavra.
void Patricia::imprimirPatricia() const {

    if (raiz == nullptr) {
        std::cout << "Patricia vazia.\n";
        return;
    }

    std::cout << "[raiz]";

    if (raiz->fimPalavra) {
        std::cout << " *";
    }

    std::cout << '\n';

    imprimirRecursivo(
        raiz,
        1
    );
}


/// @brief Percorre recursivamente a Patricia para realizar sua impressão.
/// @param no Nó atual.
/// @param nivel Nível utilizado para organizar a indentação.
void Patricia::imprimirRecursivo(
    const Node* no,
    int nivel
) const {

    for (const auto& filho : no->filhos) {

        const Node* proximoNo =
            filho.second;

        std::cout
            << std::string(nivel * 4, ' ')
            << proximoNo->prefixo;

        if (proximoNo->fimPalavra) {
            std::cout << " *";
        }

        std::cout << '\n';

        imprimirRecursivo(
            proximoNo,
            nivel + 1
        );
    }
}