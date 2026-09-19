#include "patricia.hpp"

#include <cassert>
#include <iostream>

int main() {

    Patricia patricia;

    patricia.inserirPatricia("casa");
    patricia.inserirPatricia("casaco");
    patricia.inserirPatricia("casamento");
    patricia.inserirPatricia("carro");
    patricia.inserirPatricia("bola");

    assert(patricia.buscarPatricia("casa"));
    assert(patricia.buscarPatricia("casaco"));
    assert(patricia.buscarPatricia("casamento"));
    assert(patricia.buscarPatricia("carro"));
    assert(patricia.buscarPatricia("bola"));

    assert(!patricia.buscarPatricia("cas"));
    assert(!patricia.buscarPatricia("banana"));

    std::cout
        << "=== Patricia inicial ===\n";

    patricia.imprimirPatricia();


    // Remove palavra que compartilha prefixo.
    patricia.removerPatricia("casa");

    assert(!patricia.buscarPatricia("casa"));

    assert(
        patricia.buscarPatricia("casaco")
    );

    assert(
        patricia.buscarPatricia("casamento")
    );


    std::cout
        << "\n=== Depois de remover casa ===\n";

    patricia.imprimirPatricia();


    // Remove caminho independente.
    patricia.removerPatricia("bola");

    assert(!patricia.buscarPatricia("bola"));

    // Palavra inexistente não deve causar problema.
    patricia.removerPatricia("banana");

    assert(patricia.buscarPatricia("carro"));
    assert(patricia.buscarPatricia("casaco"));

    std::cout
        << "\n=== Estado final ===\n";

    patricia.imprimirPatricia();

    std::cout
        << "\nTeste da Patricia concluido com sucesso.\n";

    return 0;
}