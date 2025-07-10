#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "preprocess.hpp"

// Lê até maxLinhas de um CSV de ratings, remove duplicatas e notas inválidas, e conta ocorrências
std::vector<Avaliacao> lerAvaliacoes(
    const std::string& caminhoCsv,
    std::unordered_map<int,int>& contagemUsuarios,
    std::unordered_map<int,int>& contagemFilmes,
    int maxLinhas = -1
);

// Função auxiliar para remover duplicatas de avaliações
void removerDuplicatas(std::vector<Avaliacao>& avaliacoes);