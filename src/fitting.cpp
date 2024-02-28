#include "../include/fitting.h"

// Function to perform the weighted fit and return the coefficients
std::vector<double> fitWeightedSecondOrderPolynomial(const std::vector<double> &Xdata,
                                                     const std::vector<double> &Ydata,
                                                     const std::vector<double> &weights)
{
    double chisq;
    size_t n = Xdata.size();
    std::vector<double> xVec(Xdata.begin(), Xdata.end());
    std::vector<double> yVec(Ydata.begin(), Ydata.end());
    std::vector<double> wVec(weights.begin(), weights.end());
    Data data = {n, xVec.data(), yVec.data(), wVec.data()};

    // Create gsl_vector instances
    gsl_matrix *X = gsl_matrix_alloc(n, 3);
    gsl_vector *Y = gsl_vector_alloc(n);
    gsl_vector *W = gsl_vector_alloc(n);

    // Fill the X matrix
    for (size_t i = 0; i < n; i++)
    {
        double xi = xVec[i];
        gsl_matrix_set(X, i, 0, 1.0);
        gsl_matrix_set(X, i, 1, xi);
        gsl_matrix_set(X, i, 2, xi * xi);
        gsl_vector_set(Y, i, yVec[i]);
        gsl_vector_set(W, i, wVec[i]);
    }
    //  Initialize the linear fitting workspace
    gsl_multifit_linear_workspace *workspace = gsl_multifit_linear_alloc(n, 3);

    //  Initialize the result vector
    gsl_vector *c = gsl_vector_alloc(3);

    //  Initialize the covariance matrix
    gsl_matrix *cov = gsl_matrix_alloc(3, 3);

    //  Perform the weighted linear fit
    gsl_multifit_wlinear(X, W, Y, c, cov, &chisq, workspace);


    //  Get the results
    double bestFitParams[3];
    bestFitParams[0] = gsl_vector_get(c, 0);
    bestFitParams[1] = gsl_vector_get(c, 1);
    bestFitParams[2] = gsl_vector_get(c, 2);

    //  Clean up
    gsl_multifit_linear_free(workspace);
    gsl_vector_free(c);
    gsl_vector_free(Y);
    gsl_vector_free(W);
    gsl_matrix_free(cov);
    gsl_matrix_free(X);

    return {bestFitParams[0], bestFitParams[1], bestFitParams[2]};
}