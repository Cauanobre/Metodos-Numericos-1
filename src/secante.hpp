#pragma once
#include <cmath>
#include <iostream>

// d_(k+1) = d_k - f(d_k) * (d_k - d_(k-1)) / (f(d_k) - f(d_(k-1)))
// Nao usa derivada, mas precisa de dois pontos iniciais (d0 e d1)
Payload secante(func f, double a, double d0, double d1, double epsilon)
{
    Payload p;
    int num_iterations = 0;

    double dant = d0;
    double d = d1;
    double fant = f(dant, a);
    double fd = f(d, a);

    do
    {
        // reta secante horizontal (f iguais) nao corta o eixo, entao nao tem como seguir
        double den = fd - fant;
        if (std::abs(den) < TOL_DERIV)
        {
            std::cerr << "secante(): f(d_k) = f(d_k-1) com a = " << a
                      << ", divisao por zero" << std::endl;
            break;
        }

        double dnovo = d - fd * (d - dant) / den;

        if (!std::isfinite(dnovo))
        {
            std::cerr << "secante(): divergiu" << std::endl;
            break;
        }

        // guarda o ponto antigo pro proximo passo
        dant = d;
        fant = fd;
        d = dnovo;
        fd = f(d, a);

        num_iterations++;
        p.insert(Epoch(num_iterations, d, fd, std::abs(d - dant)));

    } while (std::abs(fd) > epsilon && num_iterations < MAX_ITER);

    if (num_iterations >= MAX_ITER && std::abs(fd) > epsilon)
        std::cerr << "secante(): nao convergiu em " << MAX_ITER << " iteracoes" << std::endl;

    return p;
}
