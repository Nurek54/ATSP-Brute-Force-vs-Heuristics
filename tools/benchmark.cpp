// Pomiar trzech algorytmow na losowych macierzach ATSP.
// Czas jest odczytywany z CSV, ktory zapisuja same algorytmy (pomiar bez wczytywania i zapisu).
// Wynik: CSV na stdout, kolumny algorithm,n,instance,ms,cost,optimum
//
// Uzycie: benchmark [nMax=12] [instancje=20]
#include "algorithms/BruteForceTSP.h"
#include "algorithms/NearestNeighborTSP.h"
#include "algorithms/RandomTSP.h"
#include "core/TSPInstance.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Ten sam schemat co w programie: koszty 1..100, przekatna -1
static vector<vector<int>> randomMatrix(int n, unsigned seed) {
    mt19937 rng(seed);
    uniform_int_distribution<int> cost(1, 100);
    vector<vector<int>> m(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            m[i][j] = (i == j) ? -1 : cost(rng);
    return m;
}

// Ostatni wiersz pliku: "Algorytm | s | ms | ns | trasa | koszt", ms z przecinkiem dziesietnym
static double lastMilliseconds(const string& csv) {
    ifstream in(csv);
    string line, last;
    while (getline(in, line))
        if (!line.empty()) last = line;
    size_t a = last.find(" | ");
    size_t b = last.find(" | ", a + 3);
    size_t c = last.find(" | ", b + 3);
    string ms = last.substr(b + 3, c - b - 3);
    for (char& ch : ms)
        if (ch == ',') ch = '.';
    return stod(ms);
}

int main(int argc, char** argv) {
    int nMax = argc > 1 ? stoi(argv[1]) : 12;
    int instances = argc > 2 ? stoi(argv[2]) : 20;

    fs::path work = fs::temp_directory_path() / "atsp-benchmark";
    fs::create_directories(work);
    fs::current_path(work);

    cout << "algorithm,n,instance,ms,cost,optimum\n";
    for (int n = 4; n <= nMax; ++n) {
        // Brute force rosnie jak n!, dla najwiekszych n mniej powtorzen
        int count = n <= 10 ? instances : (n == 11 ? max(1, instances / 5) : max(1, instances / 20));
        for (int k = 0; k < count; ++k) {
            TSPInstance inst(randomMatrix(n, 1000u * n + k));
            for (const char* f : {"BruteForceResults.csv", "NearestNeighborResults.csv", "RandomResults.csv"})
                fs::remove(f);

            int opt = tsp_bruteforce(inst).second;
            double bfMs = lastMilliseconds("BruteForceResults.csv");
            int nn = tsp_nearest_neighbor(inst).second;
            double nnMs = lastMilliseconds("NearestNeighborResults.csv");
            int rnd = tsp_random(inst).second;
            double rndMs = lastMilliseconds("RandomResults.csv");

            printf("brute_force,%d,%d,%.6f,%d,%d\n", n, k, bfMs, opt, opt);
            printf("nearest_neighbor,%d,%d,%.6f,%d,%d\n", n, k, nnMs, nn, opt);
            printf("random,%d,%d,%.6f,%d,%d\n", n, k, rndMs, rnd, opt);
            fflush(stdout);
        }
        cerr << "n=" << n << " gotowe\n";
    }
    return 0;
}
