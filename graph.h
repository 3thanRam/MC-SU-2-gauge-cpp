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
std::vector<std::vector<double>> Lattice_calculation(std::vector<double> N, int Ini_mode, int Imax, double Beta, int graphnumb);
void Graph1(std::vector<int> Nlist, int Iterations, double Beta);
#endif