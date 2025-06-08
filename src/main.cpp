#include "preprocess.hpp"
#include <chrono>

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
    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start);
    cout << "Tempo de processamento: " << duration.count() << " ms" << endl;

}