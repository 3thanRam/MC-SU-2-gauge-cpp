#include "Util.h"
std::random_device rd;
std::default_random_engine generator(rd());
std::uniform_real_distribution<float> unidistribution(0, 1);

float Random()
{
    float R;
    for (int i = 0; i < 10; ++i)
    {
        R = unidistribution(generator);
    }
    return R;
}
int Choice(float proba1)
{
    std::discrete_distribution<int> discrdistribution{proba1, (1 - proba1)};
    // auto gen = std::bind(std::discrete_distribution<int>{1, 0.5}, std::default_random_engine(rd()));
    bool b;
    for (int i = 0; i < 10; ++i)
    {
        b = discrdistribution(generator);
    }
    return b;
}

std::vector<float> get_ini_rand_elem()
{
    std::vector<float> elem;
    elem.push_back(Random() * 2 - 1);
    return elem;
}

std::vector<float> Rotate_3Dvector_random(std::vector<float> vect)
{
    float angle = Random() * 2 * 3.14159;
    float C = cos(angle);
    float S = sin(angle);
    std::vector<float> rotvect;
    rotvect.push_back(pow(C, 2) * vect[0] - S * vect[1] + C * S * vect[2]);
    rotvect.push_back(C * S * vect[0] + C * vect[1] + pow(S, 2) * vect[2]);
    rotvect.push_back(-S * vect[0] + C * vect[2]);
    return (rotvect);
}

float get_ao(float beta, float k)
{
    int R = 1;
    float expo = exp(-2 * beta * k);
    float ao;
    float x;
    while (R == 1)
    {
        x = expo + Random() * (1 - expo);
        ao = 1 + (log(x) / (beta * k));
        R = Choice(1 - sqrt(1 - pow(ao, 2)));
    }
    return (ao);
}

std::vector<float> PauliInv(std::vector<float> a)
{
    for (int d = 1; d < 4; d++)
    {
        a[d] = -a[d];
    }
    return (a);
}

std::vector<float> Paulimult(std::vector<float> Ma, std::vector<float> Mb)
{
    std::vector<float> MProd;
    float Ma0 = Ma[0];
    float Ma1 = Ma[1];
    float Ma2 = Ma[2];
    float Ma3 = Ma[3];

    float Mb0 = Mb[0];
    float Mb1 = Mb[1];
    float Mb2 = Mb[2];
    float Mb3 = Mb[3];

    // float Mb0, Mb1, Mb2, Mb3 = Mb[0], Mb[1], Mb[2], Mb[3];

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
float PauliDet(std::vector<float> Ulink)
{
    return (pow(Ulink[0], 2) + pow(Ulink[1], 2) + pow(Ulink[2], 2) + pow(Ulink[3], 2));
}
float Trace(std::vector<float> Ulist)
{
    std::vector<float> Prod(Ulist.end() - 4, Ulist.end());
    for (int u = 4; u < Ulist.size(); u += 4)
    {
        std::vector<float> vectu(Ulist.end() - u - 4, Ulist.end() - u);
        Prod = Paulimult(vectu, Prod);
    }
    return (2 * Prod[0]);
}