"""Gráficos do item g), a partir dos CSVs do programa.

Uso (na raiz do projeto, depois de rodar o programa dentro de resultados/):
    python3 scripts/graficos.py

Lê:  resultados/iteracoes.csv e resultados/quadro_resposta.csv
Gera: graficos/*.png
"""
import csv
import math
import os

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker

RAIZ = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DIR_CSV = os.path.join(RAIZ, "resultados")
DIR_PNG = os.path.join(RAIZ, "graficos")

D0 = 0.5          # aproximação inicial usada no programa
MAX_CM = 0.7      # acima disso o movimento amplifica
A_PADRAO = 1.0

# Valor de a em que a raiz vale exatamente 0,7 cm: f(0,7) = 0  =>  a = 4·0,7² / e^0,7
A_CRITICO = 4 * MAX_CM ** 2 / math.exp(MAX_CM)   # ≈ 0,9733
# Testes perto de A_CRITICO vão só para o gráfico ampliado (08), para não apertar o 04 e o 05
JANELA_CRITICA = 0.05

# Uma cor fixa por método, igual em todos os gráficos
METODOS = {
    "newton_raphson()":    ("Newton-Raphson",    "#2a78d6"),
    "newton_modificado()": ("Newton modificado", "#eb6834"),
    "secante()":           ("Secante",           "#1baf7a"),
}
TEXTO = "#0b0b0b"
TEXTO_2 = "#52514e"
GRADE = "#e4e3df"
FUNDO = "#fcfcfb"
CURVA = "#008300"

plt.rcParams.update({
    "figure.facecolor": FUNDO, "axes.facecolor": FUNDO, "savefig.facecolor": FUNDO,
    "axes.edgecolor": TEXTO_2, "axes.labelcolor": TEXTO, "text.color": TEXTO,
    "xtick.color": TEXTO_2, "ytick.color": TEXTO_2, "font.size": 12,
    "axes.titlesize": 15, "axes.titleweight": "bold", "axes.spines.top": False,
    "axes.spines.right": False, "grid.color": GRADE, "legend.frameon": False,
})


def f(d, a):
    return a * math.exp(d) - 4 * d * d


def raizes(a, ini=-3.0, fim=6.0, passos=2000):
    """Todas as raízes de f em [ini, fim]: varre trocas de sinal e refina por bissecção."""
    achadas = []
    h = (fim - ini) / passos
    for i in range(passos):
        x0, x1 = ini + i * h, ini + (i + 1) * h
        if f(x0, a) * f(x1, a) <= 0:
            for _ in range(60):
                m = (x0 + x1) / 2
                if f(x0, a) * f(m, a) <= 0:
                    x1 = m
                else:
                    x0 = m
            r = (x0 + x1) / 2
            if not achadas or abs(r - achadas[-1]) > 1e-6:
                achadas.append(r)
    return achadas


def numero(s):
    try:
        return float(s)
    except ValueError:
        return None


def ler_csv(nome):
    with open(os.path.join(DIR_CSV, nome), newline="", encoding="utf-8") as arq:
        return list(csv.DictReader(arq, delimiter=";"))


def salvar(fig, nome):
    fig.savefig(os.path.join(DIR_PNG, nome), dpi=200, bbox_inches="tight")
    plt.close(fig)
    print("gerado: graficos/" + nome)


