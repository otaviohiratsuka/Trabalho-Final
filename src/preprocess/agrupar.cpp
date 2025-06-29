#include "preprocess/agrupar.hpp"

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <utility> 
#include <algorithm> 
using namespace std;

unordered_set<int> filtrarPorContagemMinima(
    const unordered_map<int,int>& contagens,
    int minCount
) {
    unordered_set<int> validos;
    validos.reserve(contagens.size());
    for (const auto& par : contagens) {
        if (par.second >= minCount)
            validos.insert(par.first);
    }
    return validos;
}

unordered_map<int, vector<pair<int,float>>> agruparAvaliacoesPorUsuario(
    const vector<Avaliacao>& avaliacoes,
    const unordered_set<int>& usuariosValidos,
    const unordered_set<int>& filmesValidos
) {
    unordered_map<int, vector<pair<int,float>>> agrupado;
    agrupado.reserve(usuariosValidos.size());
    for (const auto& a : avaliacoes) {
        if (usuariosValidos.find(a.usuarioId) != usuariosValidos.end() &&
            filmesValidos.find(a.filmeId) != filmesValidos.end()) {
            agrupado[a.usuarioId].emplace_back(a.filmeId, a.nota);
        }
    }
    return agrupado;
}

void escreverInputDat(
    const string& caminhoSaida,
    const unordered_map<int, vector<pair<int,float>>>& dadosAgrupados
) {
    ofstream saida(caminhoSaida);
    if (!saida.is_open()) {
        cerr << "Erro ao criar arquivo: " << caminhoSaida << "\n";
        return;
    }
    for (const auto& par : dadosAgrupados) {
        saida << par.first;
        for (const auto& item : par.second)
            saida << " " << item.first << ":" << item.second;
        saida << "\n";
    }
    saida.close();
}
