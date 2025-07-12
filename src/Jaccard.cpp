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
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>

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

struct ProcessData {
    int startIdx;
    int endIdx;
    int totalExploradores;
    char tempFileName[256];
};

void processarChunk(const vector<pair<int, vector<int>>>& exploradoresVec,
                   const vector<pair<int, vector<int>>>& perfisVec,
                   int startIdx, int endIdx, const string& tempFileName) {
    
    const int K = 10;
    const int MAX_RECOMMENDATIONS = 10;
    const double MIN_SIMILARITY = 0.01;
    
    vector<string> resultados;
    resultados.reserve(endIdx - startIdx);
    
    for (int idx = startIdx; idx < endIdx; ++idx) {
        const auto& [uidExplorador, filmesExplorador] = exploradoresVec[idx];
        
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> topK;
        
        for(const auto& [uid, filmes] : perfisVec){
            if (uid == uidExplorador) continue;
            
            double sim = jaccard(filmesExplorador, filmes);
            
            if (sim >= MIN_SIMILARITY) {
                if (topK.size() < K) {
                    topK.push({sim, uid});
                } else if (sim > topK.top().first) {
                    topK.pop();
                    topK.push({sim, uid});
                }
            }
        }
        
        vector<pair<double, int>> candidatos;
        candidatos.reserve(topK.size());
        
        while (!topK.empty()) {
            candidatos.push_back(topK.top());
            topK.pop();
        }
        
        reverse(candidatos.begin(), candidatos.end());

        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> topFilmes;
        unordered_map<int, double> filmeScore;
        
        for (const auto& [similaridade, similarUid] : candidatos) {
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

        for (const auto& [filme, score] : filmeScore) {
            if (topFilmes.size() < MAX_RECOMMENDATIONS) {
                topFilmes.push({score, filme});
            } else if (score > topFilmes.top().first) {
                topFilmes.pop();
                topFilmes.push({score, filme});
            }
        }

        vector<pair<double, int>> filmesOrdenados;
        filmesOrdenados.reserve(topFilmes.size());
        
        while (!topFilmes.empty()) {
            filmesOrdenados.push_back(topFilmes.top());
            topFilmes.pop();
        }
        
        reverse(filmesOrdenados.begin(), filmesOrdenados.end());

        stringstream ss;
        ss << uidExplorador;
        for (const auto& [score, filme] : filmesOrdenados) {
            ss << " " << filme;
        }
        
        resultados.push_back(ss.str());
    }
    
    ofstream tempFile(tempFileName);
    if (tempFile.is_open()) {
        for (const string& resultado : resultados) {
            tempFile << resultado << "\n";
        }
        tempFile.close();
    } else {
        cerr << "Erro ao abrir arquivo temporário: " << tempFileName << endl;
    }
}

void recomendarJaccard(
    const string& caminhoInput,
    const string& caminhoExplore,
    const string& caminhoOutput
) {
    auto perfis = lerPerfis(caminhoInput);
    auto exploradores = lerPerfis(caminhoExplore);

    vector<pair<int, vector<int>>> exploradoresVec;
    exploradoresVec.reserve(exploradores.size());
    for (const auto& [uid, filmes] : exploradores) {
        exploradoresVec.emplace_back(uid, filmes);
    }

    vector<pair<int, vector<int>>> perfisVec;
    perfisVec.reserve(perfis.size());
    for (const auto& [uid, filmes] : perfis) {
        perfisVec.emplace_back(uid, filmes);
    }

    int numProcessos = sysconf(_SC_NPROCESSORS_ONLN);
    if (numProcessos <= 0) numProcessos = 4;
    
    int totalExploradores = exploradoresVec.size();
    int chunkSize = (totalExploradores + numProcessos - 1) / numProcessos;
    
    vector<pid_t> processos;
    vector<string> arquivosTemp;
    
    for (int i = 0; i < numProcessos; ++i) {
        int startIdx = i * chunkSize;
        int endIdx = min(startIdx + chunkSize, totalExploradores);
        
        if (startIdx >= totalExploradores) break;
        
        string tempFileName = "temp_" + to_string(i) + "_" + to_string(getpid()) + ".dat";
        arquivosTemp.push_back(tempFileName);
        
        pid_t pid = fork();
        
        if (pid == -1) {
            cerr << "Erro ao criar processo filho: " << strerror(errno) << endl;
            continue;
        } else if (pid == 0) {
            processarChunk(exploradoresVec, perfisVec, startIdx, endIdx, tempFileName);
            exit(0);
        } else {
            processos.push_back(pid);
        }
    }
    
    for (pid_t pid : processos) {
        int status;
        waitpid(pid, &status, 0);
    }
    
    ofstream out(caminhoOutput);
    if (!out.is_open()) {
        cerr << "Erro ao abrir o arquivo: " << caminhoOutput << "\n";
        return;
    }
    
    for (const string& tempFileName : arquivosTemp) {
        ifstream tempFile(tempFileName);
        if (tempFile.is_open()) {
            string linha;
            while (getline(tempFile, linha)) {
                out << linha << "\n";
            }
            tempFile.close();
            
            if (remove(tempFileName.c_str()) != 0) {
                cerr << "Aviso: Não foi possível remover arquivo temporário " << tempFileName << endl;
            }
        } else {
            cerr << "Erro ao abrir arquivo temporário: " << tempFileName << endl;
        }
    }
    
    out.close();
}