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
│   │   └── criterio.hpp     critério de parada e de convergência
│   ├── metodos/             um arquivo por método (itens a, b, c)
│   │   ├── newton_raphson.hpp
│   │   ├── newton_modificado.hpp
│   │   └── secante.hpp
│   └── saida/
│       └── quadros.hpp      quadro resposta, comparativo e CSV (itens e, f)
├── scripts/                 scripts auxiliares (gráficos do item g)
├── resultados/              CSVs gerados pelo programa (não versionados)
├── graficos/                imagens dos gráficos (não versionadas)
├── slides/                  apresentação final
└── makefile
```

## Compilar e executar (Linux)

```sh
make            # build de desenvolvimento em bin/main
make run        # compila e executa dentro de resultados/
make static     # executável estático para entrega (g++ -static)
make dist       # gera Equipe_MN1_Tema2.zip: slides + código + executável estático
make clean
```

Entrada do programa: `n` (número de valores de a), os `n` valores de `a` e a precisão `e`.
Teste padrão do enunciado: `n = 1`, `a = 1`, `e = 1e-4` (d₀ = 0,5 fixo no código).

```sh
printf "1\n1\n1e-4\n" | ./bin/main
```

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
