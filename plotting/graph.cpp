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
    printf("Finished Plaquette Calculation (N=%d, Inimode=%d,beta=%g)\n", N, inimode, Beta);
    return lattice.Avplaq_data;
}

std::vector<double> Lattice_Wloopcalculation(int N, bool inimode, int Imax, double Beta, int Multithreadmode = 0)
{
    Lattice lattice(N, inimode, Multithreadmode);
    lattice.Heatbath(Imax, Beta);
    lattice.Wloop_expct();
    printf("Finished Wilson loop Calculation (N=%d,beta=%g)\n", N, Beta);
    return lattice.Wloop_data;
}

/**
 * Unpacks the results from a vector of futures into the provided Latt_data vector.
 */
void Unpackfutures(std::vector<std::vector<double>> &Latt_data, std::vector<std::future<std::vector<double>>> &futures, bool iter = 0)
{
    for (auto &f : futures)
    {
        if (!iter)
        {
            Latt_data.emplace_back(f.get());
        }
        else
        {
            std::vector<double> sublatticedata = f.get();
            for (int i = 0; i < sublatticedata.size(); i++)
            {
                Latt_data[i].emplace_back(sublatticedata[i]);
            }
        }
    }
}

/**
 * Generates and saves data for a graph representing average plaquette values.
 */
void Graph1(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode)
{

    std::vector<std::vector<double>> Latt_data;
    std::vector<std::vector<std::pair<int, double>>> datapts(2 * Nlist.size());

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
                futures.emplace_back(std::async(Lattice_Plaqcalculation, Nlist[n], Ini_mode, Iterations, Beta, Multithreadmode));
            }
        }
        Unpackfutures(Latt_data, futures);
    }
    else
    {
        for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
        {
            for (int n = 0; n < Nlist.size(); n++)
            {
                Latt_data.emplace_back(Lattice_Plaqcalculation(Nlist[n], Ini_mode, Iterations, Beta, Multithreadmode));
            }
        }
    }
    std::cout << "Sorting datapoints" << std::endl;
    for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
    {
        for (int n = 0; n < Nlist.size(); n++)
        {
            std::stringstream ginfo;
            ginfo << "N=" << Nlist[n] << "^4 Inimode:" << Ini_mode;
            graphinfo[Ini_mode * Nlist.size() + n] = ginfo.str();
            for (int i = 0; i < Iterations; i++)
            {
                datapts[Ini_mode * Nlist.size() + n].emplace_back(i, Latt_data[Ini_mode * Nlist.size() + n][i]);
            }
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data1");
}

/**
 * Generates a graph and saves it as a JSON file.
 */
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
            futures.emplace_back(std::async(Lattice_Plaqcalculation, Nsize, 0, Iterations, beta, Multithreadmode));
        }
        Unpackfutures(Latt_data, futures);
    }
    else
    {
        for (double beta : Betalist)
        {
            Latt_data.emplace_back(Lattice_Plaqcalculation(Nsize, 0, Iterations, beta, Multithreadmode));
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
            datapts[b].emplace_back(i, Latt_data[b][i]);
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data2");
}

/**
 * Calculate and save graph data for Wilson loop as a function of lattice size.
 */
void Graph3(std::vector<int> Nlist, int Iterations, double Beta, int Multithreadmode)
{
    int Nloops = 6;

    std::vector<std::vector<double>> Latt_data(Nloops);
    std::vector<std::vector<std::pair<int, double>>> datapts(Nloops);

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

    std::cout << "Sorting datapoints" << std::endl;
    if (Multithreadmode == 1)
    {
        for (int n = 0; n < Nlist.size(); n++)
        {
            futures.emplace_back(std::async(Lattice_Wloopcalculation, Nlist[n], 0, Iterations, Beta, Multithreadmode));
        }
        Unpackfutures(Latt_data, futures, 1);
    }
    else
    {
        for (int n = 0; n < Nlist.size(); n++)
        {
            std::vector<double> LatticeL = Lattice_Wloopcalculation(Nlist[n], 0, Iterations, Beta, Multithreadmode);
            for (int l = 0; l < Nloops; l++)
            {
                Latt_data[l].emplace_back(LatticeL[l]);
            }
        }
    }

    for (int l = 0; l < Nloops; l++)
    {
        std::stringstream ginfo;
        ginfo << l + 1 << "x" << l + 1;
        graphinfo[l] = ginfo.str();
        for (int n = 0; n < Nlist.size(); n++)
        {
            if (l + 1 < MaxWilsonloop(Nlist[n]))
            {
                datapts[l].emplace_back(Nlist[n], Latt_data[l][n]);
            }
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data3");
}

/**
 * Generates data for a Wilson loop as a function of beta and saves it in a JSON file.
 */
void Graph4(int Nsize, int Iterations, std::vector<double> Betalist, int Multithreadmode)
{
    int Nloops = 5;

    std::vector<std::vector<double>> Latt_data(Nloops);
    std::vector<std::vector<std::pair<double, double>>> datapts(Nloops);

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
            futures.emplace_back(std::async(Lattice_Wloopcalculation, Nsize, 0, Iterations, beta, Multithreadmode));
        }
        Unpackfutures(Latt_data, futures, 1);
    }
    else
    {
        for (auto beta : Betalist)
        {
            std::vector<double> Latticeb = Lattice_Wloopcalculation(Nsize, 0, Iterations, beta, Multithreadmode);
            for (int l = 0; l < Latticeb.size(); l++)
            {
                Latt_data[l].emplace_back(Latticeb[l]);
            }
        }
    }
    std::cout << "Sorting datapoints" << std::endl;
    for (int l = 0; l < Nloops; l++)
    {
        std::stringstream ginfo;
        ginfo << l + 1 << "x" << l + 1;
        graphinfo[l] = ginfo.str();
        for (int b = 0; b < Betalist.size(); b++)
        {
            datapts[l].emplace_back(Betalist[b], Latt_data[l][b]);
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data4");
}