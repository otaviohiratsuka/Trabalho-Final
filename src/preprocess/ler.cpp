#include "preprocess/ler.hpp"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <execution>
#include <iostream>

using namespace std;

// Parsing rápido de inteiros (substitui strtol)
inline int fast_atoi(const char*& ptr) {
    int val = 0;
    while (*ptr >= '0' && *ptr <= '9') {
        val = val * 10 + (*ptr++ - '0');
    }
    return val;
}

// Parsing rápido de floats (substitui strtof)
inline float fast_atof(const char*& ptr) {
    float val = 0.0f;
    float frac = 1.0f;
    while (*ptr >= '0' && *ptr <= '9') {
        val = val * 10 + (*ptr++ - '0');
    }
    if (*ptr == '.') {
        ptr++;
        while (*ptr >= '0' && *ptr <= '9') {
            frac *= 0.1f;
            val += (*ptr++ - '0') * frac;
        }
    }
    return val;
}

vector<Avaliacao> lerAvaliacoes(
    const string &caminhoCsv,
    unordered_map<int, int> &contagemUsuarios,
    unordered_map<int, int> &contagemFilmes,
    int maxLinhas
) {
    // Abre o arquivo com mmap
    int fd = open(caminhoCsv.c_str(), O_RDONLY);
    if (fd == -1) {
        cerr << "Erro ao abrir arquivo: " << caminhoCsv << "\n";
        return {};
    }
    size_t size = lseek(fd, 0, SEEK_END);
    const char* data = (const char*)mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    vector<Avaliacao> avaliacoes;
    if (maxLinhas > 0) avaliacoes.reserve(maxLinhas);

    // Pré-aloca mapas (estimativa)
    if (maxLinhas > 0) {
        contagemUsuarios.reserve(maxLinhas / 20);
        contagemFilmes.reserve(maxLinhas / 20);
    }

    const char* ptr = data;
    const char* end = data + size;

    // Pula cabeçalho
    while (ptr < end && *ptr != '\n') ptr++;
    if (ptr < end) ptr++;

    // Processamento linha a linha
    int lidos = 0;
    while (ptr < end && (maxLinhas < 0 || lidos < maxLinhas)) {
        const char* linha_start = ptr;
        while (ptr < end && *ptr != '\n') ptr++;
        if (ptr >= end) break;

        // Parsing manual (rápido)
        const char* p = linha_start;
        int usuarioId = fast_atoi(p);
        if (*p != ',') { ptr++; continue; }
        p++;
        int filmeId = fast_atoi(p);
        if (*p != ',') { ptr++; continue; }
        p++;
        float nota = fast_atof(p);

        // Valida nota
        if (nota >= 0.5f && nota <= 5.0f) {
            avaliacoes.emplace_back(Avaliacao{usuarioId, filmeId, nota});
            lidos++;
        }
        ptr++; // Pula '\n'
    }

    // remover duplicatas
    removerDuplicatas(avaliacoes);


    // Contagem de usuários e filmes
    for (const auto& a : avaliacoes) {
        contagemUsuarios[a.usuarioId]++;
        contagemFilmes[a.filmeId]++;
    }

    munmap((void*)data, size);
    return avaliacoes;
}

// funcao auxiliar remover duplicatas
void removerDuplicatas(std::vector<Avaliacao>& avaliacoes) {
    // Ordena as avaliações por (usuarioId, filmeId)
    std::sort(std::execution::par_unseq, avaliacoes.begin(), avaliacoes.end(),
              [](const Avaliacao& a, const Avaliacao& b) {
                  return std::tie(a.usuarioId, a.filmeId) < std::tie(b.usuarioId, b.filmeId);
              });

    // Remove elementos consecutivos iguais
    auto novoFim = std::unique(std::execution::par_unseq, avaliacoes.begin(), avaliacoes.end(),
              [](const Avaliacao& a, const Avaliacao& b) {
                  return a.usuarioId == b.usuarioId && a.filmeId == b.filmeId;
              });

    // Apaga duplicatas
    avaliacoes.erase(novoFim, avaliacoes.end());
}