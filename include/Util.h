#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <random>
#include <thread>
#include <future>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <algorithm>
#include <string>
#include <boost/tuple/tuple.hpp>
#include <fstream>
#include <python3.10/Python.h>
#include "json.hpp"

typedef std::array<double, 4> element;
#ifndef UTIL
#define UTIL
extern std::random_device rd;
extern std::default_random_engine generator;
extern std::uniform_real_distribution<double> unidistribution;

int Mod(int a, int b);

// resets random number generator using given seed
void init_seed();

// avoids trunc error
int intpow(int numb, int p, int result = 1);

// Returns random double in uniform range [0,1[
double Random();

// returns either 0 or 1 with probabilities: param,1-param
bool Choice(double param);

// Returns a random element of SU(2)
element get_ini_rand_elem();

// returns random 1st value of link, based on boltzmann distribution on beta(inverse) & k (SU2determiant of other links in wilson loop)
double get_ao(double betak);

// Returns prod of 2 matrices in Pauli basis (I,pauli_x,pauli_y,pauli_z)
element SU2multSU2(const element &Ma, const element &Mb);

// Returns prod of SU(2) times inverse of a second SU(2) element
element SU2multSU2dag(const element &Ua, const element &Ub);

// Performs the sum of two SU(2) elements
element SU2sum(const element &Ua, const element &Ub);

// Return the trace of a single SU(2) element
double SU2Trace(const element &Ulist);

// Returns SU2determiant of given matrix in Pauli basis
double SU2det(const element &Ulink);

void check_SU2det(const element &Ulink);

// Returns a vector of given length in random 3D direction
std::vector<double> randirectionvector(double length);

// Returns the Trace of the product of the given links
double Trace(const std::vector<element> &Ulist);

// Returns the inverse of a given SU(2) element
element reversedlink(const element &link);

#endif