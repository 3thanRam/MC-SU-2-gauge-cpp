#include "Util.h"
std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_real_distribution<double> unidistribution(0, 1);
int randiter = 5; // potentially increase randomness

double Random()
{
    double R;
    for (int i = 0; i < randiter; ++i)
    {
        R = unidistribution(generator);
    }
    return R;
}
std::vector<double> Random(int n)
{
    std::vector<double> Rvect(n);
    double R;
    for (int ni = 0; ni < n; ++ni)
    {
        for (int i = 0; i < std::max(randiter / n, 1); ++i)
        {
            R = unidistribution(generator);
        }
        Rvect[ni] = R;
    }
    return Rvect;
}
int Choice(double proba1)
{
    std::discrete_distribution<int> discrdistribution{proba1, 1 - proba1};
    bool b;
    for (int i = 0; i < randiter; ++i)
    {
        b = discrdistribution(generator);
    }
    return b;
}

std::vector<double> get_ini_rand_elem()
{
    std::vector<double> elem;
    elem.reserve(4);
    elem.emplace_back(Random() * 2 - 1.);
    double vect3length = sqrt(1 - elem[0] * elem[0]);
    std::vector<double> a_3vect = Randirectionvector(vect3length);
    elem.emplace_back(a_3vect[0]);
    elem.emplace_back(a_3vect[1]);
    elem.emplace_back(a_3vect[2]);
    return elem;
}

std::vector<double> Randirectionvector(double radius)
{
    std::vector<double> vect(3);
    double rescale;
    double len2 = 2;
    auto genrandxyz = [](double a, double b)
    {
        return std::vector<double>{a + Random() * (b - a), a + Random() * (b - a), a + Random() * (b - a)};
    };
    auto length2 = [](std::vector<double> vects)
    {
        return vects[0] * vects[0] + vects[1] * vects[1] + vects[2] * vects[2];
    };

    while (len2 > 1.)
    {
        vect = genrandxyz(-1, 1);
        len2 = length2(vect);
    }
    rescale = radius / sqrt(len2);
    vect[0] *= rescale;
    vect[1] *= rescale;
    vect[2] *= rescale;
    return vect;
}

double get_ao(double beta, double k)
{
    int Reject = 1;
    double expo = exp(-2 * beta * k);
    double ao;
    double x;
    while (Reject == 1)
    {
        x = expo + Random() * (1. - expo);
        ao = 1. + (log(x) / (beta * k));
        Reject = Choice(sqrt(1. - ao * ao));
    }
    return (ao);
}

std::vector<double> Paulimult(const std::vector<double> &Ma, const std::vector<double> &Mb)
{
    std::vector<double> MProd;
    MProd.reserve(4);

    MProd.emplace_back(Ma[0] * Mb[0] - (Ma[1] * Mb[1] + Ma[2] * Mb[2] + Ma[3] * Mb[3]));
    MProd.emplace_back(Ma[0] * Mb[1] + Ma[1] * Mb[0] - (Ma[2] * Mb[3] - Ma[3] * Mb[2]));
    MProd.emplace_back(Ma[0] * Mb[2] + Ma[2] * Mb[0] - (Ma[3] * Mb[1] - Ma[1] * Mb[3]));
    MProd.emplace_back(Ma[0] * Mb[3] + Ma[3] * Mb[0] - (Ma[1] * Mb[2] - Ma[2] * Mb[1]));
    return MProd;
}
int Mod(int a, int b)
{
    const int r = a % b;
    return r < 0 ? r + b : r;
}
double PauliDet(std::vector<double> &Ulink)
{
    return Ulink[0] * Ulink[0] + Ulink[1] * Ulink[1] + Ulink[2] * Ulink[2] + Ulink[3] * Ulink[3];
}
double Trace(std::vector<double> const &Ulist)
{
    std::vector<double> Prod(Ulist.end() - 4, Ulist.end());
    for (int u = 4; u < Ulist.size(); u += 4)
    {
        std::vector<double> Vectu(Ulist.end() - u - 4, Ulist.end() - u);
        assert(abs(PauliDet(Vectu) - 1) < 1e-10);
        Prod = Paulimult(Vectu, Prod);
    }
    return 2 * Prod[0];
}

int MaxWilsonloop(int latsize)
{
    if (latsize == 6 || latsize == 4)
    {
        return latsize;
    }
    else if (latsize == 2)
    {
        return 3;
    }
    else
    {
        return 7;
    }
};