#include "Util.h"

#ifndef SITEARRAY_CLASS
#define SITEARRAY_CLASS

/** Where & how the links are created,set,stored,accessed,.. */
struct site_array
{
    int L; // size of the lattice
    int V; // number of sites in the lattice
    // std::vector<std::unique_ptr<std::array<element, 4>>> U; //  where links are stored
    std::vector<std::array<element, 4>> U; //  where links are stored
    site_array(){};
    site_array(int L) : L(L), V(intpow(L, 4)), U(V) {}

    // Either set all links to (1,0,0,0) or random element (of determiant 1)
    void init(bool ini_cond);

    // Translate multi-index to single index
    int conv4to1Index(std::array<int, 4> site4inds);

    // Translate single index to multi-index
    std::array<int, 4> conv1to4Index(int ix);
};
#endif