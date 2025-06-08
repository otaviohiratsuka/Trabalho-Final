#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

/// Representa a avaliação (nota) de um usuário para um filme
struct Avaliacao {
    int usuarioId;      // ID único do usuário
    int filmeId;        // ID único do filme
    float nota;         // Valor da avaliação (0.5 a 5.0)

    // Compara duas avaliações: são iguais se tiverem mesmo usuário e mesmo filme
    bool operator==(const Avaliacao& outro) const {
        return usuarioId == outro.usuarioId && filmeId == outro.filmeId;
    }
};

// Função de hash para usar Avaliacao em unordered_set
struct AvaliacaoHash {
    size_t operator()(const Avaliacao& a) const {
        return std::hash<int>()(a.usuarioId) ^ (std::hash<int>()(a.filmeId) << 1);
    }
};

// Lê até maxLinhas de um CSV de ratings, remove duplicatas e notas inválidas, e conta ocorrências
std::vector<Avaliacao> lerAvaliacoes(
    const std::string& caminhoCsv,
    int maxLinhas,
    std::unordered_map<int,int>& contagemUsuarios,
    std::unordered_map<int,int>& contagemFilmes
);

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

// Função principal de pré-processamento: lê CSV, filtra, agrupa e grava input.dat
void preprocessar(
    const std::string& caminhoCsvRatings,
    const std::string& caminhoOutputInputDat,
    int maxLinhas
);