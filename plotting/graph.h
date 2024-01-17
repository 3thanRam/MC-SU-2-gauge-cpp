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
std::vector<double> Lattice_Plaqcalculation(int N, bool inimode, int Imax, double Beta, int Multithreadmode);
std::vector<std::vector<double>> Lattice_calculation2(int N, int Imax, std::vector<double> Betalist, int Multithreadmode);
void Saveas(json Jdata, std::string file_name);

void Graph1(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode);
void Graph2(int Nsize, int Iterations, std::vector<double> Betalist, int Multithreadmode);
void Graph3(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode);
void Graph4(int Nsize, int Iterations, std::vector<double> Betalist, int Multithreadmode);

#endif