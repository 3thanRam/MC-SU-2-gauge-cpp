#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <random>

#ifndef UTIL
#define UTIL
std::vector<float> get_ini_rand_elem();
float Random();
int Choice(float param);
int Mod(int a, int b);
extern std::random_device rd;
extern std::default_random_engine generator;
extern std::uniform_real_distribution<float> unidistribution;
float get_ao(float beta, float k);
std::vector<float> PauliInv(std::vector<float> a);
std::vector<float> Paulimult(std::vector<float> Ma, std::vector<float> Mb);
float PauliDet(std::vector<float> Ulink);
std::vector<float> Rotate_3Dvector_random(std::vector<float> vect);
float Trace(std::vector<float> Ulist);

#endif