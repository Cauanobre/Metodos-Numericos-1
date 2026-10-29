// Metodos Numericos I - Trabalho 1 - Tema 2
// Parte (a): Metodo de Newton-Raphson para f(d) = a*e^d - 4*d^2
//
// Compilar: g++ -O2 -static -o tema2_newton tema2_newton_raphson.cpp
// Executar: ./tema2_newton

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

const double D0 = 0.5;          // aproximacao inicial padrao
const double LIMITE = 0.7;      // acima disso o movimento amplifica (cm)
const int MAX_ITER = 100;

// f(d) = a*e^d - 4*d^2
double f(double d, double a) { return a * exp(d) - 4.0 * d * d; }

// f'(d) = a*e^d - 8*d
double df(double d, double a) { return a * exp(d) - 8.0 * d; }

struct Resultado {
    double d;        // raiz aproximada
    double erro;     // |d_k - d_(k-1)| na ultima iteracao
    double fd;       // f(d) na raiz aproximada
    int iter;        // numero de iteracoes
    bool convergiu;
};

Resultado newtonRaphson(double a, double d0, double eps) {
    Resultado r{d0, 0.0, f(d0, a), 0, false};
    double x = d0;

    if (fabs(f(x, a)) < eps) {  // d0 ja e raiz aproximada
        r.convergiu = true;
        return r;
    }

    for (int k = 1; k <= MAX_ITER; k++) {
        double deriv = df(x, a);
        if (fabs(deriv) < 1e-14) {  // derivada nula: metodo falha
            r.iter = k - 1;
            return r;
        }
        double xn = x - f(x, a) / deriv;
        r.erro = fabs(xn - x);
        r.iter = k;
        x = xn;
        r.d = x;
        r.fd = f(x, a);
        if (r.erro < eps) {
            r.convergiu = true;
            return r;
        }
    }
    return r;
}

int main() {
    int n;
    double eps;

    cout << "Numero de valores de a (n): ";
    cin >> n;
    vector<double> a(n);
    for (int i = 0; i < n; i++) {
        cout << "a[" << i + 1 << "]: ";
        cin >> a[i];
    }
    cout << "Precisao (e), ex: 0.0001: ";
    cin >> eps;

    cout << "\n=== Metodo de Newton-Raphson (d0 = " << D0 << ", e = " << eps << ") ===\n\n";
    cout << left << setw(10) << "a" << setw(16) << "d" << setw(16) << "erro |dk-dk-1|"
         << setw(16) << "f(d)" << setw(8) << "iter" << "situacao\n";
    cout << string(84, '-') << "\n";

    for (int i = 0; i < n; i++) {
        Resultado r = newtonRaphson(a[i], D0, eps);
        cout << defaultfloat << setprecision(6) << left << setw(10) << a[i];
        if (!r.convergiu) {
            cout << "nao convergiu (derivada nula ou limite de iteracoes)\n";
            continue;
        }
        cout << setw(16) << setprecision(8) << fixed << r.d
             << setw(16) << scientific << setprecision(3) << r.erro
             << setw(16) << r.fd
             << setw(8) << r.iter
             << (r.d > LIMITE ? "ALERTA: d > 0,7 cm (amplifica)" : "seguro (d <= 0,7 cm)")
             << "\n";
    }
    return 0;
}
