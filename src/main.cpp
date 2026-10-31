
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

#include "constants.hpp"
#include "functions.hpp"
#include "payload.hpp"
#include "criterio.hpp"

#include "newton_raphson.hpp"       
#include "newton_modificado.hpp"     


// Assinatura comum a todos os métodos
typedef Payload (*Metodo)(func, func, double, double, double);

// Roda um método, imprime a tabela de iterações e o resumo
void executar(const std::string& nome, Metodo metodo, double a, double d0, double eps)
{
    Payload p = metodo(&f, &df, a, d0, eps);

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

int main()
{
    const double d0 = 0.5;   // aproximação inicial padrão
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

    // ---- Execução dos métodos para cada a ----
    for (int i = 0; i < n; ++i)
    {
        double a = A[i];
        std::cout << "\n=== a = " << a << " ===";

        executar("newton_raphson()",     newton_raphson,     a, d0, eps);
        executar("newton_modificado()",  newton_modificado,  a, d0, eps);
        // executar("secante()", secante, a, d0, eps);   
    }

    return 0;
}