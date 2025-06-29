#include "Jaccard.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

using Perfil = unordered_map<int, unordered_set<int>>;

Perfil lerPerfis(const string& caminho){
    Perfil perfis;
    ifstream arq(caminho);
    if (!arq.is_open()) {
        cerr << "Erro ao abrir arquivo: " << caminho << "\n";
        return perfis;
    }
    
    string linha;
    while (getline(arq, linha)){
        if (linha.empty()) continue;
        
        stringstream ss(linha);
        int uid;
        ss >> uid;

        string par;
        unordered_set<int> filmes;
        while(ss >> par){
            size_t pos = par.find(":");
            if (pos != string::npos){
                try {
                    int filmeId = stoi(par.substr(0, pos));
                    filmes.insert(filmeId);
                } catch (const exception&) {
                    // Ignora entradas inválidas
                    continue;
                }
            }
        }
        perfis[uid] = move(filmes);
    }
    arq.close();
    return perfis;
}

double jaccard(const unordered_set<int>& a, const unordered_set<int>& b){
    if (a.empty() && b.empty()) return 1.0;
    if (a.empty() || b.empty()) return 0.0;
    
    size_t intersec = 0;
    // Otimização: iterar sobre o menor conjunto
    const auto& menor = (a.size() < b.size()) ? a : b;
    const auto& maior = (a.size() < b.size()) ? b : a;
    
    for (int filme : menor) {
        if (maior.count(filme)) intersec++;
    }

    size_t uniao = a.size() + b.size() - intersec;
    return static_cast<double>(intersec) / uniao;
}

void recomendarJaccard(
    const string& caminhoInput,
    const string& caminhoExplore,
    const string& caminhoOutput
) {
    cout << "Carregando perfis de input..." << endl;
    auto perfis = lerPerfis(caminhoInput);
    cout << "Perfis carregados: " << perfis.size() << endl;
    
    cout << "Carregando perfis de exploracao..." << endl;
    auto exploradores = lerPerfis(caminhoExplore);
    cout << "Exploradores carregados: " << exploradores.size() << endl;

    ofstream out(caminhoOutput);
    if(!out.is_open()){
        cerr << "Erro ao abrir o arquivo: " << caminhoOutput << "\n";
        return;
    }

    int processados = 0;
    for (const auto& [uidExplorador, filmesExplorador] : exploradores){
        double maiorSimilaridade = -1.0;
        int usuarioMaisSimilar = -1;

        // Encontrar usuário mais similar
        for(const auto& [uid, filmes] : perfis){
            if (uid == uidExplorador) continue; // Evita auto-comparação
            
            double sim = jaccard(filmesExplorador, filmes);
            if(sim > maiorSimilaridade){
                maiorSimilaridade = sim;
                usuarioMaisSimilar = uid;
            }
        }

        // Gerar recomendações
        unordered_set<int> recomendacoes;
        if (usuarioMaisSimilar != -1 && perfis.count(usuarioMaisSimilar)) {
            for(int filme : perfis[usuarioMaisSimilar]){
                if(!filmesExplorador.count(filme)) {
                    recomendacoes.insert(filme);
                }
            }
        }

        // Escrever resultado
        out << uidExplorador;
        for (int filme : recomendacoes) {
            out << " " << filme;
        }
        out << "\n";
        
        processados++;
        if (processados % 100 == 0) {
            cout << "Processados: " << processados << " exploradores" << endl;
        }
    }

    out.close();
    cout << "Recomendacoes geradas para " << processados << " usuarios em " << caminhoOutput << endl;
}