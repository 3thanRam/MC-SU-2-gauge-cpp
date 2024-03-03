#include "../include/figures.h"

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

std::pair<std::vector<double>, std::vector<double>> Lattice_Wloop_extra(int lattsize, int Ini_mode, int Iterations, double Beta)
{
    Lattice lattice(lattsize, Ini_mode);
    lattice.equilibrium(Iterations, Beta);
    lattice.UpdateWloop_data(Beta);
    return std::make_pair(lattice.Wloop_data, lattice.minmaxWloop);
}

void savefig1(std::vector<int> Llist, int Iterations, double Beta)
{
    json Jdata;
    std::vector<std::vector<double>> Latt_data;

    std::string Beta_str = std::to_string(Beta);
    Beta_str.erase(Beta_str.find_last_not_of('0') + 1, std::string::npos);
    Beta_str.erase(Beta_str.find_last_not_of('.') + 1, std::string::npos);
    int Nmode = 2;
    int Np = Nmode * Llist.size();
    std::vector<std::string> graphinfo(Np);

    Jdata["Xlabel"] = "Iterations";
    Jdata["Ylabel"] = "Average plaquette";
    Jdata["title"] = "Average plaquette as a function of \\n Number of iterations for \\n several lattice sizes and fixed β=" + Beta_str; //$\\beta$=" + Beta_str;
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
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::ofstream datFile(path + "/fig1.dat");

    std::vector<std::pair<int, int>> params(Np);
    for (int Ini_mode = 0; Ini_mode < Nmode; Ini_mode++)
    {
        for (uint n = 0; n < Llist.size(); n++)
        {
            std::stringstream ginfo;
            params[Ini_mode * Llist.size() + n] = std::make_pair(Ini_mode, Llist[n]);
            ginfo << "V=" << Llist[n] << "^4 Inimode:" << Ini_mode;
            graphinfo[Ini_mode * Llist.size() + n] = ginfo.str();
            datFile << "# " << graphinfo[Ini_mode * Llist.size() + n] << "\n";
            for (int i = 0; i < Iterations; i++)
            {
                datFile << i << " " << Latt_data[Ini_mode * Llist.size() + n][i] << "\n";
            }
            if (Ini_mode * Llist.size() + n < Np - 1) // if not last block
            {
                datFile << "\n\n"; // Separate blocks with an empty line
            }
        }
    }
    datFile.close();
    Jdata["params"] = params;
    Jdata["graphinfo"] = graphinfo;
    std::cout << "Saving data" << std::endl;
    Saveas(Jdata, "json_data1");
}
void drawfig1()
{
    const char *colors[] = {"#FF0000", "#00FF00", "#0000FF", "#FF00FF", "#00FFFF", "#FFFF00"};
    const char *dashtype[] = {"", "-", ".", "_", "dotdotdash", "longdash"};
    std::cout << "Drawing figure 1" << std::endl;
    std::string path = std::filesystem::current_path();
    std::string cpath = path + "/graphdata/json_data1.json";

    std::ifstream f(cpath);
    json data = json::parse(f);
    FILE *g;
    g = popen("gnuplot -p", "w");
    fprintf(g, "set title \"%s\"\n", data["title"].get<std::string>().c_str());
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel"].get<std::string>().c_str());
    fprintf(g, "set ylabel \"%s\"\n", data["Ylabel"].get<std::string>().c_str());

    int plotcount = 0;
    // Jdata["Inimodes"] = Inimodes;
    // for (const auto &info : data["graphinfo"])
    for (int i = 0; i < data["numbplots"]; i++)
    {
        std::string figtitle = data["graphinfo"][i].get<std::string>();
        // std::string figtitle = info.get<std::string>();
        auto params = data["params"][i];
        int inicond = params[0];
        int L = params[1];
        // char inicondchar = figtitle[14];
        // int inicond = inicondchar - '0';
        std::cout << figtitle << " : " << plotcount << " ; " << inicond << " ; " << colors[L / 2] << " ; " << dashtype[inicond] << std::endl;
        if (plotcount == 0)
        {
            fprintf(g, "plot 'graphdata/fig1.dat' index %d using 1:2 lt rgb  \"%s\" dashtype \"%s\" with lines title \"%s\"", plotcount, colors[L / 2], dashtype[inicond], figtitle.c_str());
        }
        else
        {
            fprintf(g, ", '' index %d using 1:2 lt rgb  \"%s\" dashtype \"%s\" with lines title \"%s\"", plotcount, colors[L / 2], dashtype[inicond], figtitle.c_str());
        }
        plotcount++;
    }
    fprintf(g, "\n");
    fflush(g);
    fprintf(g, "set term png\n");
    fprintf(g, "set output \"graphdata/fig1.png\"\n");
    fprintf(g, "replot\n");
    fprintf(g, "set term x11\n");
    pclose(g);
}

