#include <algorithm>
#include <string>
#include <cmath>
#include <boost/tuple/tuple.hpp>
#include <functional>
#include <iostream>
#include <fstream>
#include "../classes/lattice.h"
#include "../misc/json.hpp"
using json = nlohmann::json;
#ifndef GRAPH
#define GRAPH
// struct Lattice;
std::vector<std::vector<double>> Lattice_calculation(std::vector<double> N, int Ini_mode, int Imax, double Beta);
std::vector<std::vector<double>> Lattice_calculation2(int N, int Imax, std::vector<double> Betalist);
void Saveas(json Jdata, std::string file_name);
void Graph1(std::vector<int> Nlist, int Iterations, double Beta);
void Graph2(int Nsize, int Iterations, std::vector<double> Betalist);

#endif