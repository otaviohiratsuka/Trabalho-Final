#include "preprocess/preprocess.hpp"
#include "preprocess/ler.hpp"
#include "preprocess/agrupar.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <tuple>

using namespace std;


void preprocessar(
    const string& caminhoCsvRatings,
    const string& caminhoOutputInputDat,
    int maxLinhas
) {
    unordered_map<int,int> contUsuarios;
    unordered_map<int,int> contFilmes;

      // pré‑alocação antecipada
    if (maxLinhas > 0) {
        contUsuarios.reserve(maxLinhas / 20);
        contFilmes  .reserve(maxLinhas / 20);
    }

    // Passo 1: ler e filtrar avaliações
    auto avaliacoes = lerAvaliacoes(caminhoCsvRatings,contUsuarios, contFilmes, maxLinhas);
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

