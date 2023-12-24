#include "graph.h"

std::vector<std::vector<float>> Lattice_calculation(std::vector<int> N, int Ini_mode, int Imax, float Beta, int graphnumb)
{
    // Lattice lattice
    std::vector<std::vector<float>> lattdata;
    if (graphnumb == 0)
    {
        for (int n : N)
        {
            Lattice lattice(n, Ini_mode, 1);
            lattice.Heatbath(Imax, Beta);
            lattdata.push_back(lattice.Avplaq_data);
        }
    }
    return (lattdata);
}

void Graph1(std::vector<int> Nlist, int Iterations, float Beta)
{
    Gnuplot gp;
    std::string Xlabel = "Iterations";
    std::string Ylabel = "Average plaquette";
    std::string Title = "Average plaquette as a function of number of iterations for several lattice sizes & fixed beta=2.3";
    std::vector<std::vector<float>> Latt_data;
    std::vector<std::pair<int, float>> datapts;

    gp << "set xrange [0:30]\nset yrange [-1:1]\n";
    for (int Ini_mode = 0; Ini_mode < 1; Ini_mode++)
    {
        Latt_data = Lattice_calculation(Nlist, Ini_mode, Iterations, Beta, 0);
        for (int n = 0; n < Nlist.size(); n++)
        {
            for (int i = 0; i < Iterations; i++)
            {
                datapts.push_back(std::make_pair(i, Latt_data[n][i]));
            }
            gp << "plot '-' with points title 'N=4^" << Nlist[n] << "'\n";
            gp.send1d(datapts);
        }
    }
}