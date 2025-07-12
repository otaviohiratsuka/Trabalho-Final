#include "jaccard/LeitorPerfis.hpp"
#include "jaccard/GerenciadorProcessos.hpp"
#include "jaccard/ProcessadorRecomendacoes.hpp"
#include "jaccard/CalculadorSimilaridade.hpp"
#include <fstream>
#include <vector>
#include <utility>

void recomendarJaccard(
    const std::string& caminhoInput,
    const std::string& caminhoExplore,
    const std::string& caminhoOutput
) {
    auto perfis = LeitorPerfis::lerArquivo(caminhoInput);
    auto exploradores = LeitorPerfis::lerArquivo(caminhoExplore);

    std::vector<std::pair<int, std::vector<int>>> exploradoresVec;
    exploradoresVec.reserve(exploradores.size());
    for (const auto& [uid, filmes] : exploradores) {
        exploradoresVec.emplace_back(uid, filmes);
    }

    std::vector<std::pair<int, std::vector<int>>> perfisVec;
    perfisVec.reserve(perfis.size());
    for (const auto& [uid, filmes] : perfis) {
        perfisVec.emplace_back(uid, filmes);
    }

    GerenciadorProcessos::executarProcessosParalelos(exploradoresVec, perfisVec, caminhoOutput);
}