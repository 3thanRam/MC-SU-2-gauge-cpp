#include "sitearrayclass.h"

#ifndef LATTICE
#define LATTICE

struct Lattice
{
    int L;                           // length of the lattice spacing
    bool ini_cond;                   // initial state of array a: {1,0,0,0}(0) or random(1)
    int V;                           // L^4  Number of sites in the lattice
    site_array a;                    // Where the links are stored
    std::vector<element> g;          // list of group elements associated with each site in order to perfom gauge transformations
    std::vector<double> Avplaq_data; // vector of Average plaquette at each iteration
    std::vector<double> Wloop_data;  // vector of expectation value of different sized wilson loops
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
    element Staple(int ix, int mu, int nu);
    void heatbath(double Beta);
    void equilibrium(int Iterations, double Beta);
    void New_element(double Beta, int linknumber, int mu);
    double plaquette(int ix, int mu, int nu);
    double averagePlaquette();
    double averageAction();
    void gaugeregenerate();
    void gaugeTransformation();
};
#endif