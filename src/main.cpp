#include "preprocess/preprocess.hpp"
#include "recomendar/recomendar.hpp"
#include "jaccard/Jaccard.hpp"
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
}

int main(){
    algoritmoGlobal = recomendarJaccard;

    auto start = high_resolution_clock::now();
    preprocessar(
        "datasets/ratings.csv",
        "datasets/input.dat"
    );
    auto mid = high_resolution_clock::now();
    
    // Criar arquivo de teste usando subconjunto do input
    criarExploreTest("datasets/input.dat", "datasets/explore.dat", 50);

    // Recomendar usando o arquivo de teste
    recomendar(
        algoritmoGlobal,
        "datasets/input.dat",
        "datasets/explore.dat",
        "outcome/output.dat"
    );

    auto end = high_resolution_clock::now();

    auto duration_preprocess = duration_cast<milliseconds>(mid - start);
    auto duration_recommend = duration_cast<milliseconds>(end - mid);
    auto duration_total = duration_cast<milliseconds>(end - start);

    cout << "Tempo de preprocessamento: " << duration_preprocess.count() << " ms\n";
    cout << "Tempo de recomendacao: " << duration_recommend.count() << " ms\n";
    cout << "Tempo total: " << duration_total.count() << " ms\n";

     string statusPath = "/proc/" + to_string(getpid()) + "/status";
    ifstream statusFile(statusPath);
    string linha;

    while(getline(statusFile, linha)) {
        if(linha.find("VmRSS:") == 0){
            cout << " - Uso de memoria: " << linha << "\n";
            break;
        }
    }

    return 0;
}