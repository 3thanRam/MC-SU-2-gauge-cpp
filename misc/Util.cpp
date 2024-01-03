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
    elem.push_back(Random() * 2 - 1);
    double avv = sqrt(1 - pow(elem[0], 2)) / sqrt(3);
    std::vector<double> av = {avv, avv, avv};
    std::vector<double> a_vect = Rotate_3Dvector_random(av);
    elem.push_back(a_vect[0]);
    elem.push_back(a_vect[1]);
    elem.push_back(a_vect[2]);
    return elem;
}

std::vector<double> Rotate_3Dvector_random(std::vector<double> vect)
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

std::vector<double> PauliInv(std::vector<double> a)
{
    for (int d = 1; d < 4; d++)
    {
        a[d] = -a[d];
    }
    return (a);
}

std::vector<double> Paulimult(std::vector<double> Ma, std::vector<double> Mb)
{
    std::vector<double> MProd;
    double Ma0 = Ma[0];
    double Ma1 = Ma[1];
    double Ma2 = Ma[2];
    double Ma3 = Ma[3];

    double Mb0 = Mb[0];
    double Mb1 = Mb[1];
    double Mb2 = Mb[2];
    double Mb3 = Mb[3];

    MProd.push_back(Ma0 * Mb0 - (Ma1 * Mb1 + Ma2 * Mb2 + Ma3 * Mb3));
    MProd.push_back((Ma0 * Mb1 + Ma1 * Mb0) - (Ma2 * Mb3 - Ma3 * Mb2));
    MProd.push_back((Ma0 * Mb2 + Ma2 * Mb0) - (Ma3 * Mb1 - Ma1 * Mb3));
    MProd.push_back((Ma0 * Mb3 + Ma3 * Mb0) - (Ma1 * Mb2 - Ma2 * Mb1));
    return (MProd);
}
int Mod(int a, int b)
{
    int ret = a % b;
    return ret >= 0 ? ret : ret + b;
}
double PauliDet(std::vector<double> Ulink)
{
    return (pow(Ulink[0], 2) + pow(Ulink[1], 2) + pow(Ulink[2], 2) + pow(Ulink[3], 2));
}
double Trace(std::vector<double> Ulist)
{
    std::vector<double> Prod(Ulist.end() - 4, Ulist.end());
    for (int u = 4; u < Ulist.size(); u += 4)
    {
        std::vector<double> vectu(Ulist.end() - u - 4, Ulist.end() - u);
        Prod = Paulimult(vectu, Prod);
    }
    return (2 * Prod[0]);
}