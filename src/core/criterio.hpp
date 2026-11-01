#pragma once
#include <cmath>
#include "core/payload.hpp"
 

//abs(f(x1)) < E1   OU   abs(x1 - x0) < E2   (além do limite de iterações, tratado em cada metodo)
inline bool parou(double fd, double erro, double eps)
{
    return std::abs(fd) < eps || erro < eps;
}
 
// Diz se o metodo encontrou uma raiz: |f(d)| < E na última iteração.
inline bool convergiu_payload(const Payload& p, double eps)
{
    if (p.vazio()) return false;
    return std::abs(p.ultimo().fd) < eps;
}