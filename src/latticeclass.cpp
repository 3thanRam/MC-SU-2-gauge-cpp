#include "../include/latticeclass.h"

// Get the staple at a given site and direction
element Lattice::Staple(int ix, int mu, int nu)
{
    std::array<int, 4> ixinds = a.conv1to4Index(ix);

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
    ixinds[mu] = (ixinds[mu] + 1) % L;
    int forw1ix = a.conv4to1Index(ixinds);
    ixinds[mu] = (ixinds[mu] - 1 + L) % L;

    // inverse of link starting at ix + nu with direction mu
    ixinds[nu] = (ixinds[nu] + 1) % L;
    int forw2ix = a.conv4to1Index(ixinds);
    ixinds[nu] = (ixinds[nu] - 1 + L) % L;

    // inverse of link starting at ix with direction nu

    // BACKWARD (clockwise) staple:

    // inverse of link starting at ix+mu-nu with direction mu
    ixinds[nu] = (ixinds[nu] - 1 + L) % L;
    ixinds[mu] = (ixinds[mu] + 1) % L;
    int bck1ix = a.conv4to1Index(ixinds);
    ixinds[mu] = (ixinds[mu] - 1 + L) % L; // only undo mu, reuse nu for next link

    // inverse of link starting at ix - nu with direction mu
    int bck2ix = a.conv4to1Index(ixinds);

    // link starting at ix + mu with direction nu
    // int bck3ix = bck2ix; same site as bck2ix (but different direction)

    element forwStaple = SU2multSU2dag(a.U[forw1ix][nu], SU2multSU2(a.U[ix][nu], a.U[forw2ix][mu]));
    // element forwStaple = SU2multSU2dag(a.U[forw1ix]->at(nu), SU2multSU2(a.U[ix]->at(nu), a.U[forw2ix]->at(mu)));
    element backwStaple = SU2multSU2(reversedlink(SU2multSU2(a.U[bck2ix][mu], a.U[bck1ix][nu])), a.U[bck2ix][nu]);
    // element backwStaple = SU2multSU2(reversedlink(SU2multSU2(a.U[bck2ix]->at(mu), a.U[bck1ix]->at(nu))), a.U[bck2ix]->at(nu));

    return SU2sum(forwStaple, backwStaple);
}

// Generate a new link variable at a given site and direction
void Lattice::New_element(double Beta, int ix, int mu)
{

    double k;
    std::vector<double> a_3vect;

    // auto wt1 = std::chrono::high_resolution_clock::now();
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

    a_3vect = randirectionvector(std::sqrt(1. - New_a_elem[0] * New_a_elem[0]));
    // auto wt4 = std::chrono::high_resolution_clock::now();
    New_a_elem[1] = a_3vect[0];
    New_a_elem[2] = a_3vect[1];
    New_a_elem[3] = a_3vect[2];

    double inv_k = 1.0 / k;
    for (int d = 0; d < 4; d++)
    {
        Usum[d] *= inv_k;
    }
    // auto wt5 = std::chrono::high_resolution_clock::now();

    element NewUelem = SU2multSU2dag(New_a_elem, Usum);

    a.U[ix][mu] = NewUelem;
    // a.U[ix]->at(mu) = NewUelem;
    // auto wt6 = std::chrono::high_resolution_clock::now();
    // double Totaltime = std::chrono::duration_cast<std::chrono::microseconds>(wt6 - wt1).count();
    // double elapsed1 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt2 - wt1).count() / Totaltime;
    // double elapsed2 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt3 - wt2).count() / Totaltime;
    // double elapsed3 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt4 - wt3).count() / Totaltime;
    // double elapsed4 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt5 - wt4).count() / Totaltime;
    // double elapsed5 = 100 * std::chrono::duration_cast<std::chrono::microseconds>(wt6 - wt5).count() / Totaltime;
    // std::cout << "Total: " << elapsed1 + elapsed2 + elapsed3 + elapsed4 + elapsed5 << "%" << std::endl;
    // std::cout << "Totaltime " << Totaltime << "ms elapsed1 " << elapsed1 << "% elapsed2 " << elapsed2 << "% elapsed3 " << elapsed3 << "% elapsed4 " << elapsed4 << "% elapsed5 " << elapsed5 << "%" << std::endl;
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

    std::array<int, 4> ixinds = a.conv1to4Index(ix);

    // Uo: link starting at ix with direction mu

    // U1: link starting at ix + mu with direction nu
    ixinds[mu] = (ixinds[mu] + 1) % L;
    int forw1ix = a.conv4to1Index(ixinds);
    ixinds[mu] = (ixinds[mu] - 1 + L) % L;

    // U2: inverse of link starting at ix + nu with direction mu
    ixinds[nu] = (ixinds[nu] + 1) % L;
    int forw2ix = a.conv4to1Index(ixinds);
    ixinds[nu] = (ixinds[nu] - 1 + L) % L;

    // U3: inverse of link starting at ix with direction nu
    // same site as U1 but different direction

    // element plaqprod = SU2multSU2dag(SU2multSU2(a.U[ix]->at(mu), a.U[forw1ix]->at(nu)), SU2multSU2(a.U[ix]->at(nu), a.U[forw2ix]->at(mu)));
    element plaqprod = SU2multSU2dag(SU2multSU2(a.U[ix][mu], a.U[forw1ix][nu]), SU2multSU2(a.U[ix][nu], a.U[forw2ix][mu]));
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

double Lattice::Wilsonloop(int ix, int mu, int nu, int size)
{
    /*
     *                               U2s
     *      ix+size*nu  <===   ...  <===   ix+size*(nu+mu)
     *      ||                               /\
     *      ||                               ||
     *      \/                               ||
     *      ...                             ...
     *      ||                               /\
     *      ||U3s                            ||U1s
     *      \/          U0s                  ||
     *      ix          ===>   ...  ===>   ix+size*mu
     *
     */

    // Uo
    std::array<int, 4> ix0inds = a.conv1to4Index(ix);
    ix0inds[mu] = (ix0inds[mu] - 1 + L) % L; // start from the site before to cancel first +1
    element E0 = {1., 0., 0., 0.};
    for (int s = 0; s < size; s++)
    {
        ix0inds[mu] = (ix0inds[mu] + 1) % L;
        E0 = SU2multSU2(E0, a.U[a.conv4to1Index(ix0inds)][mu]);
    }

    // U1
    std::array<int, 4> ix1inds = a.conv1to4Index(ix);
    ix1inds[mu] = (ix1inds[mu] + size) % L;
    ix1inds[nu] = (ix1inds[nu] - 1 + L) % L; // start from the site before to cancel first +1
    element E1 = {1., 0., 0., 0.};
    for (int s = 0; s < size; s++)
    {
        ix1inds[nu] = (ix1inds[nu] + 1) % L;
        E1 = SU2multSU2(E1, a.U[a.conv4to1Index(ix1inds)][nu]);
    }

    // U2
    std::array<int, 4> ix2inds = a.conv1to4Index(ix);
    ix2inds[nu] = (ix2inds[nu] + size) % L;
    // ix2inds[mu] = (ix2inds[mu] + size) % L;
    ix2inds[mu] = (ix2inds[mu] - 1 + L) % L;
    element E2 = {1., 0., 0., 0.};
    for (int s = 0; s < size; s++)
    {
        ix2inds[mu] = (ix2inds[mu] + 1) % L;
        // E2 = SU2multSU2(E2, a.U[a.conv4to1Index(ix2inds)][mu]);
        E2 = SU2multSU2(E2, a.U[a.conv4to1Index(ix2inds)][mu]);
    }

    // U3
    std::array<int, 4> ix3inds = a.conv1to4Index(ix);
    // ix3inds[nu] = (ix3inds[nu] + size) % L;
    ix3inds[nu] = (ix3inds[nu] - 1 + L) % L;
    element E3 = {1., 0., 0., 0.};
    for (int s = 0; s < size; s++)
    {
        ix3inds[nu] = (ix3inds[nu] + 1) % L;
        // E3 = SU2multSU2(E3, a.U[a.conv4to1Index(ix3inds)][nu]);
        E3 = SU2multSU2(E3, a.U[a.conv4to1Index(ix3inds)][nu]);
    }

    element loopProd = SU2multSU2dag(SU2multSU2(E0, E1), SU2multSU2(E3, E2));
    return SU2Trace(loopProd);
}

double Lattice::AverageWilsonloop(int size)
{
    double Wl = 0.;
    for (int ix = 0; ix < V; ix++)
    {
        for (int mu = 0; mu < 3; mu++)
        {
            for (int nu = mu + 1; nu < 4; nu++)
            {
                Wl += Wilsonloop(ix, mu, nu, size);
            }
        }
    }
    Wl /= (V * 6. * 2);
    return Wl;
}

// Get expectation value of different sized wilson loops
void Lattice::UpdateWloop_data()
{
    int maxsizeloop = MaxWilsonloop(L);
    Wloop_data.reserve(maxsizeloop);
    for (int i = 1; i <= maxsizeloop; i++)
    {
        Wloop_data.push_back(AverageWilsonloop(i));
        std::cout << "Wilson loop " << i << " = " << Wloop_data[i - 1] << std::endl;
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

            a.U[ix][mu] = SU2multSU2(g[ix], SU2multSU2(a.U[ix][mu], reversedlink(g[linkedsitenumb])));
            // a.U[ix]->at(mu) = SU2multSU2(g[ix], SU2multSU2(a.U[ix]->at(mu), reversedlink(g[linkedsitenumb])));
        }
    }
};

/** Repeated application of touching a heatbath to whole system achieve equilibrium*/
void Lattice::equilibrium(int Iterations, double Beta)
{
    // gaugeTransformation();
    double Avplaq = 0;
    Avplaq_data.reserve(Iterations);
    for (int i = 0; i < Iterations; i++)
    {
        heatbath(Beta);
        Avplaq_data.push_back(averageAction());
        if (i > 20)
        {
            Avplaq += Avplaq_data[i];
        }
    }
    Avplaq /= (Iterations - 20);
    std::cout << "Average plaquette after " << Iterations << " iterations: " << Avplaq << std::endl;
};
