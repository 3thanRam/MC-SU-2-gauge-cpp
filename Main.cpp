#include "plotting/graph.h"
#include <python3.10/Python.h>
#include <chrono>
// Calls python script which plots the chosen figure using data from the json files in graph_data directory
void pythonplot(std::string whichgraph)
{
    const char *pdrawfile = "plotting/graphdraw.py";
    FILE *file = fopen(pdrawfile, "r");
    Py_Initialize();
    PyRun_SimpleFile(file, pdrawfile);
    PyRun_SimpleString(("Graphnumb=" + whichgraph).data());
    PyRun_SimpleString("DRAW(Graphnumb)");
    Py_Finalize();
}

// Returns vector filled with N values between a & b
std::vector<double> Rangevalues(double a, double b, int N)
{
    std::vector<double> X;
    X.reserve(N);
    double dx = (b - a) / N;
    for (int i = 0; i < N; i++)
    {
        X.push_back(a + i * dx);
    }
    return X;
}

int main()
{
    /**0-> No multithread
     * 1-> Perform calculations on different lattices in parrallel
     * 2-> Inside a lattice perform independent calculations in parrallel eg: action contribution from each site
     */
    int Multithreadmode = 2;

    // Choose plot 1,2,3,4 (5 & 6 are plotted with 4)
    char graphNumb;
    std::cout << "Choose which figure to plot 1,2,3,4: ";
    std::cin >> graphNumb;

    //"Plot saved(0)/ new data(1) : "
    bool Calc;
    std::cout << "Plot already saved data(0) or calculate new data & plot (1): ";
    std::cin >> Calc;

    int Iterations = 30;          // Number of iterations before asuming equilibrium
    std::vector<int> Nlist = {6}; // List of lattice sizes
    double Beta = 2.3;            // single inverse temperture
    std::vector<double> Betavect; // List of inverse tempertures beta

    if (Calc)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::cout << "Starting calculations" << std::endl;
        switch (graphNumb)
        {
        case '1':
            Graph1(Nlist, Iterations, Beta, Multithreadmode);
            break;
        case '2':
            Betavect = Rangevalues(1.2, 3.6, 6); // small list of inverse tempertures beta
            Graph2(Nlist[Nlist.size() - 1], Iterations, Betavect, Multithreadmode);
            break;
        case '3':
            Graph3(Nlist, Iterations, Beta, Multithreadmode);
            break;
        case '4':
            Betavect = Rangevalues(0.1, 4, 20); // Large vector of inverse tempertures beta between 0.1 and 4
            Graph4(Nlist[Nlist.size() - 1], Iterations, Betavect, Multithreadmode);
            break;
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << "\nTotal Elapsed time: " << (std::chrono::duration<double>(finish - start)).count() << " s\n";
    }
    std::string graphnumbstr(1, graphNumb);
    pythonplot(graphnumbstr);

    return 0;
}