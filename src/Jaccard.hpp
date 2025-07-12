#ifndef JACCARD_HPP
#define JACCARD_HPP

#include <vector>
#include <string>

// Declaração da função jaccard atualizada
double jaccard(const std::vector<int>& a, const std::vector<int>& b);

void recomendarJaccard(
    const std::string& caminhoInput,
    const std::string& caminhoExplore,
    const std::string& caminhoOutput
);

#endif