# ---------- 1) f(d) com as raízes marcadas ----------
# com_valores=False: versão para o slide do problema, sem revelar a resposta
def grafico_funcao(com_valores=True, nome="01_funcao_a1.png"):
    a = A_PADRAO
    xs = [-1.5 + i * 0.005 for i in range(1301)]
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.grid(True, linewidth=0.8)
    ax.axhline(0, color=TEXTO_2, linewidth=1)
    ax.axvline(0, color=TEXTO_2, linewidth=1)
    ax.plot(xs, [f(x, a) for x in xs], color=CURVA, linewidth=2.5)

    ax.axvline(MAX_CM, color="#e34948", linestyle="--", linewidth=1.5)
    ax.text(MAX_CM + 0.08, 14, "limite 0,7 cm", color=TEXTO_2)

    for r in raizes(a):
        ax.plot(r, 0, "o", color="#3b3b3b", markersize=9, zorder=3)
        rotulo = f"Raiz\n({r:.6f}; 0)" if com_valores else "raiz"
        ax.annotate(rotulo, (r, 0), xytext=(0, -48 if com_valores else -32),
                    textcoords="offset points", ha="center",
                    bbox=dict(boxstyle="round", fc="white", ec=GRADE))
    ax.plot(D0, f(D0, a), "o", color=METODOS["newton_raphson()"][1], markersize=9, zorder=3)
    ax.annotate("d₀ = 0,5", (D0, f(D0, a)), xytext=(-75, 28), textcoords="offset points",
                arrowprops=dict(arrowstyle="-", color=TEXTO_2))

    ax.set_xlim(-1.5, 5)
    ax.set_ylim(-12, 18)
    ax.set_xlabel("d (cm)")
    ax.set_ylabel("f(d)")
    ax.set_title("f(d) = a·eᵈ − 4d²  com a = 1")
    salvar(fig, nome)


# ---------- 2) f(d) para vários a: como a desloca as raízes ----------
def grafico_funcao_varios_a(valores_a):
    xs = [-1.5 + i * 0.005 for i in range(1101)]
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.grid(True, linewidth=0.8)
    ax.axhline(0, color=TEXTO_2, linewidth=1)
    ax.axvline(MAX_CM, color="#e34948", linestyle="--", linewidth=1.5)
    ax.text(MAX_CM + 0.05, -9.3, "limite 0,7 cm", color=TEXTO_2)

    # a é uma grandeza ordenada: um só tom, do claro (a pequeno) ao escuro (a grande)
    cmap = plt.get_cmap("Blues")
    for i, a in enumerate(valores_a):
        cor = cmap(0.3 + 0.7 * i / max(1, len(valores_a) - 1))
        ax.plot(xs, [f(x, a) for x in xs], color=cor, linewidth=2, label=f"a = {a:g}")

    ax.set_xlim(-1.5, 4)
    ax.set_ylim(-10, 15)
    ax.set_xlabel("d (cm)")
    ax.set_ylabel("f(d)")
    ax.set_title("Efeito de a na curva f(d)")
    ax.legend(loc="upper left", bbox_to_anchor=(1.01, 1))
    salvar(fig, "02_funcao_varios_a.png")


# ---------- 3) Convergência: erro por iteração (a = 1) ----------
def grafico_convergencia(iteracoes):
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.grid(True, linewidth=0.8)
    for m, (nome, cor) in METODOS.items():
        pts = [(int(r["k"]), float(r["erro"])) for r in iteracoes
               if r["metodo"] == m and float(r["a"]) == A_PADRAO and float(r["erro"]) > 0]
        if pts:
            ks, es = zip(*pts)
            ax.plot(ks, es, "-o", color=cor, linewidth=2, markersize=8, label=nome)
            ax.annotate(f"{len(ks)} it.", (ks[-1], es[-1]), xytext=(8, -4),
                        textcoords="offset points", color=TEXTO_2)
    ax.axhline(1e-4, color=TEXTO_2, linestyle=":", linewidth=1.5)
    ax.text(0.6, 1.3e-4, "ε = 10⁻⁴", color=TEXTO_2)
    ax.set_yscale("log")
    ax.set_xlabel("iteração k")
    ax.set_ylabel("erro |dₖ − dₖ₋₁|  (escala log)")
    ax.set_title("Convergência de cada método  (a = 1, d₀ = 0,5)")
    ax.legend()
    salvar(fig, "03_convergencia_a1.png")


