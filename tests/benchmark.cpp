#include "trie.hpp"
#include "patricia.hpp"
#include "treap.hpp"
#include "splay.hpp"
#include "kdTree.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using Relogio = std::chrono::steady_clock;

constexpr int REPETICOES = 3;

struct Resultado {
    std::string estrutura;
    std::string operacao;
    std::string padrao;

    std::size_t tamanho;

    double tempoTotalUs;
    double tempoMedioUs;
};

struct EntradaTreap {
    int chave;
    int prioridade;
};


// ============================================================
// MEDIÇÃO
// ============================================================

template <typename Funcao>
double medirTempo(Funcao funcao) {
    auto inicio = Relogio::now();

    funcao();

    auto fim = Relogio::now();

    return std::chrono::duration<double, std::micro>(
        fim - inicio
    ).count();
}


void adicionarResultado(
    std::vector<Resultado>& resultados,
    const std::string& estrutura,
    const std::string& operacao,
    const std::string& padrao,
    std::size_t tamanho,
    double tempoTotal
) {
    resultados.push_back({
        estrutura,
        operacao,
        padrao,
        tamanho,
        tempoTotal,
        tempoTotal / static_cast<double>(tamanho)
    });
}


// ============================================================
// BENCHMARK GENÉRICO
// ============================================================

template <
    typename Estrutura,
    typename Item,
    typename Inserir,
    typename Buscar,
    typename Remover
>
void executarBenchmark(
    const std::string& nome,
    const std::string& padrao,
    const std::vector<Item>& dados,
    Inserir inserir,
    Buscar buscar,
    Remover remover,
    std::vector<Resultado>& resultados
) {
    double tempoInsercao = 0;
    double tempoBusca = 0;
    double tempoRemocao = 0;

    for (int repeticao = 0;
         repeticao < REPETICOES;
         repeticao++) {

        // -----------------------------
        // INSERÇÃO
        // -----------------------------

        {
            Estrutura estrutura;

            tempoInsercao += medirTempo([&]() {
                for (const auto& item : dados) {
                    inserir(estrutura, item);
                }
            });
        }


        // -----------------------------
        // BUSCA
        // -----------------------------

        {
            Estrutura estrutura;

            for (const auto& item : dados) {
                inserir(estrutura, item);
            }

            std::size_t encontrados = 0;

            tempoBusca += medirTempo([&]() {
                for (const auto& item : dados) {
                    if (buscar(estrutura, item)) {
                        encontrados++;
                    }
                }
            });

            // Também impede que o compilador simplesmente
            // descarte o resultado das buscas.
            if (encontrados != dados.size()) {
                std::cerr
                    << "Aviso: busca falhou em "
                    << nome << '\n';
            }
        }


        // -----------------------------
        // REMOÇÃO
        // -----------------------------

        {
            Estrutura estrutura;

            for (const auto& item : dados) {
                inserir(estrutura, item);
            }

            tempoRemocao += medirTempo([&]() {
                for (const auto& item : dados) {
                    remover(estrutura, item);
                }
            });
        }
    }

    tempoInsercao /= REPETICOES;
    tempoBusca /= REPETICOES;
    tempoRemocao /= REPETICOES;

    adicionarResultado(
        resultados,
        nome,
        "Insercao",
        padrao,
        dados.size(),
        tempoInsercao
    );

    adicionarResultado(
        resultados,
        nome,
        "Busca",
        padrao,
        dados.size(),
        tempoBusca
    );

    adicionarResultado(
        resultados,
        nome,
        "Remocao",
        padrao,
        dados.size(),
        tempoRemocao
    );
}


// ============================================================
// GERAÇÃO DE DADOS
// ============================================================

std::vector<int> gerarInteiros(
    std::size_t quantidade,
    bool aleatorio
) {
    std::vector<int> valores;

    valores.reserve(quantidade);

    for (std::size_t i = 0;
         i < quantidade;
         i++) {

        valores.push_back(
            static_cast<int>(i)
        );
    }

    if (aleatorio) {
        std::mt19937 gerador(42);

        std::shuffle(
            valores.begin(),
            valores.end(),
            gerador
        );
    }

    return valores;
}


/// Palavras com muito prefixo compartilhado.
std::vector<std::string> gerarPalavrasPrefixo(
    std::size_t quantidade
) {
    std::vector<std::string> palavras;

    palavras.reserve(quantidade);

    for (std::size_t i = 0;
         i < quantidade;
         i++) {

        palavras.push_back(
            "estrutura_dados_" +
            std::to_string(i)
        );
    }

    return palavras;
}


