#include "splay.hpp"

#include <cassert>
#include <iostream>

int main() {

    Splay splay;

    // Inserções
    splay.inserirSplay(50);
    splay.inserirSplay(30);
    splay.inserirSplay(70);
    splay.inserirSplay(20);
    splay.inserirSplay(40);
    splay.inserirSplay(60);
    splay.inserirSplay(80);

    std::cout << "=== Splay apos insercoes ===\n";
    splay.imprimirSplay();

    // Buscas
    assert(splay.buscarSplay(50));
    assert(splay.buscarSplay(30));
    assert(splay.buscarSplay(80));

    assert(!splay.buscarSplay(100));
    assert(!splay.buscarSplay(-10));
    std::cout << "\n=== Antes de buscar 20 ===\n";
    splay.imprimirSplay();

    assert(splay.buscarSplay(20));

    std::cout << "\n=== Depois de buscar 20 ===\n";
    splay.imprimirSplay();

    // Remoção
    splay.removerSplay(20);

    assert(!splay.buscarSplay(20));

    assert(splay.buscarSplay(30));
    assert(splay.buscarSplay(40));
    assert(splay.buscarSplay(50));
    assert(splay.buscarSplay(60));
    assert(splay.buscarSplay(70));
    assert(splay.buscarSplay(80));

    std::cout << "\n=== Depois de remover 20 ===\n";
    splay.imprimirSplay();

    // Removendo outro elemento
    splay.removerSplay(50);

    assert(!splay.buscarSplay(50));

    // Remover algo que não existe não deve causar problemas.
    splay.removerSplay(999);

    std::cout << "\n=== Estado final ===\n";
    splay.imprimirSplay();

    std::cout << "\nTeste da Splay concluido com sucesso.\n";

    return 0;
}