#include "sitearrayclass.h"

#ifndef LATTICE
#define LATTICE

struct Lattice
{
    int L;                                              // length of the lattice spacing
    bool ini_cond;                                      // initial state of array a: {1,0,0,0}(0) or random(1)
    int V;                                              // L^4  Number of sites in the lattice
    site_array a;                                       // Where the links are stored
    std::vector<element> g;                             // list of group elements associated with each site in order to perfom gauge transformations
    std::vector<double> Avplaq_data;                    // vector of Average plaquette at each iteration
    std::vector<double> Wloop_data;                     // vector of expectation value of different sized wilson loops
    std::vector<std::pair<double, double>> minmaxWloop; // vector of min and max values of wilson loops
    Lattice() : L(0){};
    Lattice(int Lattice_length_set, bool ini_cond_set)
        : L(Lattice_length_set),
          ini_cond(ini_cond_set),
          V(intpow(Lattice_length_set, 4)),
          a(Lattice_length_set),
          g(V)
    {
        a.init(ini_cond);
    }

    // Get the staple at a given site and direction
    element Staple(int ix, int mu, int nu);

    // Generate a new link variable at a given site and direction
    void New_element(double Beta, int linknumber, int mu);

    // Get the plaquette at a given site and direction
    double plaquette(int ix, int mu, int nu);

    // Get average plaquette over the whole lattice
    double averagePlaquette();

    // Get average action from average plaquette
    double averageAction();

    double Wilsonloop(int ix, int mu, int nu, int size);

    // Get Average wilson loop of given size
    double AverageWilsonloop(int size);

    // Get expectation value of different sized wilson loops
    void UpdateWloop_data(double Beta);

    /** Touch a heatbath to each link variable to achieve equilibrium*/
    void heatbath(double Beta);

    /** Generate a new set of gauge group elements for each site */
    void gaugeregenerate();

    /** The plaquete average should be unchanged under the local gauge transformation U_{ij}-> g_{i} U_{ij} g_{j}^{-1} */
    void gaugeTransformation();

    /** Repeated application of touching a heatbath to whole system achieve equilibrium*/
    void equilibrium(int Iterations, double Beta);
};
#endif