#include "../include/graphs.h"

void Saveas(json Jdata, std::string file_name)
{
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::ofstream file(path + "/" + file_name + ".json");
    file << std::setw(4) << Jdata << std::endl;
}

std::vector<double> Lattice_Plaqcalculation(int lattsize, int Ini_mode, int Iterations, double Beta)
{
    Lattice lattice(lattsize, Ini_mode);
    lattice.equilibrium(Iterations, Beta);
    return lattice.Avplaq_data;
}

void Graph1(std::vector<int> Llist, int Iterations, double Beta)
{
    json Jdata;
    std::vector<std::vector<double>> Latt_data;
    std::vector<std::vector<std::pair<int, double>>> datapts(2 * Llist.size());

    std::string Beta_str = std::to_string(Beta);
    Beta_str.erase(Beta_str.find_last_not_of('0') + 1, std::string::npos);
    Beta_str.erase(Beta_str.find_last_not_of('.') + 1, std::string::npos);
    int Nmode = 2;
    int Np = Nmode * Llist.size();
    std::vector<std::string> graphinfo(Np);

    Jdata["Xlabel"] = "Iterations";
    Jdata["Ylabel"] = "Average plaquette";
    Jdata["title"] = "Average plaquette as a function of\n Number of iterations\n for several lattice sizes & fixed $\\beta$=" + Beta_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    Jdata["numbplots"] = Np;

    for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
    {
        for (auto L : Llist)
        {
            Latt_data.emplace_back(Lattice_Plaqcalculation(L, Ini_mode, Iterations, Beta));
            printf("Finished Plaquette Calculation (L=%d, Inimode=%d,beta=%g)\n", L, Ini_mode, Beta);
        }
    }
    std::cout << "Sorting datapoints" << std::endl;
    for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
    {
        for (uint n = 0; n < Llist.size(); n++)
        {
            std::stringstream ginfo;
            ginfo << "V=" << Llist[n] << "^4 Inimode:" << Ini_mode;
            graphinfo[Ini_mode * Llist.size() + n] = ginfo.str();
            for (int i = 0; i < Iterations; i++)
            {
                // std::cout << Latt_data[Ini_mode * Llist.size() + n][i] << std::endl;
                datapts[Ini_mode * Llist.size() + n].emplace_back(i, Latt_data[Ini_mode * Llist.size() + n][i]);
            }
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data1");
}

// Generate data for 2nd figure: Average plaquette as a function of Number of iterations for several beta & fixed lattice size
void Graph2(int L, int Iterations, std::vector<double> Betalist)
{
    json Jdata;
    std::vector<std::vector<double>> Latt_data;
    std::vector<std::vector<std::pair<int, double>>> datapts(Betalist.size());

    std::string Nsize_str = std::to_string(L);
    Nsize_str.erase(Nsize_str.find_last_not_of('0') + 1, std::string::npos);
    Nsize_str.erase(Nsize_str.find_last_not_of('.') + 1, std::string::npos);

    std::vector<std::string> graphinfo(Betalist.size());

    Jdata["Xlabel"] = "Iterations";
    Jdata["Ylabel"] = "Average plaquette";
    Jdata["title"] = "Average plaquette as a function of\n Number of iterations\n for several $\\beta$ & fixed lattice size " + Nsize_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    Jdata["numbplots"] = Betalist.size();
    std::vector<std::future<std::vector<double>>> futures;

    for (double beta : Betalist)
    {
        Latt_data.emplace_back(Lattice_Plaqcalculation(L, 0, Iterations, beta));
        printf("Finished Plaquette Calculation (L=%d, Inimode=%d,beta=%g)\n", L, 0, beta);
    }

    std::cout << "Sorting datapoints" << std::endl;
    for (uint b = 0; b < Betalist.size(); b++)
    {
        std::stringstream ginfo;
        ginfo << "beta=" << Betalist[b];
        graphinfo[b] = ginfo.str();
        for (int i = 0; i < Iterations; i++)
        {
            datapts[b].emplace_back(i, Latt_data[b][i]);
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data2");
}

// Generate data for 3rd figure: Wilson loop as a function of lattice size for fixed beta
void Graph3(std::vector<int> Llist, int Iterations, double Beta)
{
}

void Graph4(int L, int Iterations, std::vector<double> Betalist)
{
}
