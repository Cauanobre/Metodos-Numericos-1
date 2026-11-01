#pragma once
#include <cmath>
#include <iostream>
#include "core/constants.hpp"
#include "core/functions.hpp"
#include "core/criterio.hpp"

// Newton modificado: x1 = x0 - f(x0)/f'(x_inicial)
// A derivada é calculada uma vez, no ponto inicial, e permanece travada.
Payload newton_modificado(func f, func df, double a, double d0, double epsilon)
{
    Payload p;
    double x0 = d0;

    // d0 já é raiz aproximada: registra como iteração 0 e termina
    if (std::abs(f(x0, a)) < epsilon)
    {
        p.insert(Epoch(0, x0, f(x0, a), 0.0));
        return p;
    }

    // Derivada travada no ponto inicial 
    const double dfd0 = df(d0, a);

    //f'(x0) != 0 
    if (std::abs(dfd0) < TOL_DERIV)
    {
        p.motivo = "f'(x0) = 0 em x0 = " + std::to_string(d0) + ", metodo nao pode ser aplicado";
        return p;   // Payload vazio é falha
    }

    for (int k = 1; k <= MAX_ITER; ++k)
    {
        // Usamos f'(x inicial) e nao f'(x_k)
        double x1 = x0 - f(x0, a) / dfd0;

        if (!std::isfinite(x1))
        {
            p.motivo = "divergiu (valor nao finito)";
            return p;
        }

        double erro = std::abs(x1 - x0);
        p.insert(Epoch(k, x1, f(x1, a), erro));

        if (parou(f(x1, a), erro, epsilon)) return p;

        x0 = x1;
    }

    p.motivo = "nao convergiu em " + std::to_string(MAX_ITER) + " iteracoes";
    return p;
}