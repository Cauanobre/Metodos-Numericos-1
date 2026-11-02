#pragma once
#include <cmath>
#include "core/payload.hpp"
 

//abs(f(x1)) < E1   OU   abs(x1 - x0) < E2   (além do limite de iterações, tratado em cada metodo)
inline bool parou(double fd, double erro, double eps)
{
    return std::abs(fd) < eps || erro < eps;
}
 
// Diz se o metodo encontrou uma raiz: |f(d)| < E na última iteração.
// Parar pelo passo (|d_k - d_(k-1)| < E) não basta: o método pode estagnar longe
// da raiz (ex.: secante com a = 2,5 parada em d = 0,6, onde f(d) = 3,1).
inline bool convergiu_payload(const Payload& p, double eps)
{
    if (p.vazio()) return false;
    return std::abs(p.ultimo().fd) < eps;
}

// Ao parar, registra o motivo quando a parada veio só do passo, sem |f(d)| < E
inline void registrar_parada(Payload& p, double fd, double eps)
{
    if (std::abs(fd) >= eps)
        p.motivo = "parou pelo passo (|dk - dk-1| < e), mas |f(d)| >= e";
}