#include "jaccard/ProcessadorRecomendacoes.hpp"
#include "jaccard/CalculadorSimilaridade.hpp"
#include <fstream>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <sstream>

void ProcessadorRecomendacoes::processarLote(
    const std::vector<std::pair<int, std::vector<int>>>& exploradores,
    const std::vector<std::pair<int, std::vector<int>>>& perfis,
    int inicio,
    int fim,
    const std::string& nomeArquivoTemp
) {
    const int K = 10; // Número de usuários similares a considerar
    const int MAX_RECOMMENDATIONS = 10; // Número máximo de recomendações
    const double MIN_SIMILARITY = 0.01; // Similaridade mínima
    
    std::vector<std::string> resultados;
    resultados.reserve(fim - inicio);
    
    for (int idx = inicio; idx < fim; ++idx) {
        const auto& [uidExplorador, filmesExplorador] = exploradores[idx];
        
        // Passo 1: Encontrar K usuários mais similares
        std::priority_queue<std::pair<double, int>, 
                          std::vector<std::pair<double, int>>, 
                          std::greater<std::pair<double, int>>> topK;
        
        for(const auto& [uid, filmes] : perfis) {
            if (uid == uidExplorador) continue;
            
            double sim = CalculadorSimilaridade::calcularJaccard(filmesExplorador, filmes);
            
            if (sim >= MIN_SIMILARITY) {
                if (topK.size() < K) {
                    topK.push({sim, uid});
                } else if (sim > topK.top().first) {
                    topK.pop();
                    topK.push({sim, uid});
                }
            }
        }
        
        // Converter para vetor ordenado (maior similaridade primeiro)
        std::vector<std::pair<double, int>> usuariosSimilares;
        while (!topK.empty()) {
            usuariosSimilares.push_back(topK.top());
            topK.pop();
        }
        std::reverse(usuariosSimilares.begin(), usuariosSimilares.end());

        // Passo 2: Gerar recomendações baseadas nos similares
        std::unordered_map<int, double> filmeScores;
        
        for (const auto& [similaridade, similarUid] : usuariosSimilares) {
            auto it = std::find_if(perfis.begin(), perfis.end(), 
                                [similarUid](const auto& p) {
                                    return p.first == similarUid;
                                });
            
            if (it != perfis.end()) {
                for (int filme : it->second) {
                    if (!std::binary_search(filmesExplorador.begin(), 
                                         filmesExplorador.end(), 
                                         filme)) {
                        filmeScores[filme] += similaridade;
                    }
                }
            }
        }

        // Passo 3: Selecionar os melhores filmes
        std::priority_queue<std::pair<double, int>> topFilmes;
        for (const auto& [filme, score] : filmeScores) {
            topFilmes.emplace(score, filme);
        }

        // Coletar resultados ordenados
        std::vector<int> filmesRecomendados;
        while (!topFilmes.empty() && filmesRecomendados.size() < MAX_RECOMMENDATIONS) {
            filmesRecomendados.push_back(topFilmes.top().second);
            topFilmes.pop();
        }

        // Passo 4: Formatar saída
        std::stringstream ss;
        ss << uidExplorador;
        for (int filme : filmesRecomendados) {
            ss << " " << filme;
        }
        resultados.push_back(ss.str());
    }
    
    // Escrever resultados no arquivo temporário
    std::ofstream tempFile(nomeArquivoTemp);
    if (tempFile.is_open()) {
        for (const auto& linha : resultados) {
            tempFile << linha << "\n";
        }
    }
}