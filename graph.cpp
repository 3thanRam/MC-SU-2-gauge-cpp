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

    std::vector<std::vector<float>> Latt_data;
    std::vector<std::vector<std::pair<int, float>>> datapts(2 * Nlist.size());

    std::cout << "startloop" << std::endl;

    json Jdata;

    Jdata["Xlabel"] = "Iterations";
    Jdata["Ylabel"] = "Average plaquette";
    Jdata["title"] = "Average plaquette as a function of number of iterations for several lattice sizes & fixed beta=2.3";
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    int Nmode = 1;
    int Np = Nmode * Nlist.size();
    Jdata["numbplots"] = Np;
    std::vector<std::string> graphinfo(Np);

    for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
    {
        Latt_data = Lattice_calculation(Nlist, Ini_mode, Iterations, Beta, 0);
        std::cout << "Drawing" << std::endl;
        for (int n = 0; n < Nlist.size(); n++)
        {
            std::stringstream ginfo;
            ginfo << "N=4^" << Nlist[n] << " Inimode:" << Ini_mode;
            graphinfo[Ini_mode * Nlist.size() + n] = ginfo.str();
            for (int i = 0; i < Iterations; i++)
            {
                datapts[Ini_mode * Nlist.size() + n].push_back(std::make_pair(i, Latt_data[n][i]));
            }
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::string file_name = "json_data";
    std::ofstream file(path + "/" + file_name + ".json");
    file << std::setw(4) << Jdata << std::endl;
}