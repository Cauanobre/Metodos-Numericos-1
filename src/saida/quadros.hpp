#pragma once
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "core/constants.hpp"
#include "core/payload.hpp"
#include "core/criterio.hpp"

// Itens e) e f): quadros construidos a partir dos Payloads dos metodos.
// Nao recalcula nada: so le o que cada metodo registrou (ultima iteracao de
// cada Payload). Uma Execucao = um metodo rodado para um valor de a.
struct Execucao {
  std::string metodo;
  double a;
  Payload p;
};

// ---------- auxiliares ----------

// Mesmo criterio de parada dos metodos (ver core/criterio.hpp)
inline bool q_convergiu(const Execucao &e, double eps) {
  return convergiu_payload(e.p, eps);
}

// Situacao do resultado frente ao limite do enunciado (d > 0,7 cm amplifica o
// movimento)
inline std::string q_situacao(const Execucao &e, double eps) {
  if (e.p.vazio())
    return "falhou";
  if (!q_convergiu(e, eps))
    return "nao convergiu";
  return e.p.ultimo().d > MAX_CM ? "AMPLIFICA (d > 0,7 cm)"
                                 : "seguro (d <= 0,7 cm)";
}

// Formatadores: devolvem string para o numero nao alterar o formato do
// std::cout
inline std::string q_fixo(double x, int casas) {
  std::ostringstream s;
  s << std::fixed << std::setprecision(casas) << x;
  return s.str();
}

inline std::string q_cient(double x, int casas) {
  std::ostringstream s;
  s << std::scientific << std::setprecision(casas) << x;
  return s.str();
}

inline std::string q_geral(double x) {
  std::ostringstream s;
  s << std::defaultfloat << std::setprecision(6) << x;
  return s.str();
}

// Campos de uma execucao ("--" quando o Payload esta vazio, ou seja, o metodo
// falhou antes de iterar)
inline std::string q_d(const Execucao &e) {
  if (e.p.vazio())
    return "--";
  double d = e.p.ultimo().d;
  // valores gigantes (metodo divergindo, ex.: Newton modificado) em notacao
  // cientifica para nao quebrar a coluna
  return std::abs(d) >= 1e6 ? q_cient(d, 3) : q_fixo(d, 8);
}
inline std::string q_fd(const Execucao &e) {
  return e.p.vazio() ? "--" : q_cient(e.p.ultimo().fd, 3);
}
inline std::string q_erro(const Execucao &e) {
  return e.p.vazio() ? "--" : q_cient(e.p.ultimo().erro, 3);
}
inline std::string q_iter(const Execucao &e) {
  return e.p.vazio() ? "--" : std::to_string(e.p.ultimo().k);
}

inline void q_linha(char c = '-', int n = 96) {
  std::cout << std::string(n, c) << "\n";
}

// ---------- item e) Quadro resposta ----------
// Um quadro por metodo: para cada a, o d calculado e o erro.
inline void quadro_resposta(const std::vector<Execucao> &execs,
                            const std::vector<std::string> &metodos,
                            double eps) {
  std::cout << "\n";
  q_linha('=');
  std::cout << "QUADRO RESPOSTA (item e)  |  precisao e = " << q_geral(eps)
            << "\n";
  q_linha('=');

  for (const std::string &m : metodos) {
    std::cout << "\n" << m << "\n";
    q_linha();
    std::cout << std::left << std::setw(10) << "a" << std::setw(16) << "d"
              << std::setw(14) << "f(d)" << std::setw(14) << "erro"
              << std::setw(8) << "iter" << "situacao\n";
    q_linha();

    for (const Execucao &e : execs) {
      if (e.metodo != m)
        continue;
      std::cout << std::left << std::setw(10) << q_geral(e.a) << std::setw(16)
                << q_d(e) << std::setw(14) << q_fd(e) << std::setw(14)
                << q_erro(e) << std::setw(8) << q_iter(e) << q_situacao(e, eps)
                << "\n";
    }
    q_linha();
  }
}

