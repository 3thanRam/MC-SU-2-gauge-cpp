#include "plotting/graph.h"
#include <python3.10/Python.h>

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

int main(int argc, char *argv[])
{
    std::vector<int> Nlist = {4};
    std::vector<double> Betalist = {1.2, 1.6, 2.0, 2.4};
    int Iterations = 30;
    int N = 4;
    double Beta = 2.3;
    int graphNumb = 2;
    if (graphNumb == 1)
    {
        Graph1(Nlist, Iterations, Beta);
    }
    else if (graphNumb == 2)
    {
        Graph2(N, Iterations, Betalist);
    }
    pythonplot(std::to_string(graphNumb));
}