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


std::pair<std::vector<double>,std::vector<double>> fitWeightedSecondOrderPolynomial(const std::vector<double> &Xdata,
                                                     const std::vector<double> &Ydata,
                                                     const std::vector<double> &weights);
#endif