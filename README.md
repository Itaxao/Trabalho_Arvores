# Trabalho Prático - Estruturas em Árvores Avançadas

Implementação e análise experimental de cinco estruturas de dados hierárquicas em C++17:

- Trie;
- Árvore Patricia (Radix Tree compacta);
- Árvore Splay;
- Treap (Tree + Heap);
- KD-Tree bidimensional.

O projeto foi desenvolvido para comparar diferentes estratégias de organização, busca, reorganização e indexação de dados. Além das implementações, o repositório contém testes funcionais, benchmark reproduzível, resultados em CSV e scripts para geração dos gráficos e diagramas utilizados no relatório.

**Repositório:** https://github.com/Itaxao/Trabalho_Arvores

## Estrutura do projeto

```text
.
├── lib/                    # Arquivos de cabeçalho (.hpp)
├── src/                    # Implementações (.cpp)
│   └── main.cpp            # Executável principal mínimo
├── tests/                  # Testes funcionais e benchmark
├── scripts/
│   ├── graficos.py         # Gráficos do benchmark
│   └── diagramas.py        # Diagramas estruturais das árvores
├── resultados/
│   ├── benchmark.csv       # Resultados experimentais
│   ├── graficos/           # Gráficos gerados a partir do CSV
│   └── diagramas/          # Estados visuais das estruturas
├── Makefile
└── README.md
```

O relatório acadêmico é entregue separadamente e referencia este repositório. Por isso ele não precisa permanecer em uma pasta `docs/` dentro do GitHub.

## Requisitos

Para compilar as estruturas e os testes:

- compilador compatível com C++17 (`g++`);
- GNU Make.

Para gerar os gráficos:

- Python 3;
- pandas;
- matplotlib.

Para gerar os diagramas estruturais:

- Graphviz (`dot`);
- pacote Python `graphviz`.

### Ambiente Nix/NixOS

É possível abrir um ambiente temporário com as dependências usando:

```bash
nix-shell -p gcc gnumake graphviz python3 \
  python3Packages.pandas python3Packages.matplotlib python3Packages.graphviz
```

Em outras distribuições, instale `g++`, `make`, Python 3 e Graphviz pelo gerenciador de pacotes. Para os pacotes Python, uma opção é:

```bash
python3 -m pip install pandas matplotlib graphviz
```

## Compilação

Na raiz do projeto:

```bash
make
```

O executável principal será criado em:

```text
bin/arvores
```

Para executá-lo:

```bash
make run
```

O `main.cpp` foi mantido propositalmente mínimo. As demonstrações das estruturas estão nos testes individuais e nas visualizações versionadas em `resultados/`.

## Testes funcionais

Para compilar todos os testes:

```bash
make tests
```

Para compilar e executar todos os testes funcionais:

```bash
make test
```

Cada estrutura também pode ser testada isoladamente:

```bash
make run-tests_trie
make run-tests_patricia
make run-tests_splay
make run-tests_treap
make run-tests_kdtree
```

Os testes verificam operações fundamentais, casos de remoção e manutenção das propriedades específicas de cada estrutura. Eles servem principalmente para validar a corretude das implementações durante o desenvolvimento.

## Benchmark

O benchmark mede inserção, busca e remoção em conjuntos de 500, 1000, 2500 e 5000 elementos. Cada cenário é repetido três vezes e o resultado armazenado corresponde à média das execuções.

Os padrões avaliados são:

- **Trie e Patricia:** palavras variadas e palavras com prefixo compartilhado;
- **Treap e Splay:** chaves ordenadas e aleatórias;
- **KD-Tree:** pontos aleatórios e pontos distribuídos em uma diagonal.

Para compilar o benchmark com otimização `-O2` e executá-lo:

```bash
make benchmark
```

O arquivo produzido é:

```text
resultados/benchmark.csv
```

O CSV contém a estrutura, operação, padrão de entrada, tamanho do conjunto, tempo total em microssegundos e tempo médio por operação.

## Geração dos gráficos

Depois de executar o benchmark:

```bash
make graphs
```

ou diretamente:

```bash
python3 scripts/graficos.py
```

As imagens são salvas em:

```text
resultados/graficos/
```

As comparações são separadas por domínio:

- Trie x Patricia;
- Treap x Splay;
- KD-Tree em diferentes distribuições espaciais.

## Geração dos diagramas estruturais

Para reproduzir as representações visuais das árvores:

```bash
make diagrams
```

ou:

```bash
python3 scripts/diagramas.py
```

Os arquivos são gerados em:

```text
resultados/diagramas/
```

Para gerar gráficos e diagramas de uma vez:

```bash
make visuals
```

Os diagramas mostram estados relevantes das estruturas, incluindo compartilhamento e compactação de prefixos, rotações/reorganizações e alternância dos eixos da KD-Tree.

## Limpeza e recompilação

Para remover os arquivos de compilação:

```bash
make clean
```

Para limpar e recompilar o executável principal:

```bash
make rebuild
```

## Observações de implementação

A Trie e a Patricia utilizam `std::unordered_map<char, Node*>` para representar ramificações por caracteres. A Patricia armazena segmentos inteiros de strings nos nós, compactando caminhos sem bifurcação.

A Treap mantém simultaneamente a propriedade de árvore binária de busca pela chave e de max-heap pela prioridade. Há uma sobrecarga de inserção com prioridade explícita para permitir testes determinísticos e outra que gera prioridades aleatórias com `std::mt19937`.

A Splay mantém ponteiros para o pai e utiliza rotações Zig, Zag, Zig-Zig, Zag-Zag, Zig-Zag e Zag-Zig para trazer o elemento acessado até a raiz.

A KD-Tree implementada é bidimensional. A dimensão de comparação alterna entre X e Y conforme a profundidade da árvore.

## Resultados e relatório

Os resultados experimentais utilizados no relatório estão versionados em `resultados/benchmark.csv`; os gráficos estão em `resultados/graficos/` e os estados estruturais em `resultados/diagramas/`.

O relatório é entregue separadamente em PDF/LaTeX e inclui o link deste repositório nas referências. Isso mantém o GitHub focado no código e nos artefatos necessários para reproduzir os experimentos e as visualizações.
