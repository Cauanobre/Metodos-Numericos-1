# Metodos-Numericos-1

## Equipe:

- Andrey Henrique de Abreu Carneiro 555740
- Cauã Nobre Fagundes Moura - 553387
- Francisco David Vaz de Sousa 554152
- Guilherme Monteiro Bessa 554209
- Ismael Gomes da Silva Lira - 497696
- Vinicius Oliveira Barbosa - 554747

---


Ideia geral: Epoch e Payload ( Dentro do arquivo payload.hpp)

Cada método numérico repete a mesma operação várias vezes e, a cada passo, produz alguns números. Em vez de cada método imprimir o que quiser, ou devolver só a raiz final, todos devolvem um registro completo do que aconteceu. Assim, a tabela de iterações, o quadro resposta e o comparativo saem dos mesmos dados.

Epoch é uma linha desse registro, ou seja, uma iteração. Guarda quatro valores: k (número da iteração), d (aproximação da raiz), fd (f(d)) e erro (|d_k − d_(k−1)|). O nome vem do repositório de referência; aqui significa só “iteração” (em aprendizado de máquina, epoch tem outro sentido). Se preferir, renomeie para Iteracao e mude o nome nos arquivos.
Payload é a tabela inteira: um std::vector<Epoch>. O método chama p.insert(...) a cada passo e devolve o Payload no fim.
cpp
Payload p = newton_modificado(&f, &df, a, d0, eps);
p.epochs        // todas as iterações
p.ultimo()      // última iteração: tem a raiz, o erro final e a contagem k
p.vazio()       // true se o método falhou antes de iterar