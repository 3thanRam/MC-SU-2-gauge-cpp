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

std::vector<double> Lattice_Wloopcalculation(int lattsize, int Ini_mode, int Iterations, double Beta)
{
    Lattice lattice(lattsize, Ini_mode);
    lattice.equilibrium(Iterations, Beta);
    lattice.UpdateWloop_data(Beta);
    return lattice.Wloop_data;
}

std::pair<std::vector<double>, std::vector<std::pair<double, double>>> Lattice_Wloop_extra(int lattsize, int Ini_mode, int Iterations, double Beta)
{
    Lattice lattice(lattsize, Ini_mode);
    lattice.equilibrium(Iterations, Beta);
    lattice.UpdateWloop_data(Beta);
    return std::make_pair(lattice.Wloop_data, lattice.minmaxWloop);
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
    int Nloops = 6;

    std::vector<std::vector<std::tuple<double, double, double>>> Latt_data(Nloops);
    std::vector<std::vector<std::tuple<int, double, double, double>>> datapts(Nloops);

    // std::vector<std::vector<std::pair<int, double>>> datapts(Nloops);

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

    std::cout << "Sorting datapoints" << std::endl;

    for (int n = 0; n < Llist.size(); n++)
    {
        // std::vector<double> Wn = Lattice_Wloopcalculation(Llist[n], 0, Iterations, Beta);
        auto [Wn, minmaxErrorData] = Lattice_Wloop_extra(Llist[n], 0, Iterations, Beta);
        for (int l = 0; l < Nloops; l++)
        {
            Latt_data[l].emplace_back(Wn[l], minmaxErrorData[l].first, minmaxErrorData[l].second);
        }
    }

    for (int l = 0; l < Nloops; l++)
    {
        std::stringstream ginfo;
        ginfo << l + 1 << "x" << l + 1;
        graphinfo[l] = ginfo.str();
        for (int n = 0; n < Llist.size(); n++)
        {
            if (l + 1 <= MaxWilsonloop(Llist[n]))
            {
                datapts[l].emplace_back(Llist[n], std::get<0>(Latt_data[l][n]), std::get<1>(Latt_data[l][n]), std::get<2>(Latt_data[l][n]));
            }
            // for (int i = 0; i < datapts[l].size(); i++)
            //{
            //     std::cout << std::get<0>(datapts[l][i]) << " " << std::get<1>(datapts[l][i]) << " " << std::get<2>(datapts[l][i]) << " " << std::get<3>(datapts[l][i]) << std::endl;
            // }
        }
    }
    Jdata["graphinfo"] = graphinfo;
    Jdata["plots"] = datapts;
    Saveas(Jdata, "json_data3");
}

void Graph4(int L, int Iterations, std::vector<double> Betalist)
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
    Jdata["fixed"] = {L, Iterations};

    Jdata["numbplots"] = Nloops;
    std::vector<std::string> graphinfo(Nloops);

    for (auto beta : Betalist)
    {
        std::vector<double> Latticeb = Lattice_Wloopcalculation(L, 0, Iterations, beta);
        for (int l = 0; l < Nloops; l++)
        {
            Latt_data[l].emplace_back(Latticeb[l]);
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

void time_complexity(double Beta)
{
    json Jdata;

    std::vector<double> times;
    const int N[5] = {2, 4, 6, 8, 10};
    for (auto n : N)
    {
        std::cout << "N = " << n << std::endl;
        auto start = std::chrono::high_resolution_clock::now();
        Lattice_Plaqcalculation(n, 0, 30, Beta);
        auto finish = std::chrono::high_resolution_clock::now();
        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
        times.push_back(elapsed);
        std::cout << "Elapsed time: " << elapsed / 1e3 << " s\n";
    }
    Jdata["Lsizes"] = N;
    Jdata["Ltimes"] = times;
    Saveas(Jdata, "json_datatime");
}