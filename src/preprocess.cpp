#include "preprocess.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>

using namespace std;

vector<Avaliacao> lerAvaliacoes(
    const string& caminhoCsv,
    int maxLinhas,
    unordered_map<int,int>& contagemUsuarios,
    unordered_map<int,int>& contagemFilmes
) {
    ifstream arquivo(caminhoCsv);
    if (!arquivo.is_open()) {
        cerr << "Erro ao abrir arquivo: " << caminhoCsv << "\n";
        return {};
    }

    vector<Avaliacao> avaliacoes;
    unordered_set<Avaliacao,AvaliacaoHash> unicas;

    // Reservar espaço para otimizar alocação
    avaliacoes.reserve(maxLinhas);
    unicas.reserve(maxLinhas);
    unicas.max_load_factor(1.0f);
    contagemUsuarios.reserve(maxLinhas/20);
    contagemUsuarios.max_load_factor(1.0f);
    contagemFilmes.reserve(maxLinhas/50);
    contagemFilmes.max_load_factor(1.0f);

    // Buffer fixo para leitura rápida de linhas
    static char buffer[256];
    arquivo.getline(buffer, sizeof(buffer)); // pula cabeçalho

    int lidos = 0;
    while (lidos < maxLinhas && arquivo.getline(buffer, sizeof(buffer))) {
        int usuarioId, filmeId;
        float nota;

        // sscanf: parsing rápido de CSV simples
        if (sscanf(buffer, "%d,%d,%f", &usuarioId, &filmeId, &nota) != 3)
            continue;

        // Descartar notas fora do intervalo válido
        if (nota < 0.5f || nota > 5.0f)
            continue;

        Avaliacao a{usuarioId, filmeId, nota};
        // Insere se for avaliação única (sem duplicados)
        if (unicas.insert(a).second) {
            avaliacoes.push_back(a);
            contagemUsuarios[usuarioId]++;
            contagemFilmes[filmeId]++;
        }
        lidos++;
    }

    arquivo.close();
    return avaliacoes;
}

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

void escreverInputDat(
    const string& caminhoSaida,
    const unordered_map<int, vector<pair<int,float>>>& dadosAgrupados
) {
    ofstream saida(caminhoSaida);
    if (!saida.is_open()) {
        cerr << "Erro ao criar arquivo: " << caminhoSaida << "\n";
        return;
    }
    for (const auto& par : dadosAgrupados) {
        saida << par.first;
        for (const auto& item : par.second)
            saida << " " << item.first << ":" << item.second;
        saida << "\n";
    }
    saida.close();
}

void preprocessar(
    const string& caminhoCsvRatings,
    const string& caminhoOutputInputDat,
    int maxLinhas
) {
    unordered_map<int,int> contUsuarios;
    unordered_map<int,int> contFilmes;

    // Passo 1: ler e filtrar avaliações
    auto avaliacoes = lerAvaliacoes(caminhoCsvRatings, maxLinhas, contUsuarios, contFilmes);
    // Passo 2: filtrar usuários e filmes pelos limites mínimos
    auto usuariosValidos = filtrarPorContagemMinima(contUsuarios, 50);
    auto filmesValidos = filtrarPorContagemMinima(contFilmes, 50);
    
    // Passo 3: agrupar avaliações por usuário
    auto dadosAgrupados = agruparAvaliacoesPorUsuario(avaliacoes, usuariosValidos, filmesValidos);
    
    // Passo 4: escrever arquivo input.dat
    escreverInputDat(caminhoOutputInputDat, dadosAgrupados);

    cout << "Pré-processamento concluído: " << dadosAgrupados.size() << " usuarios escritos em "
         << caminhoOutputInputDat << "\n";
}
