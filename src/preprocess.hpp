// Arquivo: include/preprocess.hpp
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Avaliacao {
    int userId;
    int movieId;
    float rating;

    bool operator==(const Avaliacao& other) const {
        return userId == other.userId && movieId == other.movieId;
    }
};

struct AvaliacaoHash {
    size_t operator()(const Avaliacao& a) const {
        return std::hash<int>()(a.userId) ^ std::hash<int>()(a.movieId << 1);
    }
};

std::vector<Avaliacao> lerAvaliacoes(const std::string& nomeArquivo, int maxLinhas,
                                     std::unordered_map<int, int>& contagemUsuarios,
                                     std::unordered_map<int, int>& contagemFilmes);

std::unordered_set<int> filtrarUsuariosOuFilmes(const std::unordered_map<int, int>& contagem, int minimo);

std::unordered_map<int, std::vector<std::pair<int, float>>> agruparAvaliacoesPorUsuario(
    const std::vector<Avaliacao>& avaliacoes,
    const std::unordered_set<int>& usuariosValidos,
    const std::unordered_set<int>& filmesValidos);

void escreverArquivoDeSaida(const std::unordered_map<int, std::vector<std::pair<int, float>>>& dados);

void preProcessar(const std::string& nomeArquivo, int maxLinhas);


// Otimizações:
// - Uso de sscanf para parsing rápido
// - reserve e rehash para evitar realocações frequentes
// - Compilar com -O3 para otimizações agressivas do compilador
