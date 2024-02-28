#include <iostream>
#include <vector>
#include <tuple>
#include <gsl/gsl_multifit_nlin.h>
#include <gsl/gsl_multifit.h>
#ifndef FITTING
#define FITTING
// Define the data structure for GSL
struct Data
{
    size_t n;        // Number of data points
    const double *x; // X data
    const double *y; // Y data
    const double *w; // Weights
};

// Define the second-order polynomial function

double polynomialFitFunction(double x, const double *params);

// Define the GSL fitting function
int fitFunction(const gsl_vector *params, void *data, gsl_vector *f);

std::vector<double> fitWeightedSecondOrderPolynomial(const std::vector<double> &Xdata,
                                                     const std::vector<double> &Ydata,
                                                     const std::vector<double> &weights);
std::tuple<double, double, double> fitdata(std::vector<int> Xdata, std::vector<double> Ydata);
#endif