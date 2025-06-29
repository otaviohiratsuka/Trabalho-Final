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



// Função principal de pré-processamento: lê CSV, filtra, agrupa e grava input.dat
void preprocessar(
    const std::string& caminhoCsvRatings,
    const std::string& caminhoOutputInputDat,
    int maxLinhas = -1
);