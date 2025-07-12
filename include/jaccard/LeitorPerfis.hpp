#ifndef LEITOR_PERFIS_HPP
#define LEITOR_PERFIS_HPP

#include <unordered_map>
#include <vector>
#include <string>

using Perfil = std::unordered_map<int, std::vector<int>>;

class LeitorPerfis {
public:
    static Perfil lerArquivo(const std::string& caminho);
};

#endif