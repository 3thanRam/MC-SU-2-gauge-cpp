#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <random>

#ifndef UTIL
#define UTIL
std::vector<double> get_ini_rand_elem();
double Random();
int Choice(double param);
int Mod(int a, int b);
extern std::random_device rd;
extern std::default_random_engine generator;
extern std::uniform_real_distribution<double> unidistribution;
double get_ao(double beta, double k);
std::vector<double> PauliInv(std::vector<double> a);
std::vector<double> Paulimult(std::vector<double> Ma, std::vector<double> Mb);
double PauliDet(std::vector<double> Ulink);
std::vector<double> Rotate_3Dvector_random(std::vector<double> vect);
double Trace(std::vector<double> Ulist);

#endif