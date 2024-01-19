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
// Returns random double in uniform range [0,1[
double Random();
// returns either 0 or 1 with probailities: param,1-param
int Choice(double param);
// needeed non standard modulo operation
int Mod(int a, int b);
extern std::random_device rd;
extern std::default_random_engine generator;
extern std::uniform_real_distribution<double> unidistribution;
/** returns random 1st value of link
 * based on boltzmann distribution on beta(inverse) & k (determiant of other links in wilson loop)*/
double get_ao(double beta, double k);
// Returns prod of 2 matrices in Pauli basis (I,pauli_x,pauli_y,pauli_z)
std::vector<double> Paulimult(const std::vector<double> &Ma, const std::vector<double> &Mb);
// Returns determiant of given matrix in Pauli basis
double PauliDet(std::vector<double> &Ulink);
// Rotates a given 3D vector in a random direction
std::vector<double> Rotate_3Dvector_random(std::vector<double> &vect);
/** Returns the Trace of the product of the given links
 * Input is list of all link elements
 * To be multiplied by grouping each link (which is of size 4)
 */
double Trace(std::vector<double> const &Ulist);
// Returns the maximum size of a Wilson loop for a given lattice size
int MaxWilsonloop(int latsize);

template <size_t N>
using size = std::integral_constant<size_t, N>;
/** Custom multidimensional counter to help readability
 * Thanks to iavr at https://stackoverflow.com/a/22424361
 */
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