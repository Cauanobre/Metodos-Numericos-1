#pragma once
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

// Lê um número real aceitando vírgula ou ponto como separador decimal (ex: 0,0001 ou 1e-4)
// Retorna false se o texto digitado não for um número completo
inline bool ler_double(double& x)
{
    std::string s;
    if (!(std::cin >> s)) return false;
    std::replace(s.begin(), s.end(), ',', '.');
    char* fim = nullptr;
    x = std::strtod(s.c_str(), &fim);
    return fim != s.c_str() && *fim == '\0';
}
