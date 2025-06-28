#include "Jaccard.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

using Perfil = unordered_map<int, unordered_set<int>>;

Perfil lerPerfis(const string& caminho){
    Perfil perfis;
    ifstream arq(caminho);
    string linha;

    while (getline(arq, linha)){
        stringstream ss(linha);
        int uid;
        ss >> uid;

        string par;
        unordered_set<int> filmes;
        while(ss >> par){
            size_t pos = par.find(":");
            if (pos != string::npos){
                int filmeId = stoi (par.substr(0,pos));
                filmes.insert(filmeId);
            }
        }
        perfis[uid] = filmes;
    }
    return perfis;
}

double jaccard(const unordered_set<int>& a, const unordered_set<int>& b){
    size_t intersec = 0;
    for (int filme : a) {
    if(b.count(filme)) intersec++;
}

size_t uniao = a.size() + b.size() - intersec;
        return uniao == 0 ? 0.0 : static_cast <double> (intersec) / uniao;
}

void recomendarJaccard(
    const string& caminhoInput,
    const string& caminhoExplore,
    const string& caminhoOutput
) {

    auto perfis =  lerPerfis(caminhoInput);
    auto exploradores = lerPerfis (caminhoExplore);

    ofstream out(caminhoOutput);
    if(!out.is_open()){
        cerr << "Erro ao abrir o arquivo output.dat\n";
        return;
    }

    for (const auto& [uidExplorador, filmesExplorador]: exploradores){
        double maiorSimilaridade = -1.0;
        int usuarioMaisSimilar = -1;

        for(const auto& [uid, filmes]: perfis){
            double sim = jaccard(filmesExplorador, filmes);
            if(sim > maiorSimilaridade){
                maiorSimilaridade = sim;
                usuarioMaisSimilar = uid;
            }
        }

        unordered_set<int> recomedacoes;
        for(int filme : perfis[usuarioMaisSimilar]){
            if(!filmesExplorador.count(filme))
            recomedacoes.insert(filme);
        }

        out << uidExplorador;
        for (int filme : recomedacoes)
        out << " " << filme;
        out << "\n";
    }

    out.close();

}