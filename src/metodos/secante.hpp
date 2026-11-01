#pragma once
#include <cmath>
#include <iostream>
#include "core/constants.hpp"
#include "core/functions.hpp"
#include "core/criterio.hpp"


// d_(k+1) = d_k - f(d_k) * (d_k - d_(k-1)) / (f(d_k) - f(d_(k-1)))

Payload secante(func f, double a, double d0, double d1, double epsilon)
{
    Payload p;

    double dant = d0;
    double d = d1;
    double fant = f(dant, a);
    double fd = f(d, a);

    // se abs(f(x0)) < E1 então raiz <- x0; Fim  (registrado como iteração 0)
    if (std::abs(fant) < epsilon)
    {
        p.insert(Epoch(0, dant, fant, 0.0));
        return p;
    }

    // se abs(f(x1)) < E1 ou abs(x1 - x0) < E2 então raiz <- x1; Fim
    if (parou(fd, std::abs(d - dant), epsilon))
    {
        p.insert(Epoch(0, d, fd, std::abs(d - dant)));
        return p;
    }

    for (int k = 1; k <= MAX_ITER; ++k)
    {
        // reta secante horizontal (f iguais) não corta o eixo: não há como seguir
        double den = fd - fant;
        if (std::abs(den) < TOL_DERIV)
        {
            p.motivo = "f(d_k) = f(d_k-1), divisao por zero";
            return p;
        }

        double dnovo = d - fd * (d - dant) / den;

        if (!std::isfinite(dnovo))
        {
            p.motivo = "divergiu (valor nao finito)";
            return p;
        }

        // guarda o ponto antigo para o próximo passo
        dant = d;
        fant = fd;
        d = dnovo;
        fd = f(d, a);

        double erro = std::abs(d - dant);
        p.insert(Epoch(k, d, fd, erro));

        if (parou(fd, erro, epsilon)) return p;   // |f| < E  ou  |d_k - d_(k-1)| < E
    }

    p.motivo = "nao convergiu em " + std::to_string(MAX_ITER) + " iteracoes";
    return p;
}