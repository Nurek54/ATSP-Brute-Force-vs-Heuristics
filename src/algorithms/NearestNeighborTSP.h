#ifndef NEARESTNEIGHBORTSP_H
#define NEARESTNEIGHBORTSP_H

#include "core/TSPInstance.h"
#include <vector>
#include <utility> // std::pair

using namespace std;

pair<vector<int>, int> tsp_nearest_neighbor(const TSPInstance& instance);

#endif // NEARESTNEIGHBORTSP_H
