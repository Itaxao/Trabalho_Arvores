from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd


# ============================================================
# CAMINHOS
# ============================================================

RAIZ_PROJETO = Path(__file__).resolve().parent.parent

ARQUIVO_CSV = (
    RAIZ_PROJETO
    / "resultados"
    / "benchmark.csv"
)

PASTA_GRAFICOS = (
    RAIZ_PROJETO
    / "resultados"
    / "graficos"
)

PASTA_GRAFICOS.mkdir(
    parents=True,
    exist_ok=True
)


# ============================================================
# CONFIGURAÇÕES
# ============================================================

# Pode trocar por "tempo_medio_us" caso queira analisar
# o tempo médio de uma única operação.
COLUNA_TEMPO = "tempo_total_us"

OPERACOES = [
    "Insercao",
    "Busca",
    "Remocao"
]

NOMES_OPERACOES = {
    "Insercao": "Inserção",
    "Busca": "Busca",
    "Remocao": "Remoção"
}

NOMES_PADROES = {
    "Variado": "Variado",
    "PrefixoCompartilhado": "Prefixo compartilhado",
    "Aleatorio": "Aleatório",
    "Ordenado": "Ordenado",
    "Diagonal": "Diagonal"
}


# ============================================================
# FUNÇÃO DE GERAÇÃO DOS GRÁFICOS
# ============================================================

def gerar_grafico(
    dados,
    estruturas,
    operacao,
    titulo,
    nome_arquivo
):
    filtrado = dados[
        (dados["estrutura"].isin(estruturas))
        & (dados["operacao"] == operacao)
    ]

    plt.figure(figsize=(9, 5))

    grupos = filtrado.groupby(
        ["estrutura", "padrao"]
    )

    for (estrutura, padrao), grupo in grupos:
        grupo = grupo.sort_values("tamanho")

        nome_padrao = NOMES_PADROES.get(
            padrao,
            padrao
        )

        rotulo = (
            f"{estrutura} - {nome_padrao}"
        )

        plt.plot(
            grupo["tamanho"],
            grupo[COLUNA_TEMPO],
            marker="o",
            label=rotulo
        )

    plt.title(titulo)

    plt.xlabel(
        "Quantidade de elementos"
    )

    if COLUNA_TEMPO == "tempo_total_us":
        plt.ylabel(
            "Tempo total (µs)"
        )
    else:
        plt.ylabel(
            "Tempo médio por operação (µs)"
        )

    plt.grid(True)

    plt.legend()

    plt.tight_layout()

    caminho = (
        PASTA_GRAFICOS
        / nome_arquivo
    )

    plt.savefig(
        caminho,
        dpi=300
    )

    plt.close()

    print(
        f"Gráfico gerado: {caminho}"
    )


# ============================================================
# MAIN
# ============================================================

def main():
    if not ARQUIVO_CSV.exists():
        print(
            "Erro: benchmark.csv não encontrado."
        )

        print(
            f"Esperado em: {ARQUIVO_CSV}"
        )

        return

    dados = pd.read_csv(
        ARQUIVO_CSV
    )

    colunas_necessarias = {
        "estrutura",
        "operacao",
        "padrao",
        "tamanho",
        "tempo_total_us",
        "tempo_medio_us"
    }

    faltando = (
        colunas_necessarias
        - set(dados.columns)
    )

    if faltando:
        print(
            "Erro: existem colunas faltando no CSV:"
        )

        for coluna in faltando:
            print(
                f" - {coluna}"
            )

        return


    # ========================================================
    # TRIE X PATRICIA
    # ========================================================

    for operacao in OPERACOES:
        nome = NOMES_OPERACOES[operacao]

        gerar_grafico(
            dados,
            ["Trie", "Patricia"],
            operacao,
            (
                f"Trie x Patricia - {nome}"
            ),
            (
                "trie_patricia_"
                f"{operacao.lower()}.png"
            )
        )


    # ========================================================
    # TREAP X SPLAY
    # ========================================================

    for operacao in OPERACOES:
        nome = NOMES_OPERACOES[operacao]

        gerar_grafico(
            dados,
            ["Treap", "Splay"],
            operacao,
            (
                f"Treap x Splay - {nome}"
            ),
            (
                "treap_splay_"
                f"{operacao.lower()}.png"
            )
        )


    # ========================================================
    # KD-TREE
    # ========================================================

    for operacao in OPERACOES:
        nome = NOMES_OPERACOES[operacao]

        gerar_grafico(
            dados,
            ["KDTree"],
            operacao,
            (
                "KD-Tree - "
                f"{nome}"
            ),
            (
                "kdtree_"
                f"{operacao.lower()}.png"
            )
        )


    print(
        "\nTodos os gráficos foram gerados."
    )


if __name__ == "__main__":
    main()