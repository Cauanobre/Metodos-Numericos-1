#pragma once
#include <cmath>
#include "payload.hpp"
 

//   abs(f(x1)) < ε1   OU   abs(x1 - x0) < ε2   (além do limite de iterações, tratado em cada metodo)
inline bool parou(double fd, double erro, double eps)
{
    return std::abs(fd) < eps || erro < eps;
}
 
// Diz se o metodo terminou por criterio de precisão (e não por falha ou limite de iterações)
inline bool convergiu_payload(const Payload& p, double eps)
{
    if (p.vazio()) return false;
    const Epoch& u = p.ultimo();
    return parou(u.fd, u.erro, eps);
}