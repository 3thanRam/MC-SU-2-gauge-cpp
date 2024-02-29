#include "latticeclass.h"
#include "fitting.h"

using json = nlohmann::json;

#ifndef FIGURES
#define FIGURES

void Saveas(json Jdata, std::string file_name);

std::vector<double> Lattice_Plaqcalculation(int lattsize, int Ini_mode, int Iterations, double Beta);

std::vector<double> Lattice_Wloopcalculation(int lattsize, int Ini_mode, int Iterations, double Beta);

std::pair<std::vector<double>, std::vector<double>> Lattice_Wloop_extra(int lattsize, int Ini_mode, int Iterations, double Beta);

void savefig1(std::vector<int> Llist, int Iterations, double Beta);
void drawfig1();

void savefig2(int L, int Iterations, std::vector<double> Betalist);
void drawfig2();

void savefig3(std::vector<int> Llist, int Iterations, double Beta);
void drawfig3();

void savefig4(int L, int Iterations, std::vector<double> Betalist);
void drawfig4();

std::vector<double> gauss(std::vector<std::vector<double>> &A, std::vector<double> &B);
std::tuple<double, double, double> polyfit(std::vector<int> Xdata, std::vector<double> Ydata, int order);
std::pair<std::vector<double>,std::vector<double>> fitWilsonloop(std::vector<double> Xdata, std::vector<double> Ydata, std::vector<double> Errors);
void savefig5(int L, int Iterations, std::vector<double> Betalist);
void drawfig5();

void savefig6(int L, int Iterations, std::vector<double> Betalist);
void drawfig6();
void saveOtime(double Beta);
void drawOtime();

#endif