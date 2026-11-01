# Scripts

Scripts auxiliares (fora do executável C++), por exemplo a geração dos gráficos
do item g) em Python/Matplotlib.

- Entrada: CSVs gerados pelo programa em `resultados/` (`quadro_resposta.csv`,
  `quadro_comparativo.csv` e `iteracoes.csv`, separador `;`).
  O `iteracoes.csv` tem uma linha por iteração (`metodo;a;k;d;f(d);erro`),
  útil para gráficos de erro × iteração.
- Saída: imagens em `graficos/`.

Gerar tudo de uma vez (na raiz do projeto): `make graficos`.
Precisa de `python3` com `matplotlib` (`pip install matplotlib`).
