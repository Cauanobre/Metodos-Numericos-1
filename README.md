# Metodos-Numericos-1

1º Trabalho de Métodos Numéricos I, Raízes de Equações (Tema 2):
f(d) = a·eᵈ − 4d², pelos métodos de Newton-Raphson, Newton modificado e Secante.

## Equipe:

- Andrey Henrique de Abreu Carneiro 555740
- Cauã Nobre Fagundes Moura - 553387
- Francisco David Vaz de Sousa 554152
- Guilherme Monteiro Bessa 554209
- Ismael Gomes da Silva Lira - 497696
- Vinicius Oliveira Barbosa - 554747

---

## Estrutura

```
.
├── docs/                    enunciado do trabalho
├── src/
│   ├── main.cpp             entrada de dados e execução dos métodos
│   ├── core/                base compartilhada por todos os métodos
│   │   ├── constants.hpp    MAX_ITER, MAX_CM, TOL_DERIV, PASSO_SECANTE
│   │   ├── functions.hpp    f(d) e f'(d)
│   │   ├── payload.hpp      Epoch / Payload (registro das iterações)
│   │   ├── criterio.hpp     critério de parada e de convergência
│   │   └── entrada.hpp      leitura de números (aceita vírgula ou ponto)
│   ├── metodos/             um arquivo por método (itens a, b, c)
│   │   ├── newton_raphson.hpp
│   │   ├── newton_modificado.hpp
│   │   └── secante.hpp
│   ├── saida/
│   │   └── quadros.hpp      quadro resposta, comparativo e CSV (itens e, f)
│   └── gui/
│       └── quadros_gui.cpp  os mesmos quadros em janelas (wxWidgets)
├── scripts/                 gráficos do item g) e captura das janelas de quadros
├── resultados/              CSVs gerados pelo programa (não versionados)
├── graficos/                imagens dos gráficos (não versionadas)
├── slides/                  apresentação final
└── makefile
```

## Compilar e executar (Linux)

```sh
make            # build de desenvolvimento em bin/main
make run        # compila e executa dentro de resultados/
make csv        # roda o programa com vários valores de a e salva os CSVs em resultados/
make graficos   # make csv + gera os gráficos em graficos/ (precisa de python3 + matplotlib)
make static     # executável estático para entrega (g++ -static)
make gui        # programa com os quadros em janelas, em bin/quadros_gui (precisa de libwxgtk3.2-dev)
make quadros-img  # imagens das janelas de quadros em graficos/ (precisa de xvfb, xdotool e imagemagick)
make dist       # gera Equipe_MN1_Tema2.zip: slides + código + executável estático
make clean
```

Entrada do programa: `n` (número de valores de a), os `n` valores de `a` e a precisão `e`.
Teste padrão do enunciado: `n = 1`, `a = 1`, `e = 1e-4` (d₀ = 0,5 fixo no código).

```sh
printf "1\n1\n1e-4\n" | ./bin/main
```

A entrada aceita vírgula ou ponto como separador decimal (`0,0001` ou `1e-4`).

## Saídas

Além das tabelas no terminal, o programa grava na pasta onde é executado:

| Arquivo | Conteúdo |
|---|---|
| `quadro_resposta.csv` | resultado final de cada método para cada a (item e) |
| `quadro_comparativo.csv` | métodos lado a lado para cada a (item f) |
| `iteracoes.csv` | todas as iterações de cada método e a (base dos gráficos) |

## Quadros em janelas (wxWidgets)

`bin/quadros_gui` recebe a mesma entrada do `bin/main`, roda os mesmos métodos e mostra
os quadros resposta e comparativo em duas janelas com tabelas, no formato usado em trabalhos
anteriores da disciplina. É um programa separado e dinâmico, porque o wxWidgets depende do
GTK, que não pode ser ligado de forma estática. Por isso o executável de entrega continua
sendo o `bin/main` estático.

```sh
sudo apt install libwxgtk3.2-dev
make gui
./bin/quadros_gui
```

`make quadros-img` abre as janelas num display virtual e salva `quadro_resposta_*.png` e
`quadro_comparativo_*.png` em `graficos/` (teste padrão e a = 0,5, 1 e 2).

## Gráficos e análise do item g)

`make graficos` gera em `graficos/`:

| Imagem | O que mostra |
|---|---|
| `00_funcao_problema.png` | f(d) com a = 1 e as raízes marcadas sem valores (slide do problema) |
| `01_funcao_a1.png` | f(d) com a = 1 e as três raízes |
| `02_funcao_varios_a.png` | como a desloca a curva e as raízes |
| `03_convergencia_a1.png` | erro por iteração de cada método (a = 1) |
| `04_iteracoes_por_a.png` | iterações por valor de a (✕ = não convergiu) |
| `05_raiz_por_a.png` | raiz encontrada por cada método, por a |
| `08_limite_seguranca.png` | valor crítico de a em que d chega a 0,7 cm |

O valor crítico vem de f(0,7) = 0: a = 4·0,7² / e^0,7 ≈ 0,9733. Partindo de d₀ = 0,5,
valores de a abaixo dele dão d ≤ 0,7 cm (seguro) e acima dele o movimento amplifica.
Os valores de a testados ficam em `VALORES_A`, no makefile.

## Ideia geral: Epoch e Payload (`src/core/payload.hpp`)

Cada método numérico repete a mesma operação várias vezes e, a cada passo, produz alguns números. Em vez de cada método imprimir o que quiser, ou devolver só a raiz final, todos devolvem um registro completo do que aconteceu. Assim, a tabela de iterações, o quadro resposta e o comparativo saem dos mesmos dados.

**Epoch** é uma linha desse registro, ou seja, uma iteração. Guarda quatro valores: `k` (número da iteração), `d` (aproximação da raiz), `fd` (f(d)) e `erro` (|d_k − d_(k−1)|). O nome vem do repositório de referência; aqui significa só "iteração" (em aprendizado de máquina, epoch tem outro sentido). Se preferir, renomeie para Iteracao e mude o nome nos arquivos.

**Payload** é a tabela inteira: um `std::vector<Epoch>`. O método chama `p.insert(...)` a cada passo e devolve o Payload no fim.

```cpp
Payload p = newton_modificado(&f, &df, a, d0, eps);
p.epochs        // todas as iterações
p.ultimo()      // última iteração: tem a raiz, o erro final e a contagem k
p.vazio()       // true se o método falhou antes de iterar
```
