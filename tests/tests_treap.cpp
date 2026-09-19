#include "treap.hpp"

#include <cassert>
#include <iostream>

int main() {
    Treap treap;

    // Inserções controladas
    treap.inserirTreap(50, 50);
    treap.inserirTreap(30, 80);
    treap.inserirTreap(70, 70);
    treap.inserirTreap(20, 40);
    treap.inserirTreap(40, 60);
    treap.inserirTreap(60, 30);
    treap.inserirTreap(80, 20);

    // Teste de busca
    assert(treap.buscarTreap(50));
    assert(treap.buscarTreap(30));
    assert(treap.buscarTreap(70));
    assert(treap.buscarTreap(20));
    assert(treap.buscarTreap(80));

    assert(!treap.buscarTreap(100));
    assert(!treap.buscarTreap(-10));

    // Visualização antes das remoções
    std::cout << "=== Treap inicial ===\n";
    treap.imprimirTreap();

    // Remoção
    treap.removerTreap(20);

    assert(!treap.buscarTreap(20));
    assert(treap.buscarTreap(30));

    treap.removerTreap(50);

    assert(!treap.buscarTreap(50));
    assert(treap.buscarTreap(40));
    assert(treap.buscarTreap(60));
    assert(treap.buscarTreap(70));

    std::cout << "\n=== Treap apos remocoes ===\n";
    treap.imprimirTreap();

    // Chave que não existe
    treap.removerTreap(999);

    std::cout << "\nTeste da Treap concluido com sucesso.\n";

    return 0;
}