void savefig2(int L, int Iterations, std::vector<double> Betalist)
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
    Jdata["title"] = "Average plaquette as a function of\\n Number of iterations\\n for several β & fixed lattice size " + Nsize_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    Jdata["numbplots"] = Betalist.size();

    for (double beta : Betalist)
    {
        Latt_data.emplace_back(Lattice_Plaqcalculation(L, 0, Iterations, beta));
        printf("Finished Plaquette Calculation (L=%d, Inimode=%d,beta=%g)\n", L, 0, beta);
    }

    std::cout << "Sorting datapoints" << std::endl;
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::ofstream datFile(path + "/fig2.dat");
    for (uint b = 0; b < Betalist.size(); b++)
    {

        std::stringstream ginfo;
        ginfo << "β=" << Betalist[b];
        graphinfo[b] = ginfo.str();
        datFile << "# " << graphinfo[b] << "\n";
        for (int i = 0; i < Iterations; i++)
        {
            datFile << i << " " << Latt_data[b][i] << "\n";
        }
        if (b < Betalist.size() - 1) // if not last block
        {
            datFile << "\n\n"; // Separate blocks with an empty line
        }
    }
    datFile.close();
    Jdata["graphinfo"] = graphinfo;
    std::cout << "Saving data" << std::endl;
    Saveas(Jdata, "json_data2");
}
void drawfig2()
{
    const char *colors[] = {"#FF0000", "#00FF00", "#0000FF", "#FF00FF", "#00FFFF", "#FFFF00"};
    const char *dashtype[] = {"", "-", ".", "_", "dotdotdash", "longdash"};
    std::cout << "Drawing figure 2" << std::endl;
    std::string path = std::filesystem::current_path();
    std::string cpath = path + "/graphdata/json_data2.json";

    std::ifstream f(cpath);
    json data = json::parse(f);
    FILE *g;
    g = popen("gnuplot -p", "w");
    fprintf(g, "set title \"%s\"\n", data["title"].get<std::string>().c_str());
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel"].get<std::string>().c_str());
    fprintf(g, "set ylabel \"%s\"\n", data["Ylabel"].get<std::string>().c_str());

    int plotcount = 0;
    for (const auto &info : data["graphinfo"])
    {
        std::string figtitle = info.get<std::string>();
        char inicondchar = figtitle[14];
        int inicond = 0;
        if (plotcount == 0)
        {
            fprintf(g, "plot 'graphdata/fig2.dat' index %d using 1:2 lt rgb  \"%s\" dashtype \"%s\" with lines title \"%s\"", plotcount, colors[plotcount], dashtype[inicond], figtitle.c_str());
        }
        else
        {
            fprintf(g, ", '' index %d using 1:2 lt rgb  \"%s\" dashtype \"%s\" with lines title \"%s\"", plotcount, colors[plotcount], dashtype[inicond], figtitle.c_str());
        }
        plotcount++;
    }
    fprintf(g, "\n");
    fflush(g);
    fprintf(g, "set term png\n");
    fprintf(g, "set output \"graphdata/fig2.png\"\n");
    fprintf(g, "replot\n");
    fprintf(g, "set term x11\n");
    pclose(g);
}

