#pragma once
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

// Dados de uma iteração
struct Epoch
{
    int    k;      // número da iteração (começa em 1)
    double d;      // aproximação da raiz
    double fd;     // f(d)
    double erro;   // |d_k - d_(k-1)|

    Epoch(int k, double d, double fd, double erro) : k(k), d(d), fd(fd), erro(erro) {}
};

// Dados de todas as iterações de um método
struct Payload
{
    std::vector<Epoch> epochs;
    std::string motivo;   // por que o método parou sem convergir (vazio se convergiu)

    void insert(const Epoch& e) { epochs.push_back(e); }
    bool vazio() const { return epochs.empty(); }
    const Epoch& ultimo() const { return epochs.back(); }
};

// Imprime a tabela de iterações de um método
void print_test_table(const std::string& nome, const Payload& p)
{
    const std::string linha(52, '-');
    std::ios formato_original(nullptr);
    formato_original.copyfmt(std::cout);   // guarda o formato para restaurar no fim

    std::cout << "\n" << nome << "\n" << linha << "\n";
    std::cout << std::left << std::setw(5) << "k" << std::setw(15) << "d"
              << std::setw(15) << "f(d)" << std::setw(15) << "erro" << "\n" << linha << "\n";

    for (const Epoch& e : p.epochs)
    {
        std::cout << std::left << std::setw(5) << e.k;
        // valores gigantes (método divergindo) em notação científica para não quebrar a coluna
        if (std::abs(e.d) >= 1e6)
            std::cout << std::scientific << std::setprecision(3) << std::setw(15) << e.d;
        else
            std::cout << std::fixed << std::setprecision(8) << std::setw(15) << e.d;
        std::cout << std::scientific << std::setprecision(3) << std::setw(15) << e.fd
                  << std::setw(15) << e.erro << "\n";
    }

    std::cout << linha << std::endl;
    std::cout.copyfmt(formato_original);
}