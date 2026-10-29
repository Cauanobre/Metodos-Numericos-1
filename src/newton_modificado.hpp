#pragma once
#include <cmath>
#include <iostream>

// j(x) = x - f(x) / f'(x0)
Payload newton_modificado(func f, func df, double a, double d0, double epsilon)
{
    Payload p;
    int num_iterations = 0;
    double d = d0;

    // Derivada travada em x0 (calculada uma única vez, antes de iterar)
    const double dfd0 = df(d0, a);

    //  f'(x0) != 0 antes de iterar
    if (std::abs(dfd0) < TOL_DERIV)
    {
        std::cerr << "newton_modificado(): f'(x0) = 0 em x0 = " << d0
                  << " (a = " << a << "), metodo nao pode ser aplicado" << std::endl;
        return p;   // Payload vazio sinaliza falha
    }

    do
    {
        double dant = d;

        // Função de iteração: usa sempre f'(x0)  e nao f'(x_k)
        d = dant - f(dant, a) / dfd0;

        if (!std::isfinite(d))
        {
            std::cerr << "newton_modificado(): divergiu" << std::endl;
            break;
        }

        num_iterations++;
        p.insert(Epoch(num_iterations, d, f(d, a), std::abs(d - dant)));

    } while (std::abs(f(d, a)) > epsilon && num_iterations < MAX_ITER);

    if (num_iterations >= MAX_ITER && std::abs(f(d, a)) > epsilon)
        std::cerr << "newton_modificado(): nao convergiu em " << MAX_ITER << " iteracoes" << std::endl;

    return p;
}