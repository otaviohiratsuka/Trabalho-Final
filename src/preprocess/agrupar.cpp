#include "preprocess/agrupar.hpp"
#include <sstream>  
#include <omp.h>
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

void escreverInputDat(const string& caminhoSaida, const unordered_map<int, vector<pair<int, float>>>& dadosAgrupados) {
    // Corrigindo o tipo do vetor de ponteiros
    vector<const pair<const int, vector<pair<int, float>>>*> ptrs;
    ptrs.reserve(dadosAgrupados.size());
    
    for (const auto& par : dadosAgrupados) {
        ptrs.push_back(&par);  // Agora o tipo está correto
    }

    #pragma omp parallel for
    for (size_t i = 0; i < ptrs.size(); i++) {
        std::stringstream buffer;  // Tipo completo (graças ao #include <sstream>)
        const auto& par = *ptrs[i];
        buffer << par.first;
        for (const auto& item : par.second) {
            buffer << " " << item.first << ":" << item.second;
        }
        buffer << "\n";

        // Escreve em arquivo temporário por thread
        ofstream saida(caminhoSaida + ".tmp_" + to_string(omp_get_thread_num()), ios::app);
        saida << buffer.str();
    }

    // Merge dos arquivos temporários (serial)
    ofstream saidaFinal(caminhoSaida);
    for (int i = 0; i < omp_get_max_threads(); i++) {
        ifstream tmp(caminhoSaida + ".tmp_" + to_string(i));
        saidaFinal << tmp.rdbuf();
        remove((caminhoSaida + ".tmp_" + to_string(i)).c_str());
    }
}