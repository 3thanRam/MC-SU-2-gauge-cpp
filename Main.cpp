#include "plotting/graph.h"
#include <python3.10/Python.h>

void pythonplot(const char *pdrawfile)
{
    FILE *file = fopen(pdrawfile, "r");
    Py_Initialize();
    PyRun_SimpleFile(file, pdrawfile);
    Py_Finalize();
}

int main()
{
    std::vector<int> Nlist = {4};
    int Iterations = 30;
    double Beta = 2.3;
    Graph1(Nlist, Iterations, Beta);
    pythonplot("plotting/graphdraw.py");
}