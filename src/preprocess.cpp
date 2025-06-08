#include "preprocess.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

vector<Avaliacao> lerAvaliacoes(const string& nomeArquivo, int maxLinhas,
                                unordered_map<int, int>& contagemUsuarios,
                                unordered_map<int, int>& contagemFilmes) {
    ifstream arquivo(nomeArquivo);
    vector<Avaliacao> avaliacoes;
    unordered_set<Avaliacao, AvaliacaoHash> avaliacoesUnicas;

    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir arquivo: " << nomeArquivo << endl;
        return {};
    }

    // Otimizações: reserva, carga e reorganização
    avaliacoes.reserve(maxLinhas);
    avaliacoesUnicas.reserve(maxLinhas);
    avaliacoesUnicas.max_load_factor(1.0);
    
    contagemUsuarios.reserve(maxLinhas / 20);
    contagemUsuarios.rehash(maxLinhas / 20);
    contagemUsuarios.max_load_factor(1.0);

    contagemFilmes.reserve(maxLinhas / 50);
    contagemFilmes.rehash(maxLinhas / 50);
    contagemFilmes.max_load_factor(1.0);

    char buffer[128];
    arquivo.getline(buffer, sizeof(buffer)); // Pula o cabeçalho

    int linhasLidas = 0;
    while (linhasLidas < maxLinhas && arquivo.getline(buffer, sizeof(buffer))) {
        int userId, movieId;
        float rating;

        if (sscanf(buffer, "%d,%d,%f", &userId, &movieId, &rating) != 3) continue;

        Avaliacao a{userId, movieId, rating};
        if (avaliacoesUnicas.insert(a).second) {
            avaliacoes.push_back(a);
            contagemUsuarios[userId]++;
            contagemFilmes[movieId]++;
        }

        linhasLidas++;
    }

    arquivo.close();
    return avaliacoes;
}

unordered_set<int> filtrarUsuariosOuFilmes(const unordered_map<int, int>& contagem, int minimo) {
    unordered_set<int> validos;
    validos.reserve(contagem.size());

    for (auto& par : contagem) {
        if (par.second >= minimo)
            validos.insert(par.first);
    }
    return validos;
}

unordered_map<int, vector<pair<int, float>>> agruparAvaliacoesPorUsuario(
    const vector<Avaliacao>& avaliacoes,
    const unordered_set<int>& usuariosValidos,
    const unordered_set<int>& filmesValidos) {

    unordered_map<int, vector<pair<int, float>>> agrupado;
    for (const auto& a : avaliacoes) {
        if (usuariosValidos.find(a.userId) != usuariosValidos.end() &&
            filmesValidos.find(a.movieId) != filmesValidos.end()) {
            agrupado[a.userId].emplace_back(a.movieId, a.rating);
        }
    }
    return agrupado;
}

void escreverArquivoDeSaida(const unordered_map<int, vector<pair<int, float>>>& dados) {
    ofstream out("datasets/input.dat");
    if (!out.is_open()) {
        cerr << "Erro ao criar datasets/input.dat" << endl;
        return;
    }

    for (const auto& par : dados) {
        out << par.first;
        for (const auto& itemNota : par.second) {
            out << " " << itemNota.first << ":" << itemNota.second;
        }
        out << "\n";
    }

    out.close();
}

void preProcessar(const string& nomeArquivo, int maxLinhas) {
    unordered_map<int, int> contagemUsuarios, contagemFilmes;

    auto avaliacoes = lerAvaliacoes(nomeArquivo, maxLinhas, contagemUsuarios, contagemFilmes);

    auto usuariosValidos = filtrarUsuariosOuFilmes(contagemUsuarios, 50);
    auto filmesValidos = filtrarUsuariosOuFilmes(contagemFilmes, 50);

    auto agrupado = agruparAvaliacoesPorUsuario(avaliacoes, usuariosValidos, filmesValidos);

    escreverArquivoDeSaida(agrupado);

    cout << "Arquivo datasets/input.dat gerado com sucesso com limite de " << maxLinhas << " linhas lidas!\n";
}
