#include "kdTree.hpp"

#include <cassert>
#include <iostream>

int main() {

    KDTree kd;

    kd.inserirKDTree(30, 40);
    kd.inserirKDTree(5, 25);
    kd.inserirKDTree(70, 70);
    kd.inserirKDTree(10, 12);
    kd.inserirKDTree(50, 30);
    kd.inserirKDTree(35, 45);

    assert(kd.buscarKDTree(30, 40));
    assert(kd.buscarKDTree(5, 25));
    assert(kd.buscarKDTree(35, 45));

    assert(!kd.buscarKDTree(100, 100));

    std::cout << "=== KD-Tree inicial ===\n";
    kd.imprimirKDTree();

    kd.removerKDTree(5, 25);

    assert(!kd.buscarKDTree(5, 25));

    assert(kd.buscarKDTree(30, 40));
    assert(kd.buscarKDTree(10, 12));
    assert(kd.buscarKDTree(70, 70));

    std::cout << "\n=== KD-Tree apos remocao ===\n";
    kd.imprimirKDTree();

    std::cout << "\nTeste da KD-Tree concluido com sucesso.\n";

    return 0;
}