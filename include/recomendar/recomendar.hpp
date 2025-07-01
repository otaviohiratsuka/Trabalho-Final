#pragma once
#include <string>
#include <functional>
#include <stdexcept>



using AlgoritmoRecomendacao = std::function<void(const std::string&, const std::string&, const std::string&)>;

extern AlgoritmoRecomendacao algoritmoGlobal;

void recomendar(AlgoritmoRecomendacao algoritmo,
                   const std::string& inputPath,
                   const std::string& explorePath,
                   const std::string& outputPath);