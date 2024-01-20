#include "Util.h"
std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_real_distribution<double> unidistribution(0, 1);
int randiter = 1; // potentially increase randomness

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
    elem.push_back(Random() * 2 - 1);
    double vect3length = sqrt(1 - elem[0] * elem[0]);
    std::vector<double> a_3vect = {vect3length, 0, 0};
    Rotate_3Dvector_random(a_3vect);
    elem.push_back(a_3vect[0]);
    elem.push_back(a_3vect[1]);
    elem.push_back(a_3vect[2]);
    return elem;
}

void Rotate_3Dvector_random(std::vector<double> &vect)
{
    std::vector<double> Rands(3);
    double alpha, cos_alpha, sin_alpha, beta, cos_beta, sin_beta, gamma, cos_gamma, sin_gamma, a, b, c, d, e, f, g, h, i, vectx, vecty, vectz;

    Rands = Random(3);
    alpha = Rands[0] * 2 * 3.14159;
    beta = Rands[1] * 2 * 3.14159;
    gamma = Rands[2] * 2 * 3.14159;

    cos_alpha = cos(alpha);
    sin_alpha = sin(alpha);

    cos_beta = cos(beta);
    sin_beta = sin(beta);

    cos_gamma = cos(gamma);
    sin_gamma = sin(gamma);

    a = cos_alpha * cos_beta;
    b = cos_alpha * sin_beta * sin_gamma - sin_alpha * cos_gamma;
    c = cos_alpha * sin_beta * cos_gamma + sin_alpha * sin_gamma;

    d = sin_alpha * cos_beta;
    e = sin_alpha * sin_beta * sin_gamma + cos_alpha * cos_gamma;
    f = sin_alpha * sin_beta * cos_gamma - cos_alpha * sin_gamma;

    g = -sin_beta;
    h = cos_beta * sin_gamma;
    i = cos_beta * cos_gamma;

    vectx = a * vect[0] + b * vect[1] + c * vect[2];
    vecty = d * vect[0] + e * vect[1] + f * vect[2];
    vectz = g * vect[0] + h * vect[2] + i * vect[2];
    vect[0] = vectx;
    vect[1] = vecty;
    vect[2] = vectz;
}

double get_ao(double beta, double k)
{
    int Reject = 1;
    double expo = exp(-2 * beta * k);
    double ao;
    double x;
    while (Reject == 1)
    {
        x = expo + Random() * (1 - expo);
        ao = 1 + (log(x) / (beta * k));
        Reject = Choice(sqrt(1 - ao * ao));
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