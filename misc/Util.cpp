#include "Util.h"
std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_real_distribution<double> unidistribution(0, 1);
int randiter = 10;

double Random()
{
    double R;
    for (int i = 0; i < randiter; ++i)
    {
        R = unidistribution(generator);
    }
    return R;
}
int Choice(double proba1)
{
    std::discrete_distribution<int> discrdistribution{proba1, 1 - proba1};
    // auto gen = std::bind(std::discrete_distribution<int>{1, 0.5}, std::default_random_engine(rd()));
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
    double avv = sqrt(1 - pow(elem[0], 2)) / sqrt(3);
    std::vector<double> av = {avv, avv, avv};
    std::vector<double> a_vect = Rotate_3Dvector_random(av);
    elem.push_back(a_vect[0]);
    elem.push_back(a_vect[1]);
    elem.push_back(a_vect[2]);
    return elem;
}

std::vector<double> Rotate_3Dvector_random(std::vector<double> &vect)
{
    std::vector<double> rotvect;
    double alpha, beta, gamma, a, b, c, d, e, f, g, h, i;

    alpha = Random() * 2 * 3.14159;
    beta = Random() * 2 * 3.14159;
    gamma = Random() * 2 * 3.14159;

    a = cos(alpha) * cos(beta);
    b = cos(alpha) * sin(beta) * sin(gamma) - sin(alpha) * cos(gamma);
    c = cos(alpha) * sin(beta) * cos(gamma) + sin(alpha) * sin(gamma);

    d = sin(alpha) * cos(beta);
    e = sin(alpha) * sin(beta) * sin(gamma) + cos(alpha) * cos(gamma);
    f = sin(alpha) * sin(beta) * cos(gamma) - cos(alpha) * sin(gamma);

    g = -sin(beta);
    h = cos(beta) * sin(gamma);
    i = cos(beta) * cos(gamma);

    rotvect.push_back(a * vect[0] + b * vect[1] + c * vect[2]);
    rotvect.push_back(d * vect[0] + e * vect[1] + f * vect[2]);
    rotvect.push_back(g * vect[0] + h * vect[2] + i * vect[2]);
    return (rotvect);
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
        Reject = Choice(sqrt(1 - pow(ao, 2)));
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
    return pow(Ulink[0], 2) + pow(Ulink[1], 2) + pow(Ulink[2], 2) + pow(Ulink[3], 2);
}
double Trace(std::vector<double> const &Ulist)
{
    std::vector<double> Prod(Ulist.end() - 4, Ulist.end());
    for (int u = 4; u < Ulist.size(); u += 4)
    {
        std::vector<double> vectu(Ulist.end() - u - 4, Ulist.end() - u);
        Prod = Paulimult(vectu, Prod);
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