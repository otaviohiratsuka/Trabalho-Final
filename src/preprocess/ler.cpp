#include "preprocess/ler.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <tuple>
#include <limits>

using namespace std;

// COM LIMITES DE LINHAS

vector<Avaliacao> lerAvaliacoes(
    const string &caminhoCsv,
    unordered_map<int, int> &contagemUsuarios,
    unordered_map<int, int> &contagemFilmes,
    int maxLinhas)
{
    ifstream arquivo(caminhoCsv);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao abrir arquivo: " << caminhoCsv << "\n";
        return {};
    }

    vector<Avaliacao> avaliacoes;
    if (maxLinhas > 0)
        avaliacoes.reserve(maxLinhas);

    // pré‑aloca os mapas antes de ler
    if (maxLinhas > 0) {
        contagemUsuarios.reserve(maxLinhas / 20);
        contagemFilmes  .reserve(maxLinhas / 20);
    }

    // pula o cabeçalho de forma eficiente
    arquivo.ignore(numeric_limits<streamsize>::max(), '\n');

    int lidos = 0;
    char buffer[256];    // buffer no stack, thread‑safe

    while ((maxLinhas < 0 || lidos < maxLinhas) &&
           arquivo.getline(buffer, sizeof(buffer)))
    {
        char *p = buffer;

        // parsing manual em vez de sscanf
        int usuarioId = strtol(p, &p, 10);
        if (*p != ',') continue;
        ++p;
        int filmeId = strtol(p, &p, 10);
        if (*p != ',') continue;
        ++p;
        float nota = strtof(p, nullptr);

        if (nota < 0.5f || nota > 5.0f)
            continue;

        // armazena com emplace_back
        avaliacoes.emplace_back(Avaliacao{usuarioId, filmeId, nota});
        ++lidos;
    }

    arquivo.close();

    // Sort + unique 
    sort(avaliacoes.begin(), avaliacoes.end(),
         [](const Avaliacao &a, const Avaliacao &b) {
             return tie(a.usuarioId, a.filmeId) < tie(b.usuarioId, b.filmeId);
         });

    avaliacoes.erase(
        unique(avaliacoes.begin(), avaliacoes.end(),
               [](const Avaliacao &a, const Avaliacao &b) {
                   return a.usuarioId == b.usuarioId
                       && a.filmeId == b.filmeId;
               }),
        avaliacoes.end());

    // atualiza contagens com dados únicos
    for (const auto &a : avaliacoes) {
        contagemUsuarios[a.usuarioId]++;
        contagemFilmes [a.filmeId]++;
    }

    return avaliacoes;
}
