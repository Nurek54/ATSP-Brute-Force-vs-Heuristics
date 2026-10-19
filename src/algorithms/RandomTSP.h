#ifndef RANDOMTSP_H
#define RANDOMTSP_H

#include "core/TSPInstance.h"
#include <vector>
#include <utility>  // Dla std::pair

using namespace std;

pair<vector<int>, int> tsp_random(const TSPInstance& instance);

// Ustawia ziarno dla rand(), wywolywane raz na starcie programu
void initialize_random_seed();

#endif // RANDOMTSP_H
