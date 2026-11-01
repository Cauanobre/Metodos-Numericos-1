#pragma once
#include <cmath>
#include <iostream>
#include "core/constants.hpp"
#include "core/functions.hpp"
#include "core/criterio.hpp"

//  x1 = x0 - f(x0)/f'(x0)
Payload newton_raphson(func f, func df, double a, double d0, double epsilon)
{
    Payload p;
    double x0 = d0;

    
    if (std::abs(f(x0, a)) < epsilon)
    {
        p.insert(Epoch(0, x0, f(x0, a), 0.0));
        return p;
    }

    for (int k = 1; k <= MAX_ITER; ++k)
    {
        double dfx = df(x0, a);

        // Derivada nula
        if (std::abs(dfx) < TOL_DERIV)
        {
            p.motivo = "f'(x) = 0 em x = " + std::to_string(x0);
            return p;
        }

        double x1 = x0 - f(x0, a) / dfx;

        if (!std::isfinite(x1))
        {
            p.motivo = "divergiu (valor nao finito)";
            return p;
        }

        double erro = std::abs(x1 - x0);
        p.insert(Epoch(k, x1, f(x1, a), erro));          // escreva k, x1, f(x1)

        if (parou(f(x1, a), erro, epsilon)) return p;     // abs(f(x1)) < E1 ou abs(x1-x0) < E2

        x0 = x1;
    }

    p.motivo = "nao convergiu em " + std::to_string(MAX_ITER) + " iteracoes";
    return p;
}