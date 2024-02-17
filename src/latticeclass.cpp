#include "../include/latticeclass.h"

// Get the staple at a given site and direction
element Lattice::Staple(int ix, int mu, int nu)
{
    std::array<int, 4> Uo4inds = a.conv1to4Index(ix);

    /*
     *                U2fw
     *      ix+nu    <=====   ix+nu+mu
     *      ||                  /\
     *      ||U3fw              ||U1fw
     *      \/        Uo        ||
     *      ix       =====>   ix+mu
     *      /\                  ||
     *      ||U3bc              ||U1bc
     *      ||        U2bc      \/
     *      ix-nu    <=====    ix-nu+mu
     *
     */

    // FORWARD (anti-clockwise) staple:

    // link starting at ix + mu with direction nu
    std::array<int, 4> U1forw4inds = Uo4inds;
    U1forw4inds[mu] = (U1forw4inds[mu] + 1) % L;
    int forw1ix = a.conv4to1Index(U1forw4inds);

    // inverse of link starting at ix + nu with direction mu
    std::array<int, 4> U2forw4inds = Uo4inds;
    U2forw4inds[nu] = (U2forw4inds[nu] + 1) % L;
    int forw2ix = a.conv4to1Index(U2forw4inds);

    // inverse of link starting at ix with direction nu

    // BACKWARD (clockwise) staple:

    // inverse of link starting at ix+mu-nu with direction mu
    std::array<int, 4> U1bc4inds = Uo4inds;
    U1bc4inds[mu] = (U1bc4inds[mu] + 1) % L;
    U1bc4inds[nu] = (U1bc4inds[nu] - 1 + L) % L;
    int bck1ix = a.conv4to1Index(U1bc4inds);

    // inverse of link starting at ix - nu with direction mu
    std::array<int, 4> U2bc4inds = Uo4inds;
    U2bc4inds[nu] = (U2bc4inds[nu] - 1 + L) % L;
    int bck2ix = a.conv4to1Index(U2bc4inds);

    // link starting at ix + mu with direction nu
    std::array<int, 4> U3bc4inds = Uo4inds;
    U3bc4inds[nu] = (U3bc4inds[nu] - 1 + L) % L;
    int bck3ix = a.conv4to1Index(U3bc4inds);

    element forwStaple = SU2multSU2dag(a.data[forw1ix][nu], SU2multSU2(a.data[ix][nu], a.data[forw2ix][mu]));
    element backwStaple = SU2multSU2(reversedlink(SU2multSU2(a.data[bck2ix][mu], a.data[bck1ix][nu])), a.data[bck3ix][nu]);

    return SU2sum(forwStaple, backwStaple);
}

// Generate a new link variable at a given site and direction
void Lattice::New_element(double Beta, int ix, int mu)
{

    // auto wt1 = std::chrono::high_resolution_clock::now();

    double k, vect3length;
    std::vector<double> a_3vect;

    element Usum = {0., 0., 0., 0.};
    element New_a_elem;
    for (int nu = 0; nu < 4; nu++)
    {
        if (nu == mu)
        {
            continue;
        }
        element staple = Staple(ix, mu, nu);
        Usum = SU2sum(Usum, staple);
    }
    k = sqrt(SU2det(Usum));
    // auto wt2 = std::chrono::high_resolution_clock::now();
    New_a_elem[0] = get_ao(Beta * k);
    // auto wt3 = std::chrono::high_resolution_clock::now();

    a_3vect = randirectionvector(sqrt(1. - New_a_elem[0] * New_a_elem[0]));
    New_a_elem[1] = a_3vect[0];
    New_a_elem[2] = a_3vect[1];
    New_a_elem[3] = a_3vect[2];

    for (int d = 0; d < 4; d++)
    {
        Usum[d] /= k;
    }
    // auto wt4 = std::chrono::high_resolution_clock::now();

    element NewUelem = SU2multSU2dag(New_a_elem, Usum);

    a.data[ix][mu] = NewUelem;
    // auto wt5 = std::chrono::high_resolution_clock::now();
    // double Totaltime = std::chrono::duration_cast<std::chrono::microseconds>(wt5 - wt1).count();
    // double elapsed1 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt2 - wt1).count() / Totaltime;
    // double elapsed2 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt3 - wt2).count() / Totaltime;
    // double elapsed3 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt4 - wt3).count() / Totaltime;
    // double elapsed4 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt5 - wt4).count() / Totaltime;
    // std::cout << "Totaltime " << Totaltime << "ms elapsed1 " << elapsed1 << "% elapsed2 " << elapsed2 << "% elapsed3 " << elapsed3 << "% elapsed4 " << elapsed4 << "%" << std::endl;
    // exit(1);
}

