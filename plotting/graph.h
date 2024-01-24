#include "../classes/lattice.h"
#include "../misc/json.hpp"

using json = nlohmann::json;
#ifndef GRAPH
#define GRAPH
// Returns Average plaquette after each application of the heatbath to a lattice with given parameters
std::vector<double> Lattice_Plaqcalculation(int N, bool inimode, int Imax, double Beta, int Multithreadmode);
// Returns expectation value of different sized Wilson loops for a lattice with given parameters after reaching equilibrium (after Imax iterations)
std::vector<double> Lattice_Wloopcalculation(int N, bool inimode, int Imax, double Beta, int Multithreadmode);

// Get values from futures(threading) and place into Latt_data
void Unpackfutures(std::vector<std::vector<double>> &Latt_data, std::vector<std::future<std::vector<double>>> &futures, bool subiter);

// Save data to json file in graphdata/
void Saveas(json Jdata, std::string file_name);

// Generate data for 1st figure: Average plaquette as a function of Number of iterations for several lattice sizes & fixed beta
void Graph1(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode);
// Generate data for 2nd figure: Average plaquette as a function of Number of iterations for several beta & fixed lattice size
void Graph2(int Nsize, int Iterations, std::vector<double> Betalist, int Multithreadmode);
// Generate data for 3rd figure: Wilson loop as a function of lattice size for fixed beta
void Graph3(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode);
/**Generate data for 4th figure:
 * Wilson loop as a function of beta
 * Fit of Wilson loop as a function loop sizes for different beta & fixed lattice size
 * Cutof squared times string tension as a function of beta in Strong & weak coupling limits
 * */
void Graph4(int Nsize, int Iterations, std::vector<double> Betalist, int Multithreadmode);

#endif