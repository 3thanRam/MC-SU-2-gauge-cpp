#include "graph.h"

void Saveas(json Jdata, std::string file_name)
{
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::ofstream file(path + "/" + file_name + ".json");
    file << std::setw(4) << Jdata << std::endl;
}

std::vector<double> Lattice_Plaqcalculation(int N, bool inimode, int Imax, double Beta, int Multithreadmode = 0)
{
    Lattice lattice(N, inimode, Multithreadmode);
    lattice.Heatbath(Imax, Beta);
    return lattice.Avplaq_data;
}

std::vector<double> Lattice_Wloopcalculation(int N, bool inimode, int Imax, double Beta, int Multithreadmode = 0)
{
    Lattice lattice(N, inimode, Multithreadmode);
    lattice.Heatbath(Imax, Beta);
    lattice.Wloop_expct();
    return lattice.Wloop_data;
}

void Graph1(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode)
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
    Jdata["title"] = "Average plaquette as a function of\n Number of iterations\n for several lattice sizes & fixed $\\beta$=" + Beta_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    int Nmode = 2;
    int Np = Nmode * Nlist.size();
    Jdata["numbplots"] = Np;
    std::vector<std::string> graphinfo(Np);
    std::vector<std::future<std::vector<double>>> futures;

    if (Multithreadmode == 1)
    {
        for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
        {
            for (int n = 0; n < Nlist.size(); n++)
            {
                futures.push_back(std::async(Lattice_Plaqcalculation, Nlist[n], Ini_mode, Iterations, Beta, Multithreadmode));
            }
        }
        for (auto &f : futures)
        {
            Latt_data.push_back(f.get());
        }
    }
    else
    {
        for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
        {
            for (int n = 0; n < Nlist.size(); n++)
            {
                Latt_data.push_back(Lattice_Plaqcalculation(Nlist[n], Ini_mode, Iterations, Beta, Multithreadmode));
            }
        }
    }

    for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
    {
        for (int n = 0; n < Nlist.size(); n++)
        {
            std::stringstream ginfo;
            ginfo << "N=" << Nlist[n] << "^4 Inimode:" << Ini_mode;
            graphinfo[Ini_mode * Nlist.size() + n] = ginfo.str();
            for (int i = 0; i < Iterations; i++)
            {
                datapts[Ini_mode * Nlist.size() + n].push_back(std::make_pair(i, Latt_data[Ini_mode * Nlist.size() + n][i]));
            }
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data1");
}

void Graph2(int Nsize, int Iterations, std::vector<double> Betalist, int Multithreadmode)
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
    Jdata["title"] = "Average plaquette as a function of\n Number of iterations\n for several $\\beta$ & fixed lattice sizes " + Nsize_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    Jdata["numbplots"] = Betalist.size();
    std::vector<std::future<std::vector<double>>> futures;
    if (Multithreadmode == 1)
    {
        for (double beta : Betalist)
        {
            futures.push_back(std::async(Lattice_Plaqcalculation, Nsize, 0, Iterations, beta, Multithreadmode));
        }
        for (auto &f : futures)
        {
            Latt_data.push_back(f.get());
        }
    }
    else
    {
        for (double beta : Betalist)
        {
            Latt_data.push_back(Lattice_Plaqcalculation(Nsize, 0, Iterations, beta, Multithreadmode));
        }
    }

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

void Graph3(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode)
{
    int Nloops = 6;

    std::vector<std::vector<double>> Latt_data(Nloops);
    std::vector<std::vector<std::pair<int, double>>> datapts(Nloops);

    std::cout << "startloop" << std::endl;

    json Jdata;

    Jdata["Xlabel"] = "LATTICE SIZE";
    Jdata["Ylabel"] = "WILSON LOOP";
    std::string Beta_str = std::to_string(Beta);
    Beta_str.erase(Beta_str.find_last_not_of('0') + 1, std::string::npos);
    Beta_str.erase(Beta_str.find_last_not_of('.') + 1, std::string::npos);
    Jdata["title"] = "Wilson loop as a function of\n lattice size\n fixed $\\beta$=" + Beta_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;

    Jdata["numbplots"] = Nloops;
    std::vector<std::string> graphinfo(Nloops);
    std::vector<std::future<std::vector<double>>> futures;

    if (Multithreadmode == 1)
    {
        for (int n = 0; n < Nlist.size(); n++)
        {
            futures.push_back(std::async(Lattice_Wloopcalculation, Nlist[n], 0, Iterations, Beta, Multithreadmode));
        }
        for (auto &f : futures)
        {
            std::vector<double> LatticeL = f.get();
            for (int l = 0; l < LatticeL.size(); l++)
            {
                Latt_data[l].push_back(LatticeL[l]);
            }
        }
    }
    else
    {
        for (int n = 0; n < Nlist.size(); n++)
        {
            std::vector<double> LatticeL = Lattice_Wloopcalculation(Nlist[n], 0, Iterations, Beta, Multithreadmode);
            for (int l = 0; l < Nloops; l++)
            {
                Latt_data[l].push_back(LatticeL[l]);
            }
        }
    }
    auto Wmax = [](int wloopsize, int latsize)
    {
        if (latsize == 2)
        {
            return wloopsize <= 2;
        }
        else if (latsize == 4)
        {
            return wloopsize <= 3;
        }
        else if (latsize == 6)
        {
            return wloopsize <= 5;
        }
        else
        {
            return true;
        }
    };
    for (int l = 0; l < Nloops; l++)
    {
        std::stringstream ginfo;
        ginfo << l + 1 << "x" << l + 1;
        graphinfo[l] = ginfo.str();
        for (int n = 0; n < Nlist.size(); n++)
        {
            if (Wmax(l + 1, Nlist[n]))
            {
                datapts[l].push_back(std::make_pair(Nlist[n], Latt_data[l][n]));
            }
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data3");
}

void Graph4(int Nsize, int Iterations, std::vector<double> Betalist, int Multithreadmode)
{
    int Nloops = 5;

    std::vector<std::vector<double>> Latt_data(Nloops);
    std::vector<std::vector<std::pair<double, double>>> datapts(Nloops);

    std::cout << "startloop" << std::endl;

    json Jdata;

    Jdata["Xlabel"] = "$\\beta$";
    Jdata["Ylabel"] = "WILSON LOOP";
    Jdata["title"] = "Wilson loop as a function of $\\beta$";
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    Jdata["fixed"] = {Nsize, Iterations};

    Jdata["numbplots"] = Nloops;
    std::vector<std::string> graphinfo(Nloops);
    std::vector<std::future<std::vector<double>>> futures;

    if (Multithreadmode == 1)
    {
        for (auto beta : Betalist)
        {
            futures.push_back(std::async(Lattice_Wloopcalculation, Nsize, 0, Iterations, beta, Multithreadmode));
        }
        for (auto &f : futures)
        {
            std::vector<double> Latticeb = f.get();
            for (int l = 0; l < Latticeb.size(); l++)
            {
                Latt_data[l].push_back(Latticeb[l]);
            }
        }
    }
    else
    {
        for (auto beta : Betalist)
        {
            std::vector<double> Latticeb = Lattice_Wloopcalculation(Nsize, 0, Iterations, beta, Multithreadmode);
            for (int l = 0; l < Latticeb.size(); l++)
            {
                Latt_data[l].push_back(Latticeb[l]);
            }
        }
    }
    for (int l = 0; l < Nloops; l++)
    {
        std::stringstream ginfo;
        ginfo << l + 1 << "x" << l + 1;
        graphinfo[l] = ginfo.str();
        for (int b = 0; b < Betalist.size(); b++)
        {
            datapts[l].push_back(std::make_pair(Betalist[b], Latt_data[l][b]));
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data4");
}