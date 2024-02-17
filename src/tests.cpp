#include "../include/tests.h"

void seed_check()
{
    std::cout << "Seed check: ";
    for (int i = 0; i < 5; i++)
    {
        std::cout << Random();
    }
    std::cout << std::endl;
}

/** Repeated application of touching a heatbath to each link variable to achieve equilibrium*/
void gauge_test(double Beta)
{
    std::string inicondstr[2] = {"Cold Start", "Hot Start"};
    std::string testfail[2] = {"Failed\n", "Passed\n"};
    std::cout << std::endl;
    bool S = 1;
    for (int inicond = 0; inicond < 2; inicond++)
    {
        Lattice lattice(6, inicond);
        lattice.a.init(inicond);
        double P0 = lattice.averagePlaquette();
        lattice.gaugeTransformation();
        double Pg = lattice.averagePlaquette();
        S = S && (abs(P0 - Pg) < 1e-10);
        std::cout << inicondstr[inicond] << " gauge_test:  P0:" << P0 << " P0g:" << Pg << std::endl;
    }
    std::cout << "gauge_test " << testfail[S] << std::endl;
};

/** Repeated application of touching a heatbath to each link variable to achieve equilibrium*/
void heatbath_test(double Beta)
{
    std::string inicondstr[2] = {"\nCold Start", "\nHot Start"};
    std::string testfail[2] = {"Failed\n", "Passed\n"};
    bool S = 1;
    int L = 6;
    for (int inicond = 0; inicond < 2; inicond++)
    {
        Lattice lattice(L, inicond);
        for (int i = 1; i < 2; i++)
        {
            init_seed();
            lattice.a.init(inicond);
            init_seed();
            // seed_check();
            for (int i2 = 0; i2 < i; i2++)
            {
                lattice.heatbath(Beta);
            }
            double P0 = lattice.averagePlaquette();
            init_seed();
            lattice.a.init(inicond);
            lattice.gaugeTransformation();
            init_seed();
            // seed_check();
            for (int i2 = 0; i2 < i; i2++)
            {
                lattice.heatbath(Beta);
            }
            double Pg = lattice.averagePlaquette();
            std::cout << inicondstr[inicond] << " heatbath:  P0:" << P0 << " P0g:" << Pg << std::endl;
            S = S && (abs(P0 - Pg) < 1e-10);
        }
    }
    std::cout << "heatbath_test " << testfail[S] << std::endl;
};