// ---------- item f) Quadro comparativo ----------
// Para cada a, os metodos lado a lado com todos os dados; no fim, resumo de
// iteracoes.
inline void quadro_comparativo(const std::vector<Execucao> &execs,
                               const std::vector<std::string> &metodos,
                               const std::vector<double> &valores_a,
                               double eps) {
  std::cout << "\n";
  q_linha('=');
  std::cout << "QUADRO COMPARATIVO (item f)  |  precisao e = " << q_geral(eps)
            << "\n";
  q_linha('=');

  std::cout << std::left << std::setw(10) << "a" << std::setw(24) << "metodo"
            << std::setw(16) << "d" << std::setw(14) << "f(d)" << std::setw(14)
            << "erro" << std::setw(7) << "iter" << "situacao\n";
  q_linha();

  for (double a : valores_a) {
    // Entre os metodos que convergiram para este a, guarda o de menor numero de
    // iteracoes
    const Execucao *melhor = nullptr;
    bool primeira = true;

    for (const Execucao &e : execs) {
      if (e.a != a)
        continue; // comparacao exata: o a vem do mesmo vetor usado nas
                  // execucoes
      std::cout << std::left << std::setw(10) << (primeira ? q_geral(a) : "")
                << std::setw(24) << e.metodo << std::setw(16) << q_d(e)
                << std::setw(14) << q_fd(e) << std::setw(14) << q_erro(e)
                << std::setw(7) << q_iter(e) << q_situacao(e, eps) << "\n";
      primeira = false;

      if (q_convergiu(e, eps) &&
          (!melhor || e.p.ultimo().k < melhor->p.ultimo().k))
        melhor = &e;
    }

    std::cout << std::string(10, ' ')
              << (melhor ? "-> menos iteracoes: " + melhor->metodo + " (" +
                               std::to_string(melhor->p.ultimo().k) + ")"
                         : std::string("-> nenhum metodo convergiu"))
              << "\n";
    q_linha();
  }

  // Resumo: numero de iteracoes por a x metodo (base para a analise do item g)
  std::cout
      << "\nResumo: numero de iteracoes (-- = falhou, X = nao convergiu)\n";
  q_linha();
  std::cout << std::left << std::setw(10) << "a";
  for (const std::string &m : metodos)
    std::cout << std::setw(24) << m;
  std::cout << "\n";
  q_linha();
  for (double a : valores_a) {
    std::cout << std::left << std::setw(10) << q_geral(a);
    for (const std::string &m : metodos)
      for (const Execucao &e : execs)
        if (e.a == a && e.metodo == m)
          std::cout << std::setw(24)
                    << (e.p.vazio() ? "--"
                                    : (q_convergiu(e, eps) ? q_iter(e) : "X"));
    std::cout << "\n";
  }
  q_linha();
}

// ---------- CSV (separador ';', abre direto no Excel/LibreOffice) ----------
inline void exportar_csv(const std::vector<Execucao> &execs,
                         const std::vector<std::string> &metodos,
                         const std::vector<double> &valores_a, double eps) {
  // quadro_resposta.csv: formato longo (uma linha por metodo e por a), reflete
  // o item e)
  std::ofstream r("quadro_resposta.csv");
  r << "metodo;a;d;f(d);erro;iteracoes;situacao\n";
  for (const Execucao &e : execs)
    r << e.metodo << ';' << q_geral(e.a) << ';' << q_d(e) << ';' << q_fd(e)
      << ';' << q_erro(e) << ';' << q_iter(e) << ';' << q_situacao(e, eps)
      << "\n";

  // quadro_comparativo.csv: formato largo (uma linha por a, metodos lado a
  // lado), reflete o item f)
  std::ofstream c("quadro_comparativo.csv");
  c << "a";
  for (const std::string &m : metodos)
    c << ';' << m << " d;" << m << " f(d);" << m << " erro;" << m << " iter;"
      << m << " situacao";
  c << "\n";
  for (double a : valores_a) {
    c << q_geral(a);
    for (const std::string &m : metodos)
      for (const Execucao &e : execs)
        if (e.a == a && e.metodo == m)
          c << ';' << q_d(e) << ';' << q_fd(e) << ';' << q_erro(e) << ';'
            << q_iter(e) << ';' << q_situacao(e, eps);
    c << "\n";
  }

  std::cout
      << "\nArquivos gerados: quadro_resposta.csv, quadro_comparativo.csv\n";
}