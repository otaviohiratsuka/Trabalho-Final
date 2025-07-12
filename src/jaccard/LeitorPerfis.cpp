#include "jaccard/LeitorPerfis.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

Perfil LeitorPerfis::lerArquivo(const std::string& caminho) {
    Perfil perfis;
    std::ifstream arq(caminho);
    
    if (!arq.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << caminho << "\n";
        return perfis;
    }
    
    std::string linha;
    while (std::getline(arq, linha)) {
        if (linha.empty()) continue;
        
        std::stringstream ss(linha);
        int uid;
        ss >> uid;

        std::string par;
        std::vector<int> filmes;
        
        while(ss >> par) {
            size_t pos = par.find(":");
            if (pos != std::string::npos) {
                try {
                    int filmeId = std::stoi(par.substr(0, pos));
                    filmes.push_back(filmeId);
                } catch (const std::exception&) {
                    continue;
                }
            }
        }
        
        std::sort(filmes.begin(), filmes.end());
        filmes.erase(std::unique(filmes.begin(), filmes.end()), filmes.end());
        
        perfis[uid] = std::move(filmes);
    }
    
    arq.close();
    return perfis;
}