#include "Util.h"

#ifndef SITEARRAY_CLASS
#define SITEARRAY_CLASS

/** Where & how the links are created,set,stored,accessed,.. */
struct site_array
{
    int L;                                    // size of the lattice
    int V;                                    // number of sites in the lattice
    std::vector<std::array<element, 4>> data; // where links are stored
    site_array(){};
    site_array(int L) : L(L), V(intpow(L, 4)), data(V) {}

    /**Either set all links to (1,0,0,0) or random element (of determiant 1)*/
    void init(bool ini_cond);

    int conv4to1Index(std::array<int, 4> site4inds);
    std::array<int, 4> conv1to4Index(int ix);
};
#endif