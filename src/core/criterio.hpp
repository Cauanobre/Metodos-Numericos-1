#pragma once
#include <cmath>
#include "core/payload.hpp"
 

//abs(f(x1)) < E1   OU   abs(x1 - x0) < E2   (além do limite de iterações, tratado em cada metodo)
inline bool parou(double fd, double erro, double eps)
{
    return std::abs(fd) < eps || erro < eps;
}
 
// Diz se o metodo encontrou uma raiz: a última iteração satisfaz o mesmo
// critério de parada usado pelos métodos (|f(d)| < E ou |d_k - d_(k-1)| < E).
inline bool convergiu_payload(const Payload& p, double eps)
{
    if (p.vazio()) return false;
    return parou(p.ultimo().fd, p.ultimo().erro, eps);
}