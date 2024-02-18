#include "latticeclass.h"

using json = nlohmann::json;
#ifndef GRAPHS
#define GRAPHS
void Saveas(json Jdata, std::string file_name);

std::vector<double> Lattice_Plaqcalculation(int lattsize, int Ini_mode, int Iterations, double Beta);
std::vector<double> Lattice_Wloopcalculation(int lattsize, int Ini_mode, int Iterations, double Beta);

// Generate data for 1st figure: Average plaquette as a function of Number of iterations for several lattice sizes & fixed beta
void Graph1(std::vector<int> Nlist, int Iterations, double Beta);
// Generate data for 2nd figure: Average plaquette as a function of Number of iterations for several beta & fixed lattice size
void Graph2(int Nsize, int Iterations, std::vector<double> Betalist);
// Generate data for 3rd figure: Wilson loop as a function of lattice size for fixed beta
void Graph3(std::vector<int> Nlist, int Iterations, double Beta);
/**Generate data for 4th figure:
 * Wilson loop as a function of beta
 * Fit of Wilson loop as a function loop sizes for different beta & fixed lattice size
 * Cutof squared times string tension as a function of beta in Strong & weak coupling limits
 * */
void Graph4(int Nsize, int Iterations, std::vector<double> Betalist);
#endif