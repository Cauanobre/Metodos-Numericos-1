// Itens e) e f) em janelas (wxWidgets): quadro resposta e quadro comparativo.
// Usa os mesmos métodos, o mesmo Payload e os mesmos formatadores do programa de
// terminal (main.cpp); só muda a forma de exibir.
//
// Este programa é dinâmico (wxWidgets depende do GTK); o executável de entrega
// continua sendo o bin/main estático.
#include <wx/wx.h>
#include <wx/grid.h>

#include <iostream>
#include <string>
#include <vector>

#include "core/constants.hpp"
#include "core/functions.hpp"
#include "core/payload.hpp"
#include "core/criterio.hpp"
#include "core/entrada.hpp"

#include "metodos/newton_raphson.hpp"
#include "metodos/newton_modificado.hpp"
#include "metodos/secante.hpp"
#include "saida/quadros.hpp"

static const std::vector<std::string> METODOS = {"newton_raphson()", "newton_modificado()", "secante()"};
static const std::vector<std::string> NOMES   = {"Newton-Raphson", "Newton modificado", "Secante"};

static wxString wx(const std::string& s) { return wxString::FromUTF8(s.c_str()); }

// Janela com rolagem vertical: um título no topo e uma sequência de seções (legenda + tabela)
class Janela : public wxFrame
{
public:
    explicit Janela(const std::string& titulo)
        : wxFrame(nullptr, wxID_ANY, wx(titulo))
    {
        painel = new wxScrolledWindow(this);
        painel->SetScrollRate(0, 20);
        painel->SetBackgroundColour(*wxWHITE);
        sizer = new wxBoxSizer(wxVERTICAL);
        painel->SetSizer(sizer);

        wxStaticText* t = new wxStaticText(painel, wxID_ANY, wx(titulo));
        t->SetFont(t->GetFont().Bold().Larger());
        sizer->Add(t, 0, wxALIGN_CENTER | wxALL, 12);
    }

    // Grade somente leitura que pertence a esta janela
    wxGrid* nova_grade(int linhas, int colunas)
    {
        wxGrid* g = new wxGrid(painel, wxID_ANY);
        g->CreateGrid(linhas, colunas);
        g->EnableEditing(false);
        g->SetDefaultCellAlignment(wxALIGN_LEFT, wxALIGN_CENTRE);
        g->SetLabelFont(g->GetLabelFont().Bold());
        return g;
    }

    void texto(const std::string& s)
    {
        wxStaticText* t = new wxStaticText(painel, wxID_ANY, wx(s));
        t->SetForegroundColour(wxColour(80, 80, 80));
        sizer->Add(t, 0, wxALIGN_CENTER | wxTOP, 14);
    }

    void secao(const std::string& legenda, wxGrid* g)
    {
        texto(legenda);
        g->AutoSize();
        sizer->Add(g, 0, wxALIGN_CENTER | wxALL, 8);
    }

    // Ajusta o tamanho ao conteúdo (limitado à tela) e mostra a janela na posição dada,
    // para as duas janelas abrirem lado a lado em vez de uma cobrir a outra
    void mostrar(const wxPoint& pos)
    {
        painel->FitInside();
        wxSize ideal = sizer->GetMinSize() + wxSize(40, 20);
        wxSize tela  = wxGetDisplaySize();
        SetClientSize(std::min(ideal.x, tela.x - 40), std::min(ideal.y, tela.y - 80));
        Move(pos);
        Show();
    }

private:
    wxScrolledWindow* painel;
    wxBoxSizer* sizer;
};

