#include <vector>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <functional>
#include <random>
#include <thread>
#include <future>

#ifndef UTIL
#define UTIL
std::vector<double> get_ini_rand_elem();
double Random();
int Choice(double param);
/** needeed non standard modulo operation */
int Mod(int a, int b);
extern std::random_device rd;
extern std::default_random_engine generator;
extern std::uniform_real_distribution<double> unidistribution;
double get_ao(double beta, double k);
std::vector<double> PauliInv(std::vector<double> a);
std::vector<double> Paulimult(const std::vector<double> &Ma, const std::vector<double> &Mb);
double PauliDet(std::vector<double> &Ulink);
std::vector<double> Rotate_3Dvector_random(std::vector<double> &vect);
double Trace(std::vector<double> const &Ulist);
int MaxWilsonloop(int latsize);

template <size_t N>
using size = std::integral_constant<size_t, N>;

template <typename T, size_t N>
class counter : std::array<T, N>
{
    using A = std::array<T, N>;
    A b, e;

    template <size_t I = 0>
    void inc(size<I> = size<I>())
    {
        if (++_<I>() != std::get<I>(e))
            return;

        _<I>() = std::get<I>(b);
        inc(size<I + 1>());
    }

    void inc(size<N - 1>) { ++_<N - 1>(); }

public:
    counter(const A &b, const A &e) : A(b), b(b), e(e) {}

    counter &operator++() { return inc(), *this; }

    operator bool() const { return _<N - 1>() != std::get<N - 1>(e); }

    template <size_t I>
    T &_() { return std::get<I>(*this); }

    template <size_t I>
    constexpr const T &_() const { return std::get<I>(*this); }
};

#endif