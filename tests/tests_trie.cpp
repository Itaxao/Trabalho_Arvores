#include "trie.hpp"

#include <cassert>
#include <iostream>

int main() {
    Trie trie;

    // Inserção
    trie.inserirTrie("casa");
    trie.inserirTrie("casaco");
    trie.inserirTrie("carro");
    trie.inserirTrie("bola");

    // Busca
    assert(trie.buscarTrie("casa"));
    assert(trie.buscarTrie("casaco"));
    assert(!trie.buscarTrie("cas"));
    assert(!trie.buscarTrie("banana"));

    // Prefixos
    assert(trie.buscarPrefixoTrie("ca"));
    assert(trie.buscarPrefixoTrie("cas"));
    assert(trie.buscarPrefixoTrie("bol"));
    assert(!trie.buscarPrefixoTrie("xyz"));

    // Remoção compartilhando prefixo
    trie.removerTrie("casa");

    assert(!trie.buscarTrie("casa"));
    assert(trie.buscarTrie("casaco"));

    // Remoção de caminho exclusivo
    trie.removerTrie("bola");

    assert(!trie.buscarTrie("bola"));
    assert(!trie.buscarPrefixoTrie("bol"));

    std::cout << "Teste da Trie concluido com sucesso.\n";

    return 0;
}