/// Palavras mais variadas.
std::vector<std::string> gerarPalavrasVariadas(
    std::size_t quantidade
) {
    std::vector<std::string> palavras;

    palavras.reserve(quantidade);

    std::mt19937 gerador(42);

    std::uniform_int_distribution<int>
        letras(0, 25);

    for (std::size_t i = 0;
         i < quantidade;
         i++) {

        std::string palavra;

        for (int j = 0; j < 8; j++) {
            palavra += static_cast<char>(
                'a' + letras(gerador)
            );
        }

        // Garante unicidade.
        palavra += "_" + std::to_string(i);

        palavras.push_back(palavra);
    }

    return palavras;
}


/// Treap com prioridades reproduzíveis.
std::vector<EntradaTreap> gerarEntradasTreap(
    const std::vector<int>& chaves
) {
    std::vector<EntradaTreap> entradas;

    entradas.reserve(chaves.size());

    std::mt19937 gerador(123);

    std::uniform_int_distribution<int>
        prioridades(1, 1000000000);

    for (int chave : chaves) {
        entradas.push_back({
            chave,
            prioridades(gerador)
        });
    }

    return entradas;
}


std::vector<KDTree::Ponto> gerarPontos(
    std::size_t quantidade,
    bool aleatorio
) {
    std::vector<KDTree::Ponto> pontos;

    pontos.reserve(quantidade);

    if (!aleatorio) {
        // Caso desfavorável: pontos na diagonal.
        for (std::size_t i = 0;
             i < quantidade;
             i++) {

            pontos.emplace_back(
                static_cast<double>(i),
                static_cast<double>(i)
            );
        }

        return pontos;
    }

    std::mt19937 gerador(42);

    std::uniform_real_distribution<double>
        coordenadas(
            0.0,
            static_cast<double>(quantidade * 10)
        );

    for (std::size_t i = 0;
         i < quantidade;
         i++) {

        pontos.emplace_back(
            coordenadas(gerador),
            coordenadas(gerador)
        );
    }

    return pontos;
}


// ============================================================
// SALVAR RESULTADOS
// ============================================================

void salvarCSV(
    const std::vector<Resultado>& resultados
) {
    std::filesystem::create_directories(
        "resultados"
    );

    std::ofstream arquivo(
        "resultados/benchmark.csv"
    );

    if (!arquivo.is_open()) {
        std::cerr
            << "Erro ao criar benchmark.csv\n";

        return;
    }

    arquivo
        << "estrutura,"
        << "operacao,"
        << "padrao,"
        << "tamanho,"
        << "tempo_total_us,"
        << "tempo_medio_us\n";

    arquivo
        << std::fixed
        << std::setprecision(6);

    for (const auto& resultado : resultados) {
        arquivo
            << resultado.estrutura << ','
            << resultado.operacao << ','
            << resultado.padrao << ','
            << resultado.tamanho << ','
            << resultado.tempoTotalUs << ','
            << resultado.tempoMedioUs
            << '\n';
    }
}


// ============================================================
// MAIN
// ============================================================

