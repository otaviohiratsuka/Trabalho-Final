#include "preprocess/preprocess.hpp"
#include "recomendar/recomendar.hpp"
#include "Jaccard.hpp"
#include <chrono>
#include <fstream> 
#include <unistd.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::chrono;

// Função para criar um subconjunto do input.dat para teste
void criarExploreTest(const string& inputPath, const string& explorePath, int numUsers = 100) {
    ifstream input(inputPath);
    ofstream explore(explorePath);
    
    if (!input.is_open() || !explore.is_open()) {
        cerr << "Erro ao abrir arquivos para criar explore test" << endl;
        return;
    }
    
    string linha;
    int count = 0;
    while (getline(input, linha) && count < numUsers) {
        explore << linha << "\n";
        count++;
    }
    
    input.close();
    explore.close();
    cout << "Criado arquivo explore_test.dat com " << count << " usuarios" << endl;
}

int main(){
    algoritmoGlobal = recomendarJaccard;

    auto start = high_resolution_clock::now();
    preprocessar(
        "datasets/ratings.csv",
        "datasets/input.dat"
    );
    auto mid = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(mid - start);
    cout << "Tempo de preprocessamento: " << duration.count() << " ms" << endl;
    
    // Teste da função Jaccard
    cout << "\n=== Teste da Funcao Jaccard ===" << endl;
    vector<int> set1 = {1, 2, 3, 4}; // Já ordenado
    vector<int> set2 = {3, 4, 5, 6}; // Já ordenado
    vector<int> set3 = {7, 8, 9};    // Já ordenado
    vector<int> set4 = {1, 2, 3, 4}; // Igual ao set1

    cout << "Jaccard({1,2,3,4}, {3,4,5,6}) = " << jaccard(set1, set2) << " (esperado: 0.333)" << endl;
    cout << "Jaccard({1,2,3,4}, {7,8,9}) = " << jaccard(set1, set3) << " (esperado: 0.0)" << endl;
    cout << "Jaccard({1,2,3,4}, {1,2,3,4}) = " << jaccard(set1, set4) << " (esperado: 1.0)" << endl;
    cout << "================================\n" << endl;

    // Criar arquivo de teste usando subconjunto do input
    criarExploreTest("datasets/input.dat", "datasets/explore_test.dat", 50);

    // Recomendar usando o arquivo de teste
    recomendar(
        algoritmoGlobal,
        "datasets/input.dat",
        "datasets/explore_test.dat",  // Usar arquivo de teste
        "outcome/output_test.dat"
    );

    auto end = high_resolution_clock::now();

    auto duration_preprocess = duration_cast<milliseconds>(mid - start);
    auto duration_recommend = duration_cast<milliseconds>(end - mid);
    auto duration_total = duration_cast<milliseconds>(end - start);

    cout << "\nRelatorio de Desempenho:\n";
    cout << " - Tempo de preprocessamento: " << duration_preprocess.count() << " ms\n";
    cout << " - Tempo de recomendacao: " << duration_recommend.count() << " ms\n";
    cout << " - Tempo total: " << duration_total.count() << " ms\n";

    string statusPath = "/proc/" + to_string(getpid()) + "/status";
    ifstream statusFile(statusPath);
    string linha;

    while(getline(statusFile, linha)) {
        if(linha.find("VmRSS:") == 0){
            cout << " - Uso de memoria: " << linha << "\n";
            break;
        }
    }
    
    cout << "\nSaida gerada em outcome/output_test.dat\n";
    
    // Verificar se as recomendações são diferentes
    cout << "\n=== Verificacao das Recomendacoes ===" << endl;
    ifstream output("outcome/output_test.dat");
    string linha1, linha2, linha3;
    if (getline(output, linha1) && getline(output, linha2) && getline(output, linha3)) {
        cout << "Usuario 1: " << linha1.substr(0, min(50, (int)linha1.length())) << "..." << endl;
        cout << "Usuario 2: " << linha2.substr(0, min(50, (int)linha2.length())) << "..." << endl;
        cout << "Usuario 3: " << linha3.substr(0, min(50, (int)linha3.length())) << "..." << endl;
        
        if (linha1 == linha2 || linha2 == linha3) {
            cout << "ATENCAO: Recomendacoes identicas detectadas!" << endl;
        } else {
            cout << "OK: Recomendacoes diferentes entre usuarios" << endl;
        }
    }
    output.close();

    return 0;
}