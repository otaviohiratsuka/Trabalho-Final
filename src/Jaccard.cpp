#include "Jaccard.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <omp.h>
#include <mutex>

using namespace std;

using Perfil = unordered_map<int, vector<int>>;

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
        vector<int> filmes;
        while(ss >> par){
            size_t pos = par.find(":");
            if (pos != string::npos){
                try {
                    int filmeId = stoi(par.substr(0, pos));
                    filmes.push_back(filmeId);
                } catch (const exception&) {
                    continue;
                }
            }
        }
        
        // Ordenar e remover duplicatas
        sort(filmes.begin(), filmes.end());
        filmes.erase(unique(filmes.begin(), filmes.end()), filmes.end());
        
        perfis[uid] = move(filmes);
    }
    arq.close();
    return perfis;
}

double jaccard(const vector<int>& a, const vector<int>& b){
    if (a.empty() && b.empty()) return 1.0;
    if (a.empty() || b.empty()) return 0.0;
    
    size_t intersec = 0;
    size_t i = 0, j = 0;
    
    // Algoritmo de merge otimizado para contar interseção
    while (i < a.size() && j < b.size()) {
        if (a[i] == b[j]) {
            intersec++;
            i++; 
            j++;
        } else if (a[i] < b[j]) {
            i++;
        } else {
            j++;
        }
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

    // Converter exploradores para vector para facilitar paralelização
    vector<pair<int, vector<int>>> exploradoresVec;
    exploradoresVec.reserve(exploradores.size());
    for (const auto& [uid, filmes] : exploradores) {
        exploradoresVec.emplace_back(uid, filmes);
    }

    // Converter perfis para vector para facilitar acesso paralelo
    vector<pair<int, vector<int>>> perfisVec;
    perfisVec.reserve(perfis.size());
    for (const auto& [uid, filmes] : perfis) {
        perfisVec.emplace_back(uid, filmes);
    }

    // Armazenar resultados de forma thread-safe
    vector<string> resultados(exploradoresVec.size());

    const int K = 10;
    const int MAX_RECOMMENDATIONS = 10;
    const double MIN_SIMILARITY = 0.01;

    // Variáveis para contagem thread-safe
    int processados = 0;
    mutex processadosMutex;

    cout << "Iniciando processamento paralelo com " << omp_get_max_threads() << " threads..." << endl;

    // Paralelizar o loop principal
    #pragma omp parallel for schedule(dynamic, 1)
    for (size_t idx = 0; idx < exploradoresVec.size(); ++idx) {
        const auto& [uidExplorador, filmesExplorador] = exploradoresVec[idx];
        
        // Min-Heap para Top-K usuários mais similares
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> topK;
        
        // Calcular similaridades e manter apenas os K maiores
        for(const auto& [uid, filmes] : perfisVec){
            if (uid == uidExplorador) continue;
            
            double sim = jaccard(filmesExplorador, filmes);
            
            // Só considerar usuários com similaridade mínima
            if (sim >= MIN_SIMILARITY) {
                if (topK.size() < K) {
                    topK.push({sim, uid});
                } else if (sim > topK.top().first) {
                    topK.pop();
                    topK.push({sim, uid});
                }
            }
        }
        
        // Converter heap para vector ordenado
        vector<pair<double, int>> candidatos;
        candidatos.reserve(topK.size());
        
        while (!topK.empty()) {
            candidatos.push_back(topK.top());
            topK.pop();
        }
        
        reverse(candidatos.begin(), candidatos.end());

        // Thread-safe debug (apenas para primeiros exploradores)
        if (idx < 5) {
            #pragma omp critical
            {
                cout << "Explorador " << uidExplorador << " (" << filmesExplorador.size() 
                     << " filmes), top similares: ";
                for (size_t i = 0; i < min((size_t)5, candidatos.size()); ++i) {
                    cout << "U" << candidatos[i].second << "(" 
                         << fixed << setprecision(3) << candidatos[i].first << ") ";
                }
                cout << endl;
            }
        }

        // Min-Heap para Top-K recomendações de filmes
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> topFilmes;
        
        // Gerar recomendações ponderadas pela similaridade
        unordered_map<int, double> filmeScore;
        
        for (const auto& [similaridade, similarUid] : candidatos) {
            // Encontrar perfil do usuário similar
            auto it = find_if(perfisVec.begin(), perfisVec.end(), 
                             [similarUid](const pair<int, vector<int>>& p) {
                                 return p.first == similarUid;
                             });
            
            if (it != perfisVec.end()) {
                const auto& filmesDoUsuario = it->second;
                for (int filme : filmesDoUsuario) {
                    if (!binary_search(filmesExplorador.begin(), filmesExplorador.end(), filme)) {
                        filmeScore[filme] += similaridade;
                    }
                }
            }
        }

        // Usar min-heap para manter apenas os MAX_RECOMMENDATIONS melhores filmes
        for (const auto& [filme, score] : filmeScore) {
            if (topFilmes.size() < MAX_RECOMMENDATIONS) {
                topFilmes.push({score, filme});
            } else if (score > topFilmes.top().first) {
                topFilmes.pop();
                topFilmes.push({score, filme});
            }
        }

        // Converter heap de filmes para vector ordenado
        vector<pair<double, int>> filmesOrdenados;
        filmesOrdenados.reserve(topFilmes.size());
        
        while (!topFilmes.empty()) {
            filmesOrdenados.push_back(topFilmes.top());
            topFilmes.pop();
        }
        
        reverse(filmesOrdenados.begin(), filmesOrdenados.end());

        // Thread-safe debug das recomendações (apenas para primeiros exploradores)
        if (idx < 5) {
            #pragma omp critical
            {
                cout << "  Top recomendações: ";
                for (size_t i = 0; i < min((size_t)5, filmesOrdenados.size()); ++i) {
                    cout << "F" << filmesOrdenados[i].second << "(" 
                         << fixed << setprecision(2) << filmesOrdenados[i].first << ") ";
                }
                cout << endl;
            }
        }

        // Construir string de resultado
        stringstream ss;
        ss << uidExplorador;
        for (const auto& [score, filme] : filmesOrdenados) {
            ss << " " << filme;
        }
        
        resultados[idx] = ss.str();
        
        // Atualizar contador de forma thread-safe
        {
            lock_guard<mutex> lock(processadosMutex);
            processados++;
            if (processados % 100 == 0) {
                cout << "Processados: " << processados << " exploradores" << endl;
            }
        }
    }

    // Escrever resultados no arquivo
    ofstream out(caminhoOutput);
    if(!out.is_open()){
        cerr << "Erro ao abrir o arquivo: " << caminhoOutput << "\n";
        return;
    }

    for (const string& resultado : resultados) {
        out << resultado << "\n";
    }

    out.close();
    cout << "Recomendacoes geradas para " << processados << " usuarios em " << caminhoOutput << endl;
}