# ---------- 4) Nº de iterações por a (X = não convergiu) ----------
def grafico_iteracoes_por_a(resposta, valores_a):
    fig, ax = plt.subplots(figsize=(11, 6))
    ax.grid(True, axis="y", linewidth=0.8)
    ax.set_axisbelow(True)
    ax.yaxis.set_major_locator(matplotlib.ticker.MaxNLocator(integer=True))
    largura = 0.26
    teto = 0
    for j, (m, (nome, cor)) in enumerate(METODOS.items()):
        for i, a in enumerate(valores_a):
            r = next(r for r in resposta if r["metodo"] == m and float(r["a"]) == a)
            x = i + (j - 1) * largura
            if r["situacao"] in ("nao convergiu", "falhou"):
                ax.text(x, 0.4, "✕", ha="center", color=cor, fontsize=15, fontweight="bold")
            else:
                n = int(r["iteracoes"])
                teto = max(teto, n)
                ax.bar(x, n, largura * 0.92, color=cor, edgecolor=FUNDO, linewidth=2,
                       label=nome if i == 0 else None)
                ax.text(x, n + 0.3, str(n), ha="center", fontsize=9, color=TEXTO_2)
    ax.plot([], [], "x", color=TEXTO_2, markersize=10, markeredgewidth=2, label="não convergiu")
    ax.set_xticks(range(len(valores_a)))
    ax.set_xticklabels([f"{a:g}" for a in valores_a])
    ax.set_ylim(0, teto + 3)
    ax.set_xlabel("a")
    ax.set_ylabel("nº de iterações")
    ax.set_title("Iterações até parar, por valor de a  (ε = 10⁻⁴)")
    ax.legend(loc="upper left", ncol=4)
    salvar(fig, "04_iteracoes_por_a.png")


# ---------- 5) Raiz encontrada por a ----------
def grafico_raiz_por_a(resposta, valores_a):
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.grid(True, linewidth=0.8)
    ax.axhline(MAX_CM, color="#e34948", linestyle="--", linewidth=1.5)
    ax.text(valores_a[-1], MAX_CM + 0.08, "limite 0,7 cm", color=TEXTO_2, ha="right")
    ax.axhline(0, color=TEXTO_2, linewidth=1)
    marcadores = {"newton_raphson()": "o", "newton_modificado()": "s", "secante()": "^"}
    # pequeno deslocamento horizontal por método para os pontos não se esconderem
    desloc = {"newton_raphson()": 0.92, "newton_modificado()": 1.0, "secante()": 1.08}
    for m, (nome, cor) in METODOS.items():
        pts = [(float(r["a"]), numero(r["d"])) for r in resposta
               if r["metodo"] == m and float(r["a"]) in valores_a
               and r["situacao"] not in ("nao convergiu", "falhou")]
        if pts:
            xs, ys = zip(*pts)
            ax.plot([x * desloc[m] for x in xs], ys, marcadores[m], color=cor, markersize=10, label=nome,
                    markeredgecolor=FUNDO, markeredgewidth=1.5)
    ax.axvline(A_CRITICO, color=TEXTO_2, linestyle=":", linewidth=1.5)
    ax.text(A_CRITICO * 0.97, -0.55, f"a crítico ≈ {A_CRITICO:.4f}", color=TEXTO_2,
            ha="right")
    ax.set_xscale("log")
    ax.set_xticks(valores_a)
    ax.set_xticklabels([f"{a:g}" for a in valores_a])
    ax.set_xlabel("a  (escala log)")
    ax.set_ylabel("d encontrado (cm)")
    ax.set_title("Raiz encontrada por cada método, por valor de a")
    ax.legend(loc="upper right")
    salvar(fig, "05_raiz_por_a.png")


