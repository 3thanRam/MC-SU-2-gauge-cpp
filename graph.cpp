#include "graph.h"

std::vector<std::vector<double>> Lattice_calculation(std::vector<int> N, int Ini_mode, int Imax, double Beta, int graphnumb)
{
    // Lattice lattice
    std::vector<std::vector<double>> lattdata;
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

void Graph1(std::vector<int> Nlist, int Iterations, double Beta)
{

    std::vector<std::vector<double>> Latt_data;
    std::vector<std::vector<std::pair<int, double>>> datapts(2 * Nlist.size());

    std::cout << "startloop" << std::endl;

    json Jdata;

    Jdata["Xlabel"] = "Iterations";
    Jdata["Ylabel"] = "Average plaquette";
    std::string Beta_str = std::to_string(Beta);
    Beta_str.erase(Beta_str.find_last_not_of('0') + 1, std::string::npos);
    Beta_str.erase(Beta_str.find_last_not_of('.') + 1, std::string::npos);
    Jdata["title"] = "Average plaquette as a function of\n Number of iterations\n for several lattice sizes & fixed beta=" + Beta_str;
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