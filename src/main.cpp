#include "preprocess.hpp"
#include "Jaccard.hpp"
#include <chrono>
#include <fstream> 
#include <unistd.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace std::chrono;

int main(){
    auto start = high_resolution_clock::now();
    preprocessar(
        "datasets/ratings.csv",
        "datasets/input.dat",
        2500000
    );
    auto mid = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(mid - start);
    cout << "Tempo de processamento: " << duration.count() << " ms" << endl;
    
    recomendarJaccard(
        "datasets/input.dat",
        "datasets/explore.dat",
        "datasets/output.dat"
    );

    auto end = high_resolution_clock::now();

    auto duration_preprocess = duration_cast<milliseconds>(mid - start);
    auto duration_recommend = duration_cast<milliseconds>(end - mid);
    auto duration_total = duration_cast<milliseconds>(end - start);

    cout << "\nRelatorio de Desempenho: \n";
    cout << " - tempo de recomendação: " << duration_preprocess.count() << " ms\n";
    cout << " - tempo total: " << duration_total.count() << " ms/n";

    string statusPath = "/proc/" + to_string(getpid()) + "/status";
    ifstream statusFiles(statusPath);
    string linha;

    while(getline(statusFiles, linha)) {
        if(linha.find("VmRSS:") == 0){
            cout << " - Uso de memoria: " << linha << "\n";
            break;
        }
    }
    
    cout << "\nSaida gerada em datasets/output.dat\n";

    return 0;

}