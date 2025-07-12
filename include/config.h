#ifndef CONFIG_H
#define CONFIG_H

// Configurações do algoritmo de recomendação
const int TOP_K_SIMILAR_USERS = 10;      // Número de usuários similares a considerar
const int TOP_N_RECOMMENDATIONS = 10;    // Número máximo de recomendações por usuário
const double MIN_SIMILARITY_THRESHOLD = 0.01;  // Similaridade mínima para considerar

#endif