// Get the plaquette at a given site and direction
double Lattice::plaquette(int ix, int mu, int nu)
{

    /*
     *                U2
     *      ix+nu    <=====   ix+nu+mu
     *      ||                  /\
     *      ||U3                ||U1
     *      \/        Uo        ||
     *      ix       =====>   ix+mu
     *
     */

    std::array<int, 4> Uo4inds = a.conv1to4Index(ix);

    // Uo: link starting at ix with direction mu

    // U1: link starting at ix + mu with direction nu
    std::array<int, 4> U1forw4inds = Uo4inds;
    U1forw4inds[mu] = (U1forw4inds[mu] + 1) % L;
    int forw1ix = a.conv4to1Index(U1forw4inds);

    // U2: inverse of link starting at ix + nu with direction mu
    std::array<int, 4> U2forw4inds = Uo4inds;
    U2forw4inds[nu] = (U2forw4inds[nu] + 1) % L;
    int forw2ix = a.conv4to1Index(U2forw4inds);

    // U3: inverse of link starting at ix with direction nu
    // element plaqprod = SU2multSU2(a.data[ix][mu], SU2multSU2(a.data[forw1ix][nu], SU2multSU2(reversedlink(a.data[forw2ix][mu]), reversedlink(a.data[ix][nu]))));

    element plaqprod = SU2multSU2dag(SU2multSU2(a.data[ix][mu], a.data[forw1ix][nu]), SU2multSU2(a.data[ix][nu], a.data[forw2ix][mu]));
    return SU2Trace(plaqprod);
}

// Get average plaquette over the whole lattice
double Lattice::averagePlaquette()
{
    double S = 0.;
    for (int ix = 0; ix < V; ix++)
    {
        for (int mu = 0; mu < 3; mu++)
        {
            for (int nu = mu + 1; nu < 4; nu++)
            {
                S += plaquette(ix, mu, nu);
            }
        }
    }
    S /= (V * 6. * 2);
    return S;
}

// Get average action from average plaquette
double Lattice::averageAction()
{
    return 1 - averagePlaquette();
}

/** Touch a heatbath to each link variable to achieve equilibrium*/
void Lattice::heatbath(double Beta)
{
    for (int ix = 0; ix < V; ix++)
    {
        for (int mu = 0; mu < 4; mu++)
        {
            New_element(Beta, ix, mu);
        }
    }
};

/** Generate a new set of gauge group elements for each site */
void Lattice::gaugeregenerate()
{
    for (auto &elem : g)
    {
        elem = get_ini_rand_elem();
    }
}

/** The plaquete average should be unchanged under the local gauge transformation U_{ij}-> g_{i} U_{ij} g_{j}^{-1} */
void Lattice::gaugeTransformation()
{
    gaugeregenerate();
    for (int ix = 0; ix < V; ix++)
    {
        for (int mu = 0; mu < 4; mu++)
        {
            std::array<int, 4> linkedinds = a.conv1to4Index(ix);
            linkedinds[mu] = (linkedinds[mu] + 1) % L;
            int linkedsitenumb = a.conv4to1Index(linkedinds);

            a.data[ix][mu] = SU2multSU2(g[ix], SU2multSU2(a.data[ix][mu], reversedlink(g[linkedsitenumb])));
        }
    }
};

/** Repeated application of touching a heatbath to whole system achieve equilibrium*/
void Lattice::equilibrium(int Iterations, double Beta)
{
    // gaugeTransformation();
    double Avplaq = 0;
    for (int i = 0; i < Iterations; i++)
    {
        heatbath(Beta);
        Avplaq_data.push_back(averageAction());
        if (i > 20)
        {
            Avplaq += Avplaq_data.back();
        }
    }
    Avplaq /= (Iterations - 20);
    std::cout << "Average plaquette after " << Iterations << " iterations: " << Avplaq << std::endl;
};
