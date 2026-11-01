#pragma once
#include <cmath>
 
// Tipo usado por todos os métodos - função de (d, a)
typedef double (*func)(double, double);
 
// f(d) 
double f(double d, double a)
{
    return a * std::exp(d) - 4.0 * d * d;
}
 
// f'(d)
double df(double d, double a)
{
    return a * std::exp(d) - 8.0 * d;
}
 