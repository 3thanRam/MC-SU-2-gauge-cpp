#include "../include/sitearrayclass.h"

// Either set all links to (1,0,0,0) or random SU(2) element
void site_array::init(bool ini_cond)
{
    for (int ix = 0; ix < V; ix++)
    {
        // U[ix] = std::make_unique<std::array<element, 4>>();
        for (int mu = 0; mu < 4; mu++)
        {
            if (ini_cond)
            {
                U[ix][mu] = get_ini_rand_elem();
                // U[ix]->at(mu) = get_ini_rand_elem();
            }
            else
            {
                U[ix][mu] = {1., 0., 0., 0.};
                // U[ix]->at(mu) = {1., 0., 0., 0.};
            }
        }
    }
}

// Translate multi-index to single index
int site_array::conv4to1Index(std::array<int, 4> ix)
{
    return (((ix[0] * L + ix[1]) * L + ix[2]) * L + ix[3]);
}

// Translate single index to multi-index
std::array<int, 4> site_array::conv1to4Index(int ix0)
{
    int ix = ix0;
    int t = ix / intpow(L, 3);
    ix %= intpow(L, 3);
    int x = ix / intpow(L, 2);
    ix %= L * L;
    int y = ix / L;
    ix %= L;
    int z = ix;
    return {t, x, y, z};
}