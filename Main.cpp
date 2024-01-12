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
int main()
{
    // 6:1min19
    // gprof -q-b Main gmon.out > analysis.txt
    std::vector<int> Nlist = {6};                        // List of lattice sizes
    std::vector<double> Betalist = {1.2, 1.6, 2.0, 2.4}; // List of inverse tempertures beta
    int Iterations = 30;                                 // Number of iterations before asuming equilibrium
    int N = Nlist[0];                                    // single lattice size
    double Beta = 2.3;                                   // single inverse temperture
    int Multithreadmode = 2;                             // no multithread(0) /parrallel threads(1) / lattice site parallelisation(2)
    int graphNumb = 1;                                   // Choose which graph to plot 1,2
    assert(graphNumb == 1 || graphNumb == 2);
    bool Calc = 1; // Plot saved data(0) or plot new data (after calcultions) (1)
    auto start = std::chrono::high_resolution_clock::now();
    if (graphNumb == 1 & Calc)
    {
        Graph1(Nlist, Iterations, Beta, Multithreadmode);
    }
    else if (graphNumb == 2 & Calc)
    {
        Graph2(N, Iterations, Betalist, Multithreadmode);
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "\nTotal Elapsed time: " << elapsed.count() << " s\n";
    pythonplot(std::to_string(graphNumb));

    return (0);
}