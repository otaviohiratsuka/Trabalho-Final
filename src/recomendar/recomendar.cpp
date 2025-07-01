#include "recomendar/recomendar.hpp"

AlgoritmoRecomendacao algoritmoGlobal = nullptr;

void recomendar(AlgoritmoRecomendacao algoritmo,
                    const std::string& inputPath,
                    const std::string& explorePath,
                    const std::string& outputPath) {
    if (!algoritmo && algoritmoGlobal) {
        algoritmo = algoritmoGlobal;
    }

    if(algoritmo) {
        algoritmo(inputPath, explorePath, outputPath);
    } else {
        throw std::runtime_error("Nenhum algoritmo de recomendacao foi definido.");
    }
}