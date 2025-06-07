#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

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

void preProcessar(const std::string& nomeArquivo, int maxLinhas);
