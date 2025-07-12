#ifndef CALCULADOR_SIMILARIDADE_HPP
#define CALCULADOR_SIMILARIDADE_HPP

#include <vector>

class CalculadorSimilaridade {
public:
    static double calcularJaccard(const std::vector<int>& a, const std::vector<int>& b);
};

#endif