from pathlib import Path
from graphviz import Digraph

RAIZ = Path(__file__).resolve().parent.parent
SAIDA = RAIZ / "resultados" / "diagramas"
SAIDA.mkdir(parents=True, exist_ok=True)


def novo(nome: str, titulo: str, shape: str = "ellipse") -> Digraph:
    g = Digraph(nome)
    g.attr(
        rankdir="TB",
        label=titulo,
        labelloc="t",
        fontsize="16",
        dpi="180",
        bgcolor="white",
        margin="0.03",
        nodesep="0.28",
        ranksep="0.35",
    )
    g.attr("node", fontname="DejaVu Sans", fontsize="9", shape=shape)
    g.attr("edge", fontname="DejaVu Sans", fontsize="8")
    return g


def salvar(g: Digraph, nome: str) -> None:
    caminho = SAIDA / nome
    g.render(str(caminho), format="png", cleanup=True)
    print(f"Diagrama gerado: {caminho}.png")


# TRIE -----------------------------------------------------------------------
def trie(apos_remocao: bool) -> Digraph:
    titulo = 'Trie - após remover "casa"' if apos_remocao else "Trie - estado inicial"
    g = novo("trie", titulo, "circle")
    g.node("r", "raiz", shape="box")
    labels = {
        "c": "c", "ca": "a", "cas": "s",
        "casa": "a" if apos_remocao else "a*",
        "casac": "c", "casaco": "o*", "car": "r", "carr": "r",
        "carro": "o*", "b": "b", "bo": "o", "bol": "l", "bola": "a*",
    }
    for n, l in labels.items():
        g.node(n, l)
    for a, b in [
        ("r", "c"), ("c", "ca"), ("ca", "cas"), ("cas", "casa"),
        ("casa", "casac"), ("casac", "casaco"), ("ca", "car"),
        ("car", "carr"), ("carr", "carro"), ("r", "b"), ("b", "bo"),
        ("bo", "bol"), ("bol", "bola"),
    ]:
        g.edge(a, b)
    return g

salvar(trie(False), "trie_estado1")
salvar(trie(True), "trie_estado2")


# PATRICIA -------------------------------------------------------------------
def patricia(estado: int) -> Digraph:
    titulos = {
        1: 'Patricia - após inserir "casaco" e "casa"',
        2: 'Patricia - após novas bifurcações',
        3: 'Patricia - após remoção e compactação',
    }
    g = novo("patricia", titulos[estado], "box")
    if estado == 1:
        nodes = [("r", "raiz"), ("casa", "casa*"), ("co", "co*")]
        edges = [("r", "casa"), ("casa", "co")]
    elif estado == 2:
        nodes = [
            ("r", "raiz"), ("ca", "ca"), ("rro", "rro*"),
            ("sa", "sa*"), ("co", "co*"), ("mento", "mento*"),
        ]
        edges = [("r", "ca"), ("ca", "rro"), ("ca", "sa"), ("sa", "co"), ("sa", "mento")]
    else:
        nodes = [("r", "raiz"), ("ca", "ca"), ("rro", "rro*"), ("saco", "saco*")]
        edges = [("r", "ca"), ("ca", "rro"), ("ca", "saco")]
    for n, l in nodes:
        g.node(n, l)
    for a, b in edges:
        g.edge(a, b)
    return g

for estado in (1, 2, 3):
    salvar(patricia(estado), f"patricia_estado{estado}")


# TREAP ----------------------------------------------------------------------
def treap(estado: int) -> Digraph:
    titulos = {
        1: "Treap - estado inicial (chave, prioridade)",
        2: "Treap - após remover 20",
        3: "Treap - após remover 50",
    }
    g = novo("treap", titulos[estado], "ellipse")
    if estado == 1:
        nodes = [("30", "30,80"), ("20", "20,40"), ("70", "70,70"), ("40", "40,60"), ("80", "80,20"), ("50", "50,50"), ("60", "60,30")]
        edges = [("30", "20", "E"), ("30", "70", "D"), ("70", "40", "E"), ("70", "80", "D"), ("40", "50", "D"), ("50", "60", "D")]
    elif estado == 2:
        nodes = [("30", "30,80"), ("70", "70,70"), ("40", "40,60"), ("80", "80,20"), ("50", "50,50"), ("60", "60,30")]
        edges = [("30", "70", "D"), ("70", "40", "E"), ("70", "80", "D"), ("40", "50", "D"), ("50", "60", "D")]
    else:
        nodes = [("30", "30,80"), ("70", "70,70"), ("40", "40,60"), ("80", "80,20"), ("60", "60,30")]
        edges = [("30", "70", "D"), ("70", "40", "E"), ("70", "80", "D"), ("40", "60", "D")]
    for n, l in nodes:
        g.node(n, l)
    for a, b, lado in edges:
        g.edge(a, b, label=lado)
    return g

for estado in (1, 2, 3):
    salvar(treap(estado), f"treap_estado{estado}")


# SPLAY ----------------------------------------------------------------------
def splay(estado: int) -> Digraph:
    titulos = {
        1: "Splay - antes de buscar 20",
        2: "Splay - após a primeira rotação do Zig-Zig",
        3: "Splay - após levar 20 à raiz",
    }
    g = novo("splay", titulos[estado], "circle")
    if estado == 1:
        vals = [80, 30, 20, 70, 50, 40, 60]
        edges = [(80, 30, "E"), (30, 20, "E"), (30, 70, "D"), (70, 50, "E"), (50, 40, "E"), (50, 60, "D")]
    elif estado == 2:
        vals = [30, 20, 80, 70, 50, 40, 60]
        edges = [(30, 20, "E"), (30, 80, "D"), (80, 70, "E"), (70, 50, "E"), (50, 40, "E"), (50, 60, "D")]
    else:
        vals = [20, 30, 80, 70, 50, 40, 60]
        edges = [(20, 30, "D"), (30, 80, "D"), (80, 70, "E"), (70, 50, "E"), (50, 40, "E"), (50, 60, "D")]
    for v in vals:
        g.node(str(v), str(v))
    for a, b, lado in edges:
        g.edge(str(a), str(b), label=lado)
    return g

for estado in (1, 2, 3):
    salvar(splay(estado), f"splay_estado{estado}")


# KD-TREE --------------------------------------------------------------------
def kdtree(apos_remocao: bool) -> Digraph:
    titulo = "KD-Tree - após remover (5,25)" if apos_remocao else "KD-Tree - estado inicial"
    g = novo("kdtree", titulo, "box")
    if not apos_remocao:
        nodes = [("30", "(30,40)\nX"), ("5", "(5,25)\nY"), ("10", "(10,12)\nX"), ("70", "(70,70)\nY"), ("50", "(50,30)\nX"), ("35", "(35,45)\nY")]
        edges = [("30", "5", "E"), ("5", "10", "E"), ("30", "70", "D"), ("70", "50", "E"), ("50", "35", "E")]
    else:
        nodes = [("30", "(30,40)\nX"), ("10", "(10,12)\nY"), ("70", "(70,70)\nY"), ("50", "(50,30)\nX"), ("35", "(35,45)\nY")]
        edges = [("30", "10", "E"), ("30", "70", "D"), ("70", "50", "E"), ("50", "35", "E")]
    for n, l in nodes:
        g.node(n, l)
    for a, b, lado in edges:
        g.edge(a, b, label=lado)
    return g

salvar(kdtree(False), "kdtree_estado1")
salvar(kdtree(True), "kdtree_estado2")