# ---------- 8) Limite de segurança: a em que d cruza 0,7 cm ----------
def raiz_central(a):
    """Raiz entre 0,3 e 1,5 (a que os métodos encontram partindo de d₀ = 0,5 para a ≈ 1)."""
    x0, x1 = 0.3, 1.5
    for _ in range(60):
        m = (x0 + x1) / 2
        if f(x0, a) * f(m, a) <= 0:
            x1 = m
        else:
            x0 = m
    return (x0 + x1) / 2


def grafico_limite(resposta, valores_perto):
    ini, fim = A_CRITICO - 0.06, A_CRITICO + 0.06
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.grid(True, linewidth=0.8)

    # curva exata da raiz em função de a (referência para os pontos dos métodos)
    xs = [ini + i * (fim - ini) / 300 for i in range(301)]
    ax.plot(xs, [raiz_central(a) for a in xs], color=TEXTO_2, linewidth=1.2,
            label="raiz exata")

    ax.axhline(MAX_CM, color="#e34948", linestyle="--", linewidth=1.5)
    ax.axvline(A_CRITICO, color=TEXTO_2, linestyle=":", linewidth=1.5)
    ax.annotate(f"a crítico ≈ {A_CRITICO:.4f}", (A_CRITICO, MAX_CM), xytext=(12, -40),
                textcoords="offset points", color=TEXTO,
                arrowprops=dict(arrowstyle="-", color=TEXTO_2))
    ax.text(ini + 0.003, MAX_CM - 0.012, "seguro (d ≤ 0,7 cm)", color=TEXTO_2, va="top")
    ax.text(ini + 0.003, MAX_CM + 0.004, "amplifica (d > 0,7 cm)", color=TEXTO_2)

    marcadores = {"newton_raphson()": "o", "newton_modificado()": "s", "secante()": "^"}
    desloc = {"newton_raphson()": -0.0015, "newton_modificado()": 0.0, "secante()": 0.0015}
    for m, (nome, cor) in METODOS.items():
        pts = [(float(r["a"]), numero(r["d"])) for r in resposta
               if r["metodo"] == m and float(r["a"]) in valores_perto
               and r["situacao"] not in ("nao convergiu", "falhou")]
        if pts:
            xs_m, ys_m = zip(*pts)
            ax.plot([x + desloc[m] for x in xs_m], ys_m, marcadores[m], color=cor,
                    markersize=10, label=nome, markeredgecolor=FUNDO, markeredgewidth=1.5)

    ax.set_xlim(ini, fim)
    ax.set_xticks(valores_perto)
    ax.set_xticklabels([f"{a:g}" for a in valores_perto])
    ax.set_xlabel("a")
    ax.set_ylabel("d encontrado (cm)")
    ax.set_title("Limite de segurança: valor de a em que d chega a 0,7 cm")
    ax.legend(loc="upper left")
    salvar(fig, "08_limite_seguranca.png")


def main():
    os.makedirs(DIR_PNG, exist_ok=True)
    iteracoes = ler_csv("iteracoes.csv")
    resposta = ler_csv("quadro_resposta.csv")
    todos_a = sorted({float(r["a"]) for r in resposta})
    # a = 1 (padrão) fica nos dois grupos; o resto perto do a crítico vai só para o 08
    valores_perto = [a for a in todos_a if abs(a - A_CRITICO) <= JANELA_CRITICA]
    valores_a = [a for a in todos_a if a not in valores_perto or a == A_PADRAO]

    print(f"a critico (d = 0,7 cm): {A_CRITICO:.6f}")
    grafico_funcao()
    grafico_funcao(com_valores=False, nome="00_funcao_problema.png")
    grafico_funcao_varios_a(valores_a)
    if any(float(r["a"]) == A_PADRAO for r in iteracoes):
        grafico_convergencia(iteracoes)
    grafico_iteracoes_por_a(resposta, valores_a)
    grafico_raiz_por_a(resposta, valores_a)
    if len(valores_perto) > 1:
        grafico_limite(resposta, valores_perto)


if __name__ == "__main__":
    main()