// ---------- item e) Quadro resposta: iterações de cada método, para cada a ----------
static void quadro_resposta_gui(const std::vector<Execucao>& execs, double eps)
{
    Janela* j = new Janela("QUADROS RESPOSTA");

    for (const Execucao& e : execs)
    {
        std::string nome = e.metodo;
        for (size_t i = 0; i < METODOS.size(); ++i)
            if (METODOS[i] == e.metodo) nome = NOMES[i];

        int linhas = e.p.vazio() ? 1 : (int)e.p.epochs.size();
        wxGrid* g = j->nova_grade(linhas, 4);
        g->HideRowLabels();
        g->SetColLabelValue(0, wx("Iteração k"));
        g->SetColLabelValue(1, wx("dₖ"));
        g->SetColLabelValue(2, wx("f(dₖ)"));
        g->SetColLabelValue(3, wx("|dₖ − dₖ₋₁|"));

        if (e.p.vazio())
            g->SetCellValue(0, 0, wx("-- " + e.p.motivo));
        for (size_t r = 0; r < e.p.epochs.size(); ++r)
        {
            const Epoch& ep = e.p.epochs[r];
            g->SetCellValue(r, 0, wx(std::to_string(ep.k)));
            g->SetCellValue(r, 1, wx(std::abs(ep.d) >= 1e6 ? q_cient(ep.d, 3) : q_fixo(ep.d, 8)));
            g->SetCellValue(r, 2, wx(q_cient(ep.fd, 3)));
            g->SetCellValue(r, 3, wx(q_cient(ep.erro, 3)));
        }

        j->secao(nome + "  -  a = " + q_geral(e.a) + ", d₀ = " + q_geral(D0_PADRAO)
                     + ", ε = " + q_geral(eps) + "  |  " + q_situacao(e, eps),
                 g);
    }
    j->mostrar(wxPoint(20, 20));
}

// ---------- item f) Quadro comparativo: métodos lado a lado, para cada a ----------
static void quadro_comparativo_gui(const std::vector<Execucao>& execs,
                                   const std::vector<double>& valores_a, double eps)
{
    Janela* j = new Janela("QUADRO COMPARATIVO");
    j->texto("Condições em comum: ε = " + q_geral(eps) + ", máximo de "
             + std::to_string(MAX_ITER) + " iterações");

    const std::vector<std::string> rotulos = {"Dados iniciais", "d", "f(d)", "Nº de iterações",
                                              "erro |Δd|", "situação"};
    const std::string d0 = "d₀ = " + q_geral(D0_PADRAO);
    const std::string d0d1 = d0 + "; d₁ = " + q_geral(D0_PADRAO + PASSO_SECANTE);

    for (double a : valores_a)
    {
        wxGrid* g = j->nova_grade((int)rotulos.size(), (int)METODOS.size());
        for (size_t r = 0; r < rotulos.size(); ++r) g->SetRowLabelValue(r, wx(rotulos[r]));
        g->SetRowLabelSize(wxGRID_AUTOSIZE);

        for (size_t c = 0; c < METODOS.size(); ++c)
        {
            g->SetColLabelValue(c, wx(NOMES[c]));
            for (const Execucao& e : execs)
            {
                if (e.a != a || e.metodo != METODOS[c]) continue;
                g->SetCellValue(0, c, wx(METODOS[c] == "secante()" ? d0d1 : d0));
                g->SetCellValue(1, c, wx(q_d(e)));
                g->SetCellValue(2, c, wx(q_fd(e)));
                g->SetCellValue(3, c, wx(q_iter(e)));
                g->SetCellValue(4, c, wx(q_erro(e)));
                g->SetCellValue(5, c, wx(q_situacao(e, eps)));
            }
        }
        j->secao("Parâmetros - a = " + q_geral(a), g);
    }
    j->mostrar(wxPoint(660, 20));
}

// ---------- Aplicação: lê a entrada no terminal (como o main.cpp) e abre as janelas ----------
class App : public wxApp
{
public:
    bool OnInit() override
    {
        int n;
        double eps;

        std::cout << "Numero de valores de a (n): ";
        if (!(std::cin >> n) || n <= 0) { std::cerr << "n invalido\n"; return false; }

        std::vector<double> A(n);
        for (int i = 0; i < n; ++i)
        {
            std::cout << "a[" << i + 1 << "]: ";
            if (!ler_double(A[i])) { std::cerr << "a invalido\n"; return false; }
        }

        std::cout << "Precisao e (ex: 1e-4): ";
        if (!ler_double(eps) || eps <= 0) { std::cerr << "e invalido\n"; return false; }

        const double d0 = D0_PADRAO;
        const double d1 = d0 + PASSO_SECANTE;
        std::vector<Execucao> execs;
        for (double a : A)
        {
            execs.push_back({METODOS[0], a, newton_raphson(&f, &df, a, d0, eps)});
            execs.push_back({METODOS[1], a, newton_modificado(&f, &df, a, d0, eps)});
            execs.push_back({METODOS[2], a, secante(&f, a, d0, d1, eps)});
        }

        quadro_resposta_gui(execs, eps);
        quadro_comparativo_gui(execs, A, eps);
        return true;
    }
};

wxIMPLEMENT_APP(App);
