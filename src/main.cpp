
#include <iostream>
#include <vector>

#include "constants.hpp"
#include "functions.hpp"
#include "payload.hpp"


#include "newton_modificado.hpp"     // minha parte


int main()
{
    const double d0 = 0.5;   // aproximação inicial padrão ( temos que calcular fazer o calculo pela teoria dos sinas ainda em)
    int n;
    double eps;

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

    //  Execução cada a 
    for (int i = 0; i < n; ++i)
    {
        double a = A[i];

        Payload p = newton_modificado(&f, &df, a, d0, eps);

        std::cout << "\n=== a = " << a << " ===";
        print_test_table("newton_modificado()", p);

        if (p.vazio())
        {
            std::cout << "Resultado: metodo falhou (sem raiz calculada)\n";
            continue;
        }

        const Epoch& u = p.ultimo();
        bool convergiu = std::abs(u.fd) <= eps;

        std::cout << std::setprecision(8) << "Resultado: d = " << u.d << " | erro = " << u.erro
                  << " | iteracoes = " << u.k << " | "
                  << (convergiu ? "convergiu" : "NAO convergiu") << " | "
                  << (convergiu ? (u.d > MAX_CM ? "AMPLIFICA (d > 0,7 cm)" : "seguro (d <= 0,7 cm)") : "--")
                  << std::endl;
    }

    return 0;
}