int main() {
    std::vector<std::size_t> tamanhos = {
        500,
        1000,
        2500,
        5000
    };

    std::vector<Resultado> resultados;


    for (std::size_t tamanho : tamanhos) {

        std::cout
            << "\n==============================\n"
            << "Tamanho: " << tamanho
            << "\n==============================\n";


        // ====================================================
        // TRIE + PATRICIA
        // ====================================================

        auto palavrasVariadas =
            gerarPalavrasVariadas(tamanho);

        auto palavrasPrefixo =
            gerarPalavrasPrefixo(tamanho);


        executarBenchmark<Trie>(
            "Trie",
            "Variado",
            palavrasVariadas,

            [](Trie& trie, const std::string& palavra) {
                trie.inserirTrie(palavra);
            },

            [](Trie& trie, const std::string& palavra) {
                return trie.buscarTrie(palavra);
            },

            [](Trie& trie, const std::string& palavra) {
                trie.removerTrie(palavra);
            },

            resultados
        );


        executarBenchmark<Trie>(
            "Trie",
            "PrefixoCompartilhado",
            palavrasPrefixo,

            [](Trie& trie, const std::string& palavra) {
                trie.inserirTrie(palavra);
            },

            [](Trie& trie, const std::string& palavra) {
                return trie.buscarTrie(palavra);
            },

            [](Trie& trie, const std::string& palavra) {
                trie.removerTrie(palavra);
            },

            resultados
        );


        executarBenchmark<Patricia>(
            "Patricia",
            "Variado",
            palavrasVariadas,

            [](Patricia& patricia, const std::string& palavra) {
                patricia.inserirPatricia(palavra);
            },

            [](Patricia& patricia, const std::string& palavra) {
                return patricia.buscarPatricia(palavra);
            },

            [](Patricia& patricia, const std::string& palavra) {
                patricia.removerPatricia(palavra);
            },

            resultados
        );


        executarBenchmark<Patricia>(
            "Patricia",
            "PrefixoCompartilhado",
            palavrasPrefixo,

            [](Patricia& patricia, const std::string& palavra) {
                patricia.inserirPatricia(palavra);
            },

            [](Patricia& patricia, const std::string& palavra) {
                return patricia.buscarPatricia(palavra);
            },

            [](Patricia& patricia, const std::string& palavra) {
                patricia.removerPatricia(palavra);
            },

            resultados
        );


        // ====================================================
        // TREAP + SPLAY
        // ====================================================

        auto inteirosOrdenados =
            gerarInteiros(tamanho, false);

        auto inteirosAleatorios =
            gerarInteiros(tamanho, true);

        auto treapOrdenada =
            gerarEntradasTreap(
                inteirosOrdenados
            );

        auto treapAleatoria =
            gerarEntradasTreap(
                inteirosAleatorios
            );


        executarBenchmark<Treap>(
            "Treap",
            "Ordenado",
            treapOrdenada,

            [](Treap& treap, const EntradaTreap& item) {
                treap.inserirTreap(
                    item.chave,
                    item.prioridade
                );
            },

            [](Treap& treap, const EntradaTreap& item) {
                return treap.buscarTreap(
                    item.chave
                );
            },

            [](Treap& treap, const EntradaTreap& item) {
                treap.removerTreap(
                    item.chave
                );
            },

            resultados
        );


        executarBenchmark<Treap>(
            "Treap",
            "Aleatorio",
            treapAleatoria,

            [](Treap& treap, const EntradaTreap& item) {
                treap.inserirTreap(
                    item.chave,
                    item.prioridade
                );
            },

            [](Treap& treap, const EntradaTreap& item) {
                return treap.buscarTreap(
                    item.chave
                );
            },

            [](Treap& treap, const EntradaTreap& item) {
                treap.removerTreap(
                    item.chave
                );
            },

            resultados
        );


        executarBenchmark<Splay>(
            "Splay",
            "Ordenado",
            inteirosOrdenados,

            [](Splay& splay, const int& chave) {
                splay.inserirSplay(chave);
            },

            [](Splay& splay, const int& chave) {
                return splay.buscarSplay(chave);
            },

            [](Splay& splay, const int& chave) {
                splay.removerSplay(chave);
            },

            resultados
        );


        executarBenchmark<Splay>(
            "Splay",
            "Aleatorio",
            inteirosAleatorios,

            [](Splay& splay, const int& chave) {
                splay.inserirSplay(chave);
            },

            [](Splay& splay, const int& chave) {
                return splay.buscarSplay(chave);
            },

            [](Splay& splay, const int& chave) {
                splay.removerSplay(chave);
            },

            resultados
        );


        // ====================================================
        // KD-TREE
        // ====================================================

        auto pontosAleatorios =
            gerarPontos(tamanho, true);

        auto pontosDiagonais =
            gerarPontos(tamanho, false);


        executarBenchmark<KDTree>(
            "KDTree",
            "Aleatorio",
            pontosAleatorios,

            [](KDTree& kd, const KDTree::Ponto& ponto) {
                kd.inserirKDTree(
                    ponto.x,
                    ponto.y
                );
            },

            [](KDTree& kd, const KDTree::Ponto& ponto) {
                return kd.buscarKDTree(
                    ponto.x,
                    ponto.y
                );
            },

            [](KDTree& kd, const KDTree::Ponto& ponto) {
                kd.removerKDTree(
                    ponto.x,
                    ponto.y
                );
            },

            resultados
        );


        executarBenchmark<KDTree>(
            "KDTree",
            "Diagonal",
            pontosDiagonais,

            [](KDTree& kd, const KDTree::Ponto& ponto) {
                kd.inserirKDTree(
                    ponto.x,
                    ponto.y
                );
            },

            [](KDTree& kd, const KDTree::Ponto& ponto) {
                return kd.buscarKDTree(
                    ponto.x,
                    ponto.y
                );
            },

            [](KDTree& kd, const KDTree::Ponto& ponto) {
                kd.removerKDTree(
                    ponto.x,
                    ponto.y
                );
            },

            resultados
        );


        std::cout << "Concluido.\n";
    }


    salvarCSV(resultados);


    std::cout
        << "\n==============================\n"
        << "Benchmark concluido.\n"
        << "Resultados: resultados/benchmark.csv\n"
        << "==============================\n";

    return 0;
}