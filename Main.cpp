#include "plotting/graph.h"
#include <python3.10/Python.h>
#include <chrono>

void pythonplot(std::string whichgraph)
{
    const char *pdrawfile = "plotting/graphdraw.py";
    std::string fullpathstr = "fullpath=path.removesuffix('/plotting')+'/graphdata/json_data" + whichgraph + ".json'";

    FILE *file = fopen(pdrawfile, "r");
    Py_Initialize();
    PyRun_SimpleFile(file, pdrawfile);
    PyRun_SimpleString((fullpathstr).data());
    PyRun_SimpleString("DRAW(fullpath)");
    Py_Finalize();
}

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
    // gprof -q-b Main gmon.out > analysis.txt
    std::vector<int> Nlist = {6};                             // List of lattice sizes
    std::vector<double> Betalist = {1.2, 1.6, 2.0, 2.4};      // List of inverse tempertures beta
    std::vector<double> Beta_array = Rangevalues(0.1, 4, 10); // List of inverse tempertures beta
    int Iterations = 30;                                      // Number of iterations before asuming equilibrium
    int N = Nlist[0];                                         // single lattice size
    double Beta = 2.3;                                        // single inverse temperture
    int Multithreadmode = 2;                                  // no multithread(0) /parrallel threads(1) / lattice site parallelisation(2)
    int graphNumb = 4;                                        // Choose which graph to plot 1,2
    assert(graphNumb == 1 || graphNumb == 2 || graphNumb == 3 || graphNumb == 4);
    bool Calc = 1; // Plot already saved data(0) or plot new data (after calcultions) (1)
    auto start = std::chrono::high_resolution_clock::now();
    if (graphNumb == 1 & Calc)
    {
        Graph1(Nlist, Iterations, Beta, Multithreadmode);
    }
    else if (graphNumb == 2 & Calc)
    {
        Graph2(N, Iterations, Betalist, Multithreadmode);
    }
    else if (graphNumb == 3 & Calc)
    {
        Graph3(Nlist, Iterations, Beta, Multithreadmode);
    }
    else if (graphNumb == 4 & Calc)
    {
        Graph4(N, Iterations, Beta_array, Multithreadmode);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "\nTotal Elapsed time: " << elapsed.count() << " s\n";
    pythonplot(std::to_string(graphNumb));

    return 0;
}