#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

#include "core/constants.hpp"
#include "core/functions.hpp"
#include "core/payload.hpp"
#include "core/criterio.hpp"

#include "metodos/newton_raphson.hpp"
#include "metodos/newton_modificado.hpp"
#include "metodos/secante.hpp"
#include "saida/quadros.hpp"

// Assinatura comum a Newton-Raphson e Newton modificado
typedef Payload (*Metodo)(func, func, double, double, double);

// Imprime a tabela de iterações e o resumo de um Payload (usado por todos os métodos)
void imprimir_resultado(const std::string& nome, const Payload& p, double eps)
{
    print_test_table(nome, p);

    if (p.vazio())
    {
        std::cout << "Resultado: metodo falhou (sem raiz calculada)\n";
        return;
    }

    const Epoch& u = p.ultimo();
    bool convergiu = convergiu_payload(p, eps);

    std::cout << std::setprecision(8) << "Resultado: d = " << u.d << " | erro = " << u.erro
              << " | iteracoes = " << u.k << " | "
              << (convergiu ? "convergiu" : "NAO convergiu") << " | "
              << (convergiu ? (u.d > MAX_CM ? "AMPLIFICA (d > 0,7 cm)" : "seguro (d <= 0,7 cm)") : "--")
              << std::endl;
}

// Métodos que recebem (f, df, a, d0, eps): Newton-Raphson e Newton modificado
Payload executar(const std::string& nome, Metodo metodo, double a, double d0, double eps)
{
    Payload p = metodo(&f, &df, a, d0, eps);
    imprimir_resultado(nome, p, eps);
    return p;
}

int main()
{
    const double d0 = 0.5;                    // aproximação inicial padrão
    const double d1 = d0 + PASSO_SECANTE;     // segundo ponto da secante (usa 2 pontos iniciais)
    int n;
    double eps;

    // ---- Entrada: n, a (para cada n) e e ----
    std::cout << "Numero de valores de a (n): ";
    if (!(std::cin >> n) || n <= 0) { std::cerr << "n invalido\n"; return 1; }

    std::vector<double> A(n);
    for (int i = 0; i < n; ++i)
    {
        std::cout << "a[" << i + 1 << "]: ";
        if (!(std::cin >> A[i])) { std::cerr << "a invalido\n"; return 1; }
    }

    std::cout << "Precisao e (ex: 1e-4): ";
    if (!(std::cin >> eps) || eps <= 0) { std::cerr << "e invalido\n"; return 1; }

    // Guarda o Payload de cada método para cada a (usado nos quadros)
    std::vector<Execucao> execs;
    const std::vector<std::string> metodos = {"newton_raphson()", "newton_modificado()", "secante()"};

    // ---- Execução dos métodos para cada a ----
    for (int i = 0; i < n; ++i)
    {
        double a = A[i];
        std::cout << "\n=== a = " << a << " ===";

        Payload pnr = executar("newton_raphson()",    newton_raphson,    a, d0, eps);
        Payload pnm = executar("newton_modificado()", newton_modificado, a, d0, eps);

        // Secante tem assinatura própria (precisa de d0 e d1), por isso é chamada à parte
        Payload psc = secante(&f, a, d0, d1, eps);
        imprimir_resultado("secante()", psc, eps);

        // Registra os resultados deste a para os quadros
        execs.push_back({metodos[0], a, pnr});
        execs.push_back({metodos[1], a, pnm});
        execs.push_back({metodos[2], a, psc});
    }

    // ---- Itens e) e f): quadros e exportação CSV ----
    quadro_resposta(execs, metodos, eps);
    quadro_comparativo(execs, metodos, A, eps);
    exportar_csv(execs, metodos, A, eps);

    return 0;
}
