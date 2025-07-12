#include "jaccard/GerenciadorProcessos.hpp"
#include "jaccard/ProcessadorRecomendacoes.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

int GerenciadorProcessos::determinarNumeroProcessos(int numProcessosSugerido) {
    if (numProcessosSugerido > 0) return numProcessosSugerido;
    
    int numProcessos = sysconf(_SC_NPROCESSORS_ONLN);
    return (numProcessos <= 0) ? 4 : numProcessos;
}

void GerenciadorProcessos::executarProcessosParalelos(
    const std::vector<std::pair<int, std::vector<int>>>& exploradores,
    const std::vector<std::pair<int, std::vector<int>>>& perfis,
    const std::string& caminhoSaida,
    int numProcessos
) {
    numProcessos = determinarNumeroProcessos(numProcessos);
    int totalExploradores = exploradores.size();
    int chunkSize = (totalExploradores + numProcessos - 1) / numProcessos;
    
    std::vector<pid_t> processos;
    std::vector<std::string> arquivosTemp;
    
    for (int i = 0; i < numProcessos; ++i) {
        int startIdx = i * chunkSize;
        int endIdx = std::min(startIdx + chunkSize, totalExploradores);
        
        if (startIdx >= totalExploradores) break;
        
        std::string tempFileName = "temp_" + std::to_string(i) + "_" + std::to_string(getpid()) + ".dat";
        arquivosTemp.push_back(tempFileName);
        
        pid_t pid = fork();
        
        if (pid == -1) {
            std::cerr << "Erro ao criar processo filho\n";
            continue;
        } else if (pid == 0) {
            ProcessadorRecomendacoes::processarLote(exploradores, perfis, startIdx, endIdx, tempFileName);
            _exit(0);
        } else {
            processos.push_back(pid);
        }
    }
    
    for (pid_t pid : processos) {
        int status;
        waitpid(pid, &status, 0);
    }
    
    std::ofstream out(caminhoSaida);
    if (out.is_open()) {
        for (const std::string& tempFileName : arquivosTemp) {
            std::ifstream tempFile(tempFileName);
            if (tempFile.is_open()) {
                std::string linha;
                while (std::getline(tempFile, linha)) {
                    out << linha << "\n";
                }
                tempFile.close();
                std::remove(tempFileName.c_str());
            }
        }
        out.close();
    }
}