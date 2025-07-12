#include "jaccard/CalculadorSimilaridade.hpp"
#include <algorithm>

double CalculadorSimilaridade::calcularJaccard(const std::vector<int>& a, const std::vector<int>& b) {
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