void savefig3(std::vector<int> Llist, int Iterations, double Beta)
{
    json Jdata;
    int Nloops = 6;
    std::vector<std::vector<std::pair<double, double>>> Latt_data(Nloops);

    Jdata["Xlabel"] = "LATTICE SIZE";
    Jdata["Ylabel"] = "WILSON LOOP";
    std::string Beta_str = std::to_string(Beta);
    Beta_str.erase(Beta_str.find_last_not_of('0') + 1, std::string::npos);
    Beta_str.erase(Beta_str.find_last_not_of('.') + 1, std::string::npos);
    Jdata["title"] = "Wilson loop as a function of\\n lattice size\\n fixed β=" + Beta_str;
    Jdata["b"] = 0;
    Jdata["t"] = 1;

    Jdata["numbplots"] = Nloops;
    std::vector<std::string> graphinfo(Nloops);

    std::cout << "Sorting datapoints" << std::endl;
    for (int n = 0; n < Llist.size(); n++)
    {
        // std::vector<double> Wn = Lattice_Wloopcalculation(Llist[n], 0, Iterations, Beta);
        auto [Wn, stdDeviation] = Lattice_Wloop_extra(Llist[n], 0, Iterations, Beta);
        for (int l = 0; l < Nloops; l++)
        {
            Latt_data[l].emplace_back(Wn[l], stdDeviation[l]);
        }
    }
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::ofstream datFile(path + "/fig3.dat");
    for (int l = 0; l < Nloops; l++)
    {
        std::stringstream ginfo;
        ginfo << l + 1 << "x" << l + 1;
        graphinfo[l] = ginfo.str();
        datFile << "# " << graphinfo[l] << "\n";
        for (int n = 0; n < Llist.size(); n++)
        {
            if (l + 1 <= MaxWilsonloop(Llist[n]))
            {
                datFile << Llist[n] << " " << std::get<0>(Latt_data[l][n]) << " " << std::get<1>(Latt_data[l][n]) << "\n";
                // datapts[l].emplace_back(Llist[n], std::get<0>(Latt_data[l][n]), std::get<1>(Latt_data[l][n]));
            }
        }
        if (l < Nloops - 1) // if not last block
        {
            datFile << "\n\n"; // Separate blocks with an empty line
        }
    }
    datFile.close();
    Jdata["graphinfo"] = graphinfo;
    std::cout << "Saving data" << std::endl;
    Saveas(Jdata, "json_data3");
}
void drawfig3()
{
    const char *colors[] = {"#FF0000", "#00FF00", "#0000FF", "#FF00FF", "#00FFFF", "#FFFF00"};
    const char *dashtype[] = {"", "-", ".", "_", "dotdotdash", "longdash"};
    std::cout << "Drawing figure 3" << std::endl;
    std::string path = std::filesystem::current_path();
    std::string cpath = path + "/graphdata/json_data3.json";

    std::ifstream f(cpath);
    json data = json::parse(f);
    FILE *g;
    g = popen("gnuplot -p", "w");
    fprintf(g, "set title \"%s\"\n", data["title"].get<std::string>().c_str());
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel"].get<std::string>().c_str());
    fprintf(g, "set ylabel \"%s\"\n", data["Ylabel"].get<std::string>().c_str());

    // int plotcount = 0;
    // for (const auto &info : data["graphinfo"])
    //{
    //     std::string figtitle = info.get<std::string>();
    //     int inicond = 0;
    //     if (plotcount == 0)
    //     {
    //         fprintf(g, "plot 'graphdata/fig3.dat' index %d 1:2:3 with yerrorbars lt rgb  \"%s\"  title \"%s\"", plotcount, colors[plotcount], figtitle.c_str());
    //     }
    //     else
    //     {
    //         fprintf(g, ", '' index %d 1:2:3 with yerrorbars lt rgb  \"%s\" title \"%s\"", plotcount, colors[plotcount], figtitle.c_str());
    //     }
    //     plotcount++;
    // }
    fprintf(g, "set xrange [0:12]\n");
    fprintf(g, "set yrange [0.01:1]\n");
    fprintf(g, "set logscale y\n");

    fprintf(g, "plot 'graphdata/fig3.dat' index 0 using 1:2:3 with yerrorlines lt rgb  \"#FF0000\" title \"1x1\",");
    fprintf(g, "'' index 1 using 1:2:3 with yerrorlines lt rgb  \"#00FF00\" title \"2x2\",");
    fprintf(g, "'' index 2 using 1:2:3 with yerrorlines lt rgb  \"#0000FF\" title \"3x3\",");
    fprintf(g, "'' index 3 using 1:2:3 with yerrorlines lt rgb  \"#FF00FF\" title \"4x4\",");
    fprintf(g, "'' index 4 using 1:2:3 with yerrorlines lt rgb  \"#00FFFF\" title \"5x5\",");
    fprintf(g, "'' index 5 using 1:2:3 with yerrorlines lt rgb  \"#FFFF00\" title \"6x6\"");
    fprintf(g, "\n");
    fflush(g);
    fprintf(g, "set term png\n");
    fprintf(g, "set output \"graphdata/fig3.png\"\n");
    fprintf(g, "replot\n");
    fprintf(g, "set term x11\n");
    pclose(g);
}

