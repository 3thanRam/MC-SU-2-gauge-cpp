#include "../include/graphs.h"
#include "../include/tests.h"

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
    // Choose plot 1,2,3,4 (5 & 6 are plotted with 4) or run tests (T)
    char graphNumb;
    std::cout << "Choose which figure to plot 1,2,3,4,T: ";
    std::cin >> graphNumb;

    //"Plot saved(0) or  new data(1)"
    bool Calc;

    std::string cpath = std::filesystem::current_path();
    struct stat sb;                                                              // Structure which would store the metadata
    std::string datapath = cpath + "/graphdata/json_data" + graphNumb + ".json"; // Path to graphdata directory
    // struct stat sb;                                                              // Structure which would store the metadata
    if (stat((datapath).data(), &sb) != 0) // if the file doesn't exist then force calc=1
    {
        Calc = 1;
    }
    else
    {
        std::cout << "Plot already saved data(0) or calculate new data & plot (1): ";
        std::cin >> Calc;
    }

    int Iterations = 50;          // Number of iterations before asuming equilibrium
    std::vector<int> Nlist;       // List of lattice sizes
    int N;                        // single lattice size
    double Beta = 2.3;            // single inverse temperture
    std::vector<double> Betavect; // List of inverse tempertures beta

    if (Calc)
    {
        int numIntegers = 1 * (graphNumb != 'T' && graphNumb != 'd');
        if (graphNumb == '1' || graphNumb == '3')
        {
            std::cout << "Choose Number of Lattice sizes: ";
            std::cin >> numIntegers;
        }
        for (int i = 0; i < numIntegers; ++i)
        {
            std::cout << "Input Lattice size: ";
            std::cin >> N;
            Nlist.push_back(N);
        }
        std::cout << "Starting calculations" << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        switch (graphNumb)
        {
        case '1':
            Graph1(Nlist, Iterations, Beta);
            break;
        case '2':
            Betavect = Rangevalues(1.2, 3.6, 6); // small list of inverse tempertures beta
            Graph2(Nlist[0], Iterations, Betavect);
            break;
        case '3':
            Graph3(Nlist, Iterations, 3.0);
            break;
        case '4':
            Betavect = Rangevalues(0.1, 4, 20); // Large vector of inverse tempertures beta between 0.1 and 4
            Graph4(Nlist[0], Iterations, Betavect);
            break;
        case 'T':
            gauge_test(Beta);
            heatbath_test(Beta);
            break;
        default:
            Lattice_Plaqcalculation(8, 0, Iterations, Beta);
            break;
        }
        auto finish = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
        std::cout << "\nTotal Elapsed time: " << elapsed / 1e3 << " s\n";
        double Avtime = elapsed / Iterations;
        std::cout << "\n Av time per iteration: " << Avtime << " ms/iteration\n";
        // 182.4 ms/iteration for 8^4 lattice
    }
    if (graphNumb == '1' || graphNumb == '2' || graphNumb == '3' || graphNumb == '4')
    {
        std::string graphnumbstr(1, graphNumb);
        pythonplot(graphnumbstr);
    }

    return 0;
}