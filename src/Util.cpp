#include "../include/Util.h"
// std::random_device rd;
//  std::default_random_engine generator(rd());
std::default_random_engine generator(07022024); // fix seed
std::uniform_real_distribution<double> unidistribution(0, 1);

int Mod(int a, int b)
{
    const int r = a % b;
    return r < 0 ? r + b : r;
}

void init_seed()
{
    generator.seed(07022024);
}

// avoids trunc error
int intpow(int numb, int p, int result)
{
    return p == 0 ? result : intpow(numb, p - 1, result * numb);
}

// Returns random double in uniform range [0,1[
double Random()
{
    return unidistribution(generator);
}

// Returns either 0 or 1 with probabilities: param,1-param
bool Choice(double proba1)
{
    return (1. - proba1) > unidistribution(generator);
}

element get_ini_rand_elem()
{
    element U = {0., 0., 0., 0.};
    U[0] = Random() * 2. - 1.;
    std::vector<double> a_3vect = randirectionvector(sqrt(1. - U[0] * U[0]));
    std::copy(a_3vect.begin(), a_3vect.end(), U.begin() + 1);
    return U;
}

double get_ao(double betak)
{
    double expo = exp(-2. * betak);
    double ao;
    double x;
    while (true)
    {
        x = expo + Random() * (1. - expo);
        ao = 1. + (log(x) / betak);
        if (Choice(sqrt(1. - ao * ao)) == 0) // reject(1) with probability 1-sqrt(1-ao^2) <=> keep(0) with probability sqrt(1-ao^2)
        {
            break;
        }
    }
    return ao;
}

// Returns SU2determiant of given matrix in Pauli basis
double SU2det(const element &Ulink)
{
    return Ulink[0] * Ulink[0] + Ulink[1] * Ulink[1] + Ulink[2] * Ulink[2] + Ulink[3] * Ulink[3];
}

void check_SU2det(const element &Ulink)
{
    assert(abs(SU2det(Ulink) - 1.) < 1e-10);
}

element reversedlink(const element &link)
{
    element reversedlink = link;
    reversedlink[1] = -reversedlink[1];
    reversedlink[2] = -reversedlink[2];
    reversedlink[3] = -reversedlink[3];
    return reversedlink;
}

// Returns the Trace of the product of the given links
double Trace(const std::vector<element> &Ulist)
{
    element Uprod = Ulist.back();
    for (int u = Ulist.size() - 2; u >= 0; u--)
    {
        Uprod = SU2multSU2(Ulist[u], Uprod);
    }
    return 2. * Uprod[0];
}

double SU2Trace(const element &Ulist)
{
    return 2. * Ulist[0];
}
// Returns prod of 2 matrices in Pauli basis (I,pauli_x,pauli_y,pauli_z)
element SU2multSU2(const element &Ua, const element &Ub)
{
    element UProd;

    UProd[0] = Ua[0] * Ub[0] - (Ua[1] * Ub[1] + Ua[2] * Ub[2] + Ua[3] * Ub[3]);
    UProd[1] = Ua[0] * Ub[1] + Ua[1] * Ub[0] - (Ua[2] * Ub[3] - Ua[3] * Ub[2]);
    UProd[2] = Ua[0] * Ub[2] + Ua[2] * Ub[0] - (Ua[3] * Ub[1] - Ua[1] * Ub[3]);
    UProd[3] = Ua[0] * Ub[3] + Ua[3] * Ub[0] - (Ua[1] * Ub[2] - Ua[2] * Ub[1]);
    return UProd;
}

element SU2multSU2dag(const element &Ua, const element &Ub)
{
    element UProd;

    UProd[0] = Ua[0] * Ub[0] + (Ua[1] * Ub[1] + Ua[2] * Ub[2] + Ua[3] * Ub[3]);
    UProd[1] = -Ua[0] * Ub[1] + Ua[1] * Ub[0] + (Ua[2] * Ub[3] - Ua[3] * Ub[2]);
    UProd[2] = -Ua[0] * Ub[2] + Ua[2] * Ub[0] + (Ua[3] * Ub[1] - Ua[1] * Ub[3]);
    UProd[3] = -Ua[0] * Ub[3] + Ua[3] * Ub[0] + (Ua[1] * Ub[2] - Ua[2] * Ub[1]);
    return UProd;
}
element SU2sum(const element &Ua, const element &Ub)
{
    element Usum;

    Usum[0] = Ua[0] + Ub[0];
    Usum[1] = Ua[1] + Ub[1];
    Usum[2] = Ua[2] + Ub[2];
    Usum[3] = Ua[3] + Ub[3];
    return Usum;
}

// Returns a vector of given length in random 3D direction
std::vector<double> randirectionvector(double length)
{
    double theta = 2. * M_PI * Random(); // Azimuthal angle
    double cosphi = 2. * Random() - 1.;  // Polar angle
    double lsinphi = length * sqrt(1. - cosphi * cosphi);

    return {lsinphi * cos(theta), lsinphi * sin(theta), length * cosphi};
}