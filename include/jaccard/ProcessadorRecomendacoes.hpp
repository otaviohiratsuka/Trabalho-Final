#ifndef PROCESSADOR_RECOMENDACOES_HPP
#define PROCESSADOR_RECOMENDACOES_HPP

#include <vector>
#include <string>
#include <utility>
#include "config.h" // Inclui a constante TOP_N_RECOMMENDATIONS

class ProcessadorRecomendacoes {
public:
    static void processarLote(
        const std::vector<std::pair<int, std::vector<int>>>& exploradores,
        const std::vector<std::pair<int, std::vector<int>>>& perfis,
        int inicio,
        int fim,
        const std::string& nomeArquivoTemp
    );
};

#endif