#include <algorithm>
#include <string>
#include <cmath>
#include <boost/tuple/tuple.hpp>
#include <functional>
#include "lattice.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
using json = nlohmann::json;
#ifndef GRAPH
#define GRAPH
// struct Lattice;
std::vector<std::vector<float>> Lattice_calculation(std::vector<float> N, int Ini_mode, int Imax, float Beta, int graphnumb);
void Graph1(std::vector<int> Nlist, int Iterations, float Beta);
#endif