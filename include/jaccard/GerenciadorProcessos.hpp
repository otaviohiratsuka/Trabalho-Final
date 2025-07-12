#ifndef GERENCIADOR_PROCESSOS_HPP
#define GERENCIADOR_PROCESSOS_HPP

#include <vector>
#include <string>

class GerenciadorProcessos {
public:
    static void executarProcessosParalelos(
        const std::vector<std::pair<int, std::vector<int>>>& exploradores,
        const std::vector<std::pair<int, std::vector<int>>>& perfis,
        const std::string& caminhoSaida,
        int numProcessos = 0
    );
    
private:
    static int determinarNumeroProcessos(int numProcessosSugerido);
};

#endif