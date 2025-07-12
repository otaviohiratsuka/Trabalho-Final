#include "preprocess/agrupar.hpp"
#include <sstream>  
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <utility> 
#include <algorithm> 
#include <unistd.h>
#include <sys/wait.h>
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



void escreverInputDat(const std::string& caminhoSaida, 
                     const std::unordered_map<int, std::vector<std::pair<int, float>>>& dadosAgrupados) {

    // 1. Convertemos o mapa para um vetor de ponteiros
    std::vector<const std::pair<const int, std::vector<std::pair<int, float>>>*> ptrs;
    ptrs.reserve(dadosAgrupados.size());
    for (const auto& par : dadosAgrupados) {
        ptrs.push_back(&par);
    }

    // 2. Determina número de processos baseado nos núcleos disponíveis
    long numProcessos = sysconf(_SC_NPROCESSORS_ONLN);
    if (numProcessos <= 0) numProcessos = 2;  // Fallback seguro

    // Limita o número de processos se houver poucos dados
    if (ptrs.size() < static_cast<size_t>(numProcessos) * 10) {
        numProcessos = std::max(1L, static_cast<long>(ptrs.size()) / 10);
    }

    // 3. Divide o trabalho entre os processos
    const size_t chunkSize = ptrs.size() / static_cast<size_t>(numProcessos);
    std::vector<pid_t> pids;
    pids.reserve(static_cast<size_t>(numProcessos));

    for (long i = 0; i < numProcessos; ++i) {
        pid_t pid = fork();
        if (pid == 0) {  // Processo filho
            size_t start = static_cast<size_t>(i) * chunkSize;
            size_t end = (i == numProcessos - 1) ? ptrs.size() : start + chunkSize;

            std::ofstream saida(caminhoSaida + ".tmp_" + std::to_string(i));
            if (!saida.is_open()) {
                std::cerr << "Erro ao criar arquivo temporário\n";
                exit(1);
            }

            for (size_t j = start; j < end; ++j) {
                const auto& par = *ptrs[j];
                saida << par.first;
                for (const auto& item : par.second) {
                    saida << " " << item.first << ":" << item.second;
                }
                saida << "\n";
            }
            exit(0);

        } else if (pid > 0) {
            pids.push_back(pid);
        } else {
            std::cerr << "Erro ao criar processo filho!\n";
            exit(1);
        }
    }

    // 4. Processo pai aguarda todos os filhos terminarem
    for (pid_t pid : pids) {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {  // Corrigido: parêntese fechado corretamente
            if (WEXITSTATUS(status) != 0) {
                std::cerr << "Processo filho falhou com código: " << WEXITSTATUS(status) << "\n";
            }
        }
    }

    // 5. Merge dos arquivos temporários
    std::ofstream saidaFinal(caminhoSaida);
    if (!saidaFinal.is_open()) {
        std::cerr << "Erro ao criar arquivo final\n";
        exit(1);
    }

    for (long i = 0; i < numProcessos; ++i) {
        std::string tmpFile = caminhoSaida + ".tmp_" + std::to_string(i);
        std::ifstream tmp(tmpFile);
        if (tmp.is_open()) {
            saidaFinal << tmp.rdbuf();
            tmp.close();
            std::remove(tmpFile.c_str());
        } else {
            std::cerr << "Aviso: Não foi possível abrir arquivo temporário " << tmpFile << "\n";
        }
    }
}