void savefig4(int L, int Iterations, std::vector<double> Betalist)
{
    int Nloops = 5;

    std::vector<std::vector<double>> Latt_data(Nloops);

    json Jdata;

    Jdata["Xlabel"] = "β";
    Jdata["Ylabel"] = "WILSON LOOP";
    Jdata["title"] = "Wilson loop as a function of β \\n for fixed lattice size L=" + std::to_string(L);
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
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    std::ofstream datFile(path + "/fig4.dat");

    for (int l = 0; l < Nloops; l++)
    {
        std::stringstream ginfo;
        ginfo << l + 1 << "x" << l + 1;
        graphinfo[l] = ginfo.str();
        datFile << "# " << graphinfo[l] << "\n";
        for (int b = 0; b < Betalist.size(); b++)
        {
            datFile << Betalist[b] << " " << Latt_data[l][b] << "\n";
        }
        if (l < Nloops - 1) // if not last block
        {
            datFile << "\n\n"; // Separate blocks with an empty line
        }
    }
    datFile.close();
    Jdata["graphinfo"] = graphinfo;
    std::cout << "Saving data" << std::endl;
    Saveas(Jdata, "json_data4");
}
void drawfig4()
{
    const char *colors[] = {"#FF0000", "#00FF00", "#0000FF", "#FF00FF", "#00FFFF", "#FFFF00"};
    const char *dashtype[] = {"", "-", ".", "_", "dotdotdash", "longdash"};
    std::cout << "Drawing figure 4" << std::endl;
    std::string path = std::filesystem::current_path();
    std::string cpath = path + "/graphdata/json_data4.json";

    std::ifstream f(cpath);
    json data = json::parse(f);
    FILE *g;
    g = popen("gnuplot -p", "w");
    // fprintf(g, "set term x11 enhanced\n");
    fprintf(g, "set title \"%s\"\n", data["title"].get<std::string>().c_str());
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel"].get<std::string>().c_str());
    fprintf(g, "set ylabel \"%s\"\n", data["Ylabel"].get<std::string>().c_str());

    fprintf(g, "set xrange [0:4.5]\n");
    fprintf(g, "set yrange [0:1]\n");
    int plotcount = 0;
    for (const auto &info : data["graphinfo"])
    {
        std::string figtitle = info.get<std::string>();
        int inicond = 0;
        if (plotcount == 0)
        {
            fprintf(g, "plot 'graphdata/fig4.dat' index %d using 1:2 lt rgb  \"%s\" title \"%s\"", plotcount, colors[plotcount], figtitle.c_str());
        }
        else
        {
            fprintf(g, ", '' index %d using 1:2 lt rgb  \"%s\" title \"%s\"", plotcount, colors[plotcount], figtitle.c_str());
        }
        plotcount++;
    }
    fprintf(g, ", [0:2] x/4 lt rgb  \"%s\" title \"%s\"", colors[0], "β/4");
    fprintf(g, ", [2:4] 1-3/(4*x) lt rgb  \"%s\" title \"%s\"", colors[0], "1-3*β/4");

    fprintf(g, ",[0:2] (x/4)**4 lt rgb  \"%s\" title \"%s\"", colors[1], "(β/4)^4");
    fprintf(g, "\n");
    fflush(g);
    fprintf(g, "set term png  \n");
    fprintf(g, "set output \"graphdata/fig4.png\"\n");
    fprintf(g, "replot\n");
    fprintf(g, "set term x11 \n");
    pclose(g);
}

// fit -log(Y) = A + B * X + C * X^2
std::pair<std::vector<double>, std::vector<double>> fitWilsonloop(std::vector<double> Xdata, std::vector<double> Ydata, std::vector<double> Errors)
{
    std::vector<double> logYdata, logErrors;
    // double A, B, C;
    // for (auto y : Ydata)
    for (int i = 0; i < Ydata.size(); i++)
    {
        logYdata.push_back(-log(Ydata[i]));
        logErrors.push_back(-log(Errors[i]));
    }

    return fitWeightedSecondOrderPolynomial(Xdata, logYdata, logErrors);
}
void savefig5(int L, int Iterations, std::vector<double> Betalist)
{
    int Nloops = 5;
    double betafitvals[] = {1.9, 2.1, 2.3, 2.5};
    int Nfitvals = 4;

    std::vector<std::vector<double>> Loopsides(Nfitvals);
    Loopsides[0] = {0, 1, 2};
    Loopsides[1] = {0, 1, 2};
    Loopsides[2] = {1, 2, 3, 4};
    Loopsides[3] = {1, 2, 3, 4};

    std::vector<std::vector<double>> Latt_data(Nloops);
    std::vector<std::vector<double>> Latt_sqdata(Nloops);
    std::vector<std::vector<double>> StdDev(Nloops);
    for (int l = 0; l < Nloops; l++)
    {
        for (int b = 0; b < Betalist.size(); b++)
        {
            Latt_data[l].emplace_back(0.0);
            Latt_sqdata[l].emplace_back(0.0);
        }
    }

    // lambda function to fit wilson loop
    auto expofitfct = [](double s, double A, double B, double C)
    {
        double expo = exp(-(A + B * s + C * s * s));
        return;
    };

    json Jdata;

    Jdata["Xlabel"] = "LOOP SIDE";
    Jdata["Ylabel"] = "WILSON LOOP";
    Jdata["title"] = "Fit of Wilson loop as a function of the loop side \\n from dependence on β \\n for fixed lattice size L=" + std::to_string(L);
    Jdata["b"] = 0;
    Jdata["t"] = 1;
    Jdata["fixed"] = {L, Iterations};

    Jdata["numbplots"] = Nloops;
    std::vector<std::string> graphinfo(Nloops);

    int Nav = 3;
    for (int b = 0; b < Betalist.size(); b++)
    {
        for (int n = 0; n < Nav; n++)
        {
            std::vector<double> Latticeb = Lattice_Wloopcalculation(L, 0, Iterations, Betalist[b]);
            for (int l = 0; l < Nloops; l++)
            {
                Latt_data[l][b] += Latticeb[l] / Nav;
                Latt_sqdata[l][b] += Latticeb[l] * Latticeb[l] / Nav;
            }
        }
    }

    for (int b = 0; b < Betalist.size(); b++)
    {
        for (int l = 0; l < Nloops; l++)
        {
            StdDev[l].push_back(sqrt((Latt_sqdata[l][b] - (Latt_data[l][b] * Latt_data[l][b])) / Nav));
        }
    }

    std::cout << "Sorting datapoints" << std::endl;
    std::vector<std::vector<double>> fitcoefs(Nfitvals);
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";

    std::ofstream datFile(path + "/fig5.dat");
    for (int b = 0; b < Nfitvals; b++)
    {
        std::stringstream ginfo;
        ginfo << "beta=" << betafitvals[b];
        graphinfo[b] = ginfo.str();
        datFile << "# " << graphinfo[b] << "\n";

        std::vector<double> Loopsidefitvals;
        std::vector<double> Latticefitvals;
        std::vector<double> stdDeviation;
        int bi = nearestElemInd(betafitvals[b], Betalist);

        if (betafitvals[b] <= 2.1)
        {
            Loopsidefitvals = {0, 1, 2};
            Latticefitvals = {1.0, Latt_data[0][bi], Latt_data[1][bi]};

            stdDeviation.emplace_back(0);
            datFile << Loopsidefitvals[0] << " " << Latticefitvals[0] << " " << 0 << "\n";

            stdDeviation.emplace_back(StdDev[0][bi]);
            datFile << Loopsidefitvals[1] << " " << Latticefitvals[1] << " " << StdDev[0][bi] << "\n";

            stdDeviation.emplace_back(StdDev[1][bi]);
            datFile << Loopsidefitvals[2] << " " << Latticefitvals[2] << " " << StdDev[1][bi] << "\n";
        }
        else
        {
            Loopsidefitvals = Loopsides[b];
            for (int ls = 0; ls < Loopsidefitvals.size(); ls++)
            {
                stdDeviation.emplace_back(StdDev[ls][bi]);
                Latticefitvals.emplace_back(Latt_data[ls][bi]);
                datFile << Loopsidefitvals[ls] << " " << Latticefitvals.back() << " " << stdDeviation.back() << "\n";
            }
        }
        std::vector<double> abccoefs;
        // fit W=exp(-( a+ b*S + c*S^2))<-> -log(W) = A+ B*S + C*S^2
        if (betafitvals[b] <= 2.1)
        {
            //-log(W=0)=A=0
            //-log(W=1)=B+C  -log(W=2)=2B+4C
            double C = log(Latticefitvals[1]) - 0.5 * log(Latticefitvals[2]);
            double B = -log(Latticefitvals[1]) - C;
            abccoefs = {0, B, C};
        }
        else
        {
            auto result = fitWilsonloop(Loopsidefitvals, Latticefitvals, stdDeviation);
            abccoefs = result.first;
        }
        fitcoefs[b] = abccoefs;
        if (b < Nfitvals - 1) // if not last block
        {
            datFile << "\n\n"; // Separate blocks with an empty line
        }
    }
    datFile.close();
    Jdata["fitcoefs"] = fitcoefs;
    Jdata["graphinfo"] = graphinfo;
    Saveas(Jdata, "json_data5");
}
void drawfig5()
{
    const char *colors[] = {"#FF0000", "#00FF00", "#0000FF", "#FF00FF", "#00FFFF", "#FFFF00"};
    const char *dashtype[] = {"", "-", ".", "_", "dotdotdash", "longdash"};
    std::cout << "Drawing figure 5" << std::endl;
    std::string path = std::filesystem::current_path();
    std::string cpath = path + "/graphdata/json_data5.json";

    std::ifstream f(cpath);
    json data = json::parse(f);
    FILE *g;
    g = popen("gnuplot -p", "w");
    fprintf(g, "set title \"%s\"\n", data["title"].get<std::string>().c_str());
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel"].get<std::string>().c_str());
    fprintf(g, "set ylabel \"%s\"\n", data["Ylabel"].get<std::string>().c_str());

    fprintf(g, "set xrange [0:6]\n");
    fprintf(g, "set yrange [0.01:1]\n");
    fprintf(g, "set logscale y\n");

    fprintf(g, "plot 'graphdata/fig5.dat' index 0 using 1:2:3 with yerrorbars lt rgb  \"#FF0000\" title \"β=1.9\",");
    fprintf(g, "'' index 1 using 1:2:3 with yerrorbars lt rgb  \"#00FF00\" title \"β=2.1\",");
    fprintf(g, "'' index 2 using 1:2:3 with yerrorbars lt rgb  \"#0000FF\" title \"β=2.3\",");
    fprintf(g, "'' index 3 using 1:2:3 with yerrorbars lt rgb  \"#FF00FF\" title \"β=2.5\",");

    std::vector<std::tuple<double, double, double>> fitcoefs = data["fitcoefs"];
    for (int b = 0; b < fitcoefs.size(); b++)
    {
        auto [A, B, C] = fitcoefs[b];
        if (b < fitcoefs.size() - 1)
        {
            fprintf(g, " [0:5] exp(-(%.3f+%.3f*x+%.3f*x**2)) lt rgb  \"%s\" title \"fit β=%.1f\",", A, B, C, colors[b], 1.9 + 0.2 * b);
        }
        else
        {
            fprintf(g, " [0:5] exp(-(%.3f+%.3f*x+%.3f*x**2)) lt rgb  \"%s\" title \"fit β=%.1f\"", A, B, C, colors[b], 1.9 + 0.2 * b);
        }
    }

    fprintf(g, "\n");
    fflush(g);
    fprintf(g, "set term png  \n");
    fprintf(g, "set output \"graphdata/fig5.png\"\n");
    fprintf(g, "replot\n");
    fprintf(g, "set term x11 \n");
    pclose(g);
}

void savefig6(int L, int Iterations, std::vector<double> Betalist)
{
    int Nloops = 5;

    json Jdata;

    Jdata["Xlabel"] = "β";
    Jdata["Ylabel"] = "a^2k";
    Jdata["title"] = "Cutof squared times string tension \\n as a function of β in \\n Strong & weak coupling limits \\n for fixed lattice size L=" + std::to_string(L);
    Jdata["b"] = 0;
    Jdata["t"] = 10;
    Jdata["fixed"] = {L, Iterations};

    Jdata["numbplots"] = Nloops;
    std::vector<std::string> graphinfo(1);

    std::cout << "Sorting datapoints" << std::endl;
    std::vector<std::vector<double>> fitcoefs(Betalist.size());
    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";

    std::ofstream datFile(path + "/fig6.dat");
    std::stringstream ginfo;
    ginfo << "fitdata";
    graphinfo[0] = ginfo.str();
    datFile << "# " << graphinfo[0] << "\n";
    for (int b = 0; b < Betalist.size(); b++)
    {
        int Nav = 5;
        if (Betalist[b] >= 2.1)
        {
            Nav = 10;
        }

        std::vector<double> Latt_data(Nloops);
        std::vector<double> Latt_sqdata(Nloops);
        std::vector<double> StdDev(Nloops);
        std::cout << "beta: " << Betalist[b] << std::endl;
        for (int n = 0; n < Nav; n++)
        {
            std::vector<double> Latticeb = Lattice_Wloopcalculation(L, 0, Iterations, Betalist[b]);
            for (int l = 0; l < Nloops; l++)
            {
                Latt_data[l] += Latticeb[l] / Nav;
                Latt_sqdata[l] += Latticeb[l] * Latticeb[l] / Nav;
            }
        }
        for (int l = 0; l < Nloops; l++)
        {
            StdDev[l] = sqrt((Latt_sqdata[l] - (Latt_data[l] * Latt_data[l])) / Nav);
        }
        std::vector<double> Loopsidefitvals;
        std::vector<double> Latticefitvals;
        std::vector<double> stdDeviation;

        if (Betalist[b] <= 1.6)
        {
            Loopsidefitvals = {0, 1};
            Latticefitvals = {1.0, Latt_data[0]};

            stdDeviation.push_back(0);
            stdDeviation.push_back(StdDev[0]);
        }
        else if (Betalist[b] <= 2.1)
        {
            Loopsidefitvals = {0, 1, 2};
            Latticefitvals = {1.0, Latt_data[0], Latt_data[1]};

            stdDeviation.push_back(0);
            stdDeviation.push_back(StdDev[0]);
            stdDeviation.push_back(StdDev[1]);
        }
        else
        {
            Loopsidefitvals = {1, 2, 3, 4};
            // for (auto ls : Loopsidefitvals)
            for (int ls = 0; ls < Loopsidefitvals.size(); ls++)
            {
                stdDeviation.push_back(StdDev[ls]);
                Latticefitvals.push_back(Latt_data[ls]);
            }
        }
        // std::vector<double> Errors;
        // for (int l = 0; l < Loopsidefitvals.size(); l++)
        //{
        //     Errors.push_back(1.0);
        // }
        std::vector<double> abccoefs;
        std::vector<double> coefErrors = {0, 0, 0};
        // fit W=exp(-( a+ b*S + c*S^2))
        if (Betalist[b] <= 1.6)
        {
            abccoefs = {0, 0, -log(Latticefitvals[1])}; // C term dominates

            double dCdW = -1.0 / Latticefitvals[1]; // Partial derivative of C with respect to W
            coefErrors[2] = abs(dCdW) * StdDev[1];
        }
        else if (Betalist[b] <= 2.1)
        {
            //-log(W=0)=A=0
            //-log(W=1)=B+C  -log(W=2)=2B+4C
            double C = log(Latticefitvals[1]) - 0.5 * log(Latticefitvals[2]);
            double B = -log(Latticefitvals[1]) - C;
            abccoefs = {0, B, C};
            // Estimate errors based on error propagation
            // double dCdB = 0.5 / Latticefitvals[1];
            double dCdC = -0.5 / Latticefitvals[2];

            // double errorA = sqrt(dCdB * dCdB * StdDev[0] * StdDev[0] + dCdC * dCdC * StdDev[1] * StdDev[1]);
            // double errorB = sqrt(dCdB * dCdB * StdDev[0] * StdDev[0]);
            coefErrors[2] = sqrt(dCdC * dCdC * StdDev[1] * StdDev[1]); // assuming that the errors on Latticefitvals[1] and Latticefitvals[2] are independent
        }
        else
        {

            auto result = fitWilsonloop(Loopsidefitvals, Latticefitvals, stdDeviation);
            abccoefs = result.first;
            coefErrors = result.second;
        }
        std::cout << "abccoefs: " << abccoefs[0] << " " << abccoefs[1] << " " << abccoefs[2] << " " << std::endl;
        std::cout << "coefErrors: " << coefErrors[0] << " " << coefErrors[1] << " " << coefErrors[2] << " " << std::endl;

        fitcoefs[b] = abccoefs;
        datFile << Betalist[b] << " " << abccoefs[2] << " " << coefErrors[2] << "\n";
    }
    datFile.close();
    Jdata["graphinfo"] = graphinfo;
    Saveas(Jdata, "json_data6");
}
void drawfig6()
{
    const char *colors[] = {"#FF0000", "#00FF00", "#0000FF", "#FF00FF", "#00FFFF", "#FFFF00"};
    const char *dashtype[] = {"", "-", ".", "_", "dotdotdash", "longdash"};
    std::cout << "Drawing figure 6" << std::endl;
    std::string path = std::filesystem::current_path();
    std::string cpath = path + "/graphdata/json_data6.json";

    std::ifstream f(cpath);
    json data = json::parse(f);
    FILE *g;
    g = popen("gnuplot -p", "w");
    fprintf(g, "set title \"%s\"\n", data["title"].get<std::string>().c_str());
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel"].get<std::string>().c_str());
    fprintf(g, "set ylabel \"%s\"\n", data["Ylabel"].get<std::string>().c_str());

    fprintf(g, "set xrange [0:3.5]\n");
    fprintf(g, "set yrange [0.01:10]\n");
    fprintf(g, "set logscale y\n");

    fprintf(g, "plot 'graphdata/fig6.dat' using 1:2:3 with yerrorbars lt rgb  \"#FF0000\" title \"fit data\"");

    fprintf(g, ", [0:3.5] -log(x/4) lt rgb  \"%s\" title \"%s\"", colors[0], "Strong coupling lim: -ln(β/4)");
    fprintf(g, ", [1.5:2.9] exp((-6*pi**2)*(x-2)/11) lt rgb  \"%s\" title \"%s\"", colors[1], "Weak coupling lim: exp((-6*pi**2)*(β-2)/11)");

    fprintf(g, "\n");
    fflush(g);
    fprintf(g, "set term png  \n");
    fprintf(g, "set output \"graphdata/fig6.png\"\n");
    fprintf(g, "replot\n");
    fprintf(g, "set term x11 \n");
    pclose(g);
}

void saveOtime(double Beta)
{
    json Jdata;

    const int N[5] = {2, 4, 6, 8, 10};
    std::vector<double> betalist = {1.7,1.9, 2.1, 2.3, 2.5, 2.7};
    Jdata["Xlabel"] = "LATTICE SIZE";
    Jdata["Xlabel2"] = "β";
    Jdata["Ylabel"] = "TIME (ms)";

    std::string Beta_str = std::to_string(Beta);
    Beta_str.erase(Beta_str.find_last_not_of('0') + 1, std::string::npos);
    Beta_str.erase(Beta_str.find_last_not_of('.') + 1, std::string::npos);
    Jdata["title"] = "Time to perform 50 iterations \\n for different lattice sizes and β=" + Beta_str;
    Jdata["title2"] = "Time to perform 50 iterations \\n for different β sizes and lattice size L=6";

    std::string cpath = std::filesystem::current_path();
    std::string path = cpath + "/graphdata";
    double maxi = 0;
     double maxi2 = 0;
    std::ofstream datFile(path + "/Otime.dat");
    int Navg = 5;
    for (auto n : N)
    {
        //std::cout << "N = " << n << std::endl;
        double elapsed = 0;
        for (int i = 0; i < Navg; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            Lattice_Plaqcalculation(n, 0, 30, Beta);
            auto finish = std::chrono::high_resolution_clock::now();
            elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
        }
        elapsed /= Navg;
        datFile << n << " " << elapsed << "\n";
        if (elapsed > maxi)
        {
            maxi = elapsed;
        }
        //std::cout << "Elapsed time: " << elapsed / 1e3 << " s\n";
    }
    datFile << "\n\n";
    for (auto b : betalist)
    {
        double elapsed = 0;
        for (int i = 0; i < Navg; i++)
        {
            auto start = std::chrono::high_resolution_clock::now();
            Lattice_Plaqcalculation(6, 0, 30, b);
            auto finish = std::chrono::high_resolution_clock::now();
            elapsed += std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count();
        }
        elapsed /= Navg;
        datFile << b << " " << elapsed << "\n";
        if (elapsed > maxi2)
        {
            maxi2 = elapsed;
        }
        //std::cout << "Elapsed time: " << elapsed / 1e3 << " s\n";
    }
    datFile.close();
    Jdata["Lsizes"] = N;
    Jdata["max"] = maxi;
    Jdata["max2"] = maxi2;
    Saveas(Jdata, "json_datatime");
}
void drawOtime()
{
    const char *colors[] = {"#FF0000", "#00FF00", "#0000FF", "#FF00FF", "#00FFFF", "#FFFF00"};
    const char *dashtype[] = {"", "-", ".", "_", "dotdotdash", "longdash"};
    std::cout << "Drawing Otime" << std::endl;
    std::string path = std::filesystem::current_path();
    std::string cpath = path + "/graphdata/json_datatime.json";

    std::ifstream f(cpath);
    json data = json::parse(f);
    FILE *g;
    g = popen("gnuplot -p", "w");
    fprintf(g, "set term png  \n");
    fprintf(g, "set output \"graphdata/Otimefig.png\"\n");
    fprintf(g, "set xrange [1:12]\n");
    //std::cout << "max: " << data["max"].get<double>() << std::endl;
    fprintf(g, "set yrange [1: \"%f\"]\n", data["max"].get<double>() * 1.1);
    fprintf(g, "set logscale y\n");
    fprintf(g, "set logscale x\n");
    fprintf(g, "set title \"%s\"\n", data["title"].get<std::string>().c_str());

    fprintf(g, "set multiplot layout 1,2 columns\n");
    
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel"].get<std::string>().c_str());
    fprintf(g, "set ylabel \"%s\"\n", data["Ylabel"].get<std::string>().c_str());
    fprintf(g, "plot 'graphdata/Otime.dat' index 0  using 1:2 lt 1 lc rgb 'blue' with lines title 'data points'");
    fprintf(g, ", [1:12] x**2 lc rgb  'yellow' title 'O(L**2)'");
    fprintf(g, ", [1:12] x**3 lc rgb  'red' title 'O(L**3)'");
    fprintf(g, ", [1:12] x**4 lc rgb  'green' title 'O(L**4)'");
    fprintf(g, ", [1:12] x**5 lc rgb  'purple' title 'O(L**5)'\n");
    fprintf(g, "unset xlabel \n");
    fprintf(g, "unset logscale y\n");
    fprintf(g, "unset logscale x\n");

    fprintf(g, "set autoscale x\n");
    fprintf(g, "set autoscale y\n");
    //std::cout << "max: " << data["max"].get<double>() << std::endl;
    //fprintf(g, "set yrange [0: \"%f\"]\n", data["max2"].get<double>() * 1.1);
    fprintf(g, "set xlabel \"%s\"\n", data["Xlabel2"].get<std::string>().c_str());
    fprintf(g, "set title \"%s\"\n", data["title2"].get<std::string>().c_str());
    fprintf(g, "plot 'graphdata/Otime.dat' index 1  using 1:2 lt 1 lc rgb 'blue' title 'data points'\n");
    fprintf(g, "unset multiplot \n");
    fprintf(g, "set term x11 \n");
    pclose(g);
}