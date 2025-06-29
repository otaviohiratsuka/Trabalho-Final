#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "preprocess/preprocess.hpp"

// Filtra IDs (usuário ou filme) que aparecem pelo menos minCount vezes
std::unordered_set<int> filtrarPorContagemMinima(
    const std::unordered_map<int,int>& contagens,
    int minCount
);

// Agrupa avaliações por usuário, considerando apenas usuários e filmes válidos
std::unordered_map<int, std::vector<std::pair<int,float>>>
agruparAvaliacoesPorUsuario(
    const std::vector<Avaliacao>& avaliacoes,
    const std::unordered_set<int>& usuariosValidos,
    const std::unordered_set<int>& filmesValidos
);


// Escreve as avaliações agrupadas no arquivo input.dat, no formato:
// usuarioId filmeId1:nota1 filmeId2:nota2 ...
void escreverInputDat(
    const std::string& caminhoSaida,
    const std::unordered_map<int, std::vector<std::pair<int,float>>>& dadosAgrupados
);