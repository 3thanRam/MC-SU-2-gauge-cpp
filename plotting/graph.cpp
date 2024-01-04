#include "graph.h"

void Saveas(json Jdata, std::string file_name)
{
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::ofstream file(path + "/" + file_name + ".json");
    file << std::setw(4) << Jdata << std::endl;
}

std::vector<std::vector<double>> Lattice_calculation(std::vector<int> N, int Ini_mode, int Imax, double Beta)
{
    std::vector<std::vector<double>> lattdata;
    for (int n : N)
    {
        Lattice lattice(n, Ini_mode, 1);
        lattice.Heatbath(Imax, Beta);
        lattdata.push_back(lattice.Avplaq_data);
    }
    return (lattdata);
}

std::vector<std::vector<double>> Lattice_calculation2(int N, int Imax, std::vector<double> Betalist)
{
    std::vector<std::vector<double>> lattdata;
    for (int beta : Betalist)
    {
        Lattice lattice(N, 0, 1);
        lattice.Heatbath(Imax, beta);
        lattdata.push_back(lattice.Avplaq_data);
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
    int Nmode = 2;
    int Np = Nmode * Nlist.size();
    Jdata["numbplots"] = Np;
    std::vector<std::string> graphinfo(Np);

    for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
    {
        Latt_data = Lattice_calculation(Nlist, Ini_mode, Iterations, Beta);
        std::cout << "Sorting datapoints" << std::endl;
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
    Saveas(Jdata, "json_data1");
}

void Graph2(int Nsize, int Iterations, std::vector<double> Betalist)
{
    json Jdata;
    std::vector<std::vector<double>> Latt_data;
    std::vector<std::vector<std::pair<int, double>>> datapts(Betalist.size());

    std::string Nsize_str = std::to_string(Nsize);
    Nsize_str.erase(Nsize_str.find_last_not_of('0') + 1, std::string::npos);
    Nsize_str.erase(Nsize_str.find_last_not_of('.') + 1, std::string::npos);

    std::vector<std::string> graphinfo(Betalist.size());

    Jdata["Xlabel"] = "Iterations";
    Jdata["Ylabel"] = "Average plaquette";
    Jdata["title"] = "Average plaquette as a function of\n Number of iterations\n for several beta & fixed lattice sizes " + Nsize_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    Jdata["numbplots"] = Betalist.size();

    Latt_data = Lattice_calculation2(Nsize, Iterations, Betalist);
    std::cout << "Sorting datapoints" << std::endl;
    for (int b = 0; b < Betalist.size(); b++)
    {
        std::stringstream ginfo;
        ginfo << "beta=" << Betalist[b];
        graphinfo[b] = ginfo.str();
        for (int i = 0; i < Iterations; i++)
        {
            datapts[b].push_back(std::make_pair(i, Latt_data[b][i]));
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data2");
}
