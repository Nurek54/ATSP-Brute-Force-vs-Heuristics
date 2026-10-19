#include "app/Menu.h"
#include "core/TSPInstance.h"
#include "algorithms/BruteForceTSP.h"
#include "algorithms/NearestNeighborTSP.h"
#include "algorithms/RandomTSP.h"
#include "core/TSPUtilities.h"
#include "app/TSPSimulation.h"
#include <iostream>
#include <fstream>

#ifndef ATSP_DATA_DIR
#define ATSP_DATA_DIR "data"
#endif

// Funkcja obsługująca całe menu
void Menu::run() {
    int option = -1;

    while (true) {
        showMenu();
        cin >> option;

        if (option == 0) {
            cout << "Koniec programu.\n";
            break;  // Zakoncz program
        }

        switch (option) {
            case 1:
                bruteForceOption();
                break;
            case 2:
                nearestNeighborOption();
                break;
            case 3:
                randomAlgorithmOption();
                break;
            case 4:
                generateRandomMatrixOption();
                break;
            case 5:
                loadMatrixFromFileOption();
                break;
            case 6:
                runSimulationOption();
                break;
            default:
                cout << "Niepoprawna opcja. Sprobuj ponownie.\n";
        }
    }
}

// Funkcja wyswietlajaca menu
void Menu::showMenu() {
    cout << "===========================\n";
    cout << "  Problem komiwojazera (TSP)\n";
    cout << "===========================\n";
    cout << "1. Uruchom Brute Force\n";
    cout << "2. Uruchom Nearest Neighbor\n";
    cout << "3. Uruchom Random Algorithm\n";
    cout << "4. Wygeneruj nowa losowa macierz odleglosci\n";
    cout << "5. Wczytaj dane z gotowego pliku\n";
    cout << "6. Symulacja na losowych macierzach\n";
    cout << "0. Wyjscie\n";
    cout << "Wybierz opcje: ";
}

// Funkcja pytajaca o nazwe pliku
string Menu::getFileNameFromUser(const string& prompt) {
    string filename;
    cout << prompt;
    cin >> filename;
    return filename;
}

// Funkcja do zapisywania macierzy do pliku
void Menu::saveMatrixToFile(const vector<vector<int>>& matrix, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        int size = matrix.size();
        outFile << size << endl;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                outFile << matrix[i][j] << " ";
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Losowa macierz zapisana do pliku: " << filename << endl;
    } else {
        cerr << "Nie mozna zapisac macierzy do pliku!" << endl;
    }
}

// Wypisuje trase i jej koszt, czas wykonania trafia do pliku CSV
void Menu::printResult(const string& name, const pair<vector<int>, int>& result, const string& csvFile) {
    const vector<int>& path = result.first;
    if (path.empty()) {
        return;
    }
    cout << name << ": koszt " << result.second << "\n";
    cout << "Trasa: ";
    for (int city : path) {
        cout << city << " -> ";
    }
    cout << path.front() << "\n";
    cout << "Czas i trasa dopisane do " << csvFile << "\n";
}

// Opcje menu
void Menu::bruteForceOption() {
    if (!fileLoaded) {
        cout << "Najpierw zaladuj macierz odleglosci (opcja 4 lub 5).\n";
        return;
    }
    TSPInstance instance(inputFilename);
    printResult("Brute Force", tsp_bruteforce(instance), "BruteForceResults.csv");
    cout << endl;
}

void Menu::nearestNeighborOption() {
    if (!fileLoaded) {
        cout << "Najpierw zaladuj macierz odleglosci (opcja 4 lub 5).\n";
        return;
    }
    TSPInstance instance(inputFilename);
    printResult("Nearest Neighbor", tsp_nearest_neighbor(instance), "NearestNeighborResults.csv");
    cout << endl;
}

void Menu::randomAlgorithmOption() {
    if (!fileLoaded) {
        cout << "Najpierw zaladuj macierz odleglosci (opcja 4 lub 5).\n";
        return;
    }
    TSPInstance instance(inputFilename);
    printResult("Random", tsp_random(instance), "RandomResults.csv");
    cout << endl;
}

void Menu::generateRandomMatrixOption() {
    int size, maxValue;
    cout << "Podaj rozmiar macierzy: ";
    cin >> size;
    cout << "Podaj maksymalna wartosc odleglosci: ";
    cin >> maxValue;

    vector<vector<int>> matrix = Utilities::generate_random_matrix(size, 1, maxValue);
    inputFilename = getFileNameFromUser("Podaj nazwe pliku do zapisania macierzy (np. matrix.atsp): ");
    saveMatrixToFile(matrix, inputFilename);
    fileLoaded = true;
}

void Menu::loadMatrixFromFileOption() {
    while (true) {
        // Nazwa trafia do inputFilename dopiero po udanym otwarciu,
        // zeby nieudana proba nie zepsula wczesniej wczytanego pliku
        string name = getFileNameFromUser("Podaj nazwe pliku z macierza odleglosci (np. matrix_6x6.atsp): ");
        ifstream file(name);
        if (!file.is_open()) {
            // Sprobuj jeszcze w folderze data/ projektu
            string inData = string(ATSP_DATA_DIR) + "/" + name;
            file.open(inData);
            if (file.is_open()) {
                name = inData;
            }
        }
        cout << "Probuje otworzyc plik: " << name << endl;

        if (!file.is_open()) {
            cerr << "Nie mozna otworzyc pliku: " << name << endl;
            cout << "Czy chcesz sprobowac ponownie? (t/n): ";
            char retry;
            cin >> retry;
            if (retry == 'n' || retry == 'N') {
                cout << "Anulowano wczytywanie pliku.\n";
                return;
            }
        } else {
            inputFilename = name;
            cout << "Plik z macierza " << inputFilename << " wczytany.\n";
            fileLoaded = true;
            return;
        }
    }
}

void Menu::runSimulationOption() {
    int numMatrices, matrixSize, maxCost;
    cout << "Podaj liczbe macierzy do symulacji: ";
    cin >> numMatrices;
    cout << "Podaj rozmiar macierzy: ";
    cin >> matrixSize;
    cout << "Podaj maksymalna wartosc odleglosci: ";
    cin >> maxCost;

    cout << endl;

    TSPSimulation simulation(numMatrices, matrixSize, maxCost);
    simulation.runSimulation();
}

