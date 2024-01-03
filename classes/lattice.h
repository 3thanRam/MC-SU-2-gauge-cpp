#include "../classes/a_class.h"

#ifndef LATTICE
#define LATTICE

//  The lattice

struct Lattice
{
    // a_array(int Lsize) : Lsize(Lsize), data(4, std::vector<double>(pow(Lsize, 4) * 16)) {}
    int Lattice_length;
    bool ini_cond;
    int Numb_cpu;
    int totnumb_orientlinks;
    int totnumb_elements;
    int Nplaq;
    a_array a;
    std::vector<double> Avplaq_data;
    std::vector<double> Wloop_data;
    Lattice(){};
    Lattice(int Lat_Length_set, bool ini_cond_set, int Numb_cpu_set) : Lattice_length(Lat_Length_set), ini_cond(ini_cond_set), Numb_cpu(Numb_cpu_set), a(Lat_Length_set), totnumb_orientlinks(4 * pow(Lat_Length_set, 4)), totnumb_elements(4 * totnumb_orientlinks), Nplaq(18 * totnumb_orientlinks)
    {
        a.init(ini_cond);
    }

    std::vector<double> Neighprod(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int revd2)
    {
        std::vector<double> Ulinks = Neighbours(ind1, ind2, ind3, ind4, d1, d2, 1, pow(-1, revd2));
        std::vector<double> U3(Ulinks.end() - 4, Ulinks.end());
        std::vector<double> U2(Ulinks.end() - 8, Ulinks.end() - 4);
        std::vector<double> U1(Ulinks.end() - 12, Ulinks.end() - 8);

        std::vector<double> Uprod = Paulimult(U1, Paulimult(U2, U3));
        return (Uprod);
    }
    std::vector<double> Neighbours(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int signd1, int signd2, int Wsize = 1)
    {
        std::vector<double> Ulist;
        std::vector<double> a_elem;
        std::vector<int> Xini{ind1, ind2, ind3, ind4};
        int w, x, y, z;
        auto Uargs = [this](std::vector<int> Xijkl, int Ws, int dd, int signdd)
        {std::vector<int> outvec;for(int d=0;d<4;d++){if(signdd>0){outvec.push_back(Mod(Xijkl[d]+Ws*(d==dd),Lattice_length));}else{outvec.push_back(Mod(Xijkl[d]-(1+Ws)*(d==dd),Lattice_length));}}return(outvec); };

        std::vector<int> indshift1{(d1 == 0), (d1 == 1), (d1 == 2), (d1 == 3)};
        std::vector<int> indshift2{(d2 == 0), (d2 == 1), (d2 == 2), (d2 == 3)};

        std::vector<int> XJ{Mod(Xini[0] + indshift1[0], Lattice_length), Mod(Xini[1] + indshift1[1], Lattice_length), Mod(Xini[2] + indshift1[2], Lattice_length), Mod(Xini[3] + indshift1[3], Lattice_length)};
        std::vector<int> XK{Mod(Xini[0] + indshift1[0] + indshift2[0], Lattice_length), Mod(Xini[1] + indshift1[1] + indshift2[1], Lattice_length), Mod(Xini[2] + indshift1[2] + indshift2[2], Lattice_length), Mod(Xini[3] + indshift1[3] + indshift2[3], Lattice_length)};
        std::vector<int> XL{Mod(Xini[0] + indshift2[0], Lattice_length), Mod(Xini[1] + indshift2[1], Lattice_length), Mod(Xini[2] + indshift2[2], Lattice_length), Mod(Xini[3] + indshift2[3], Lattice_length)};
        std::vector<int> Uarg;

        std::vector<int> dparam{d1, d2, d1, d2};
        std::vector<int> signdparam{signd1, signd2, -signd1, -signd2};
        std::vector<std::vector<int>> Xparam = {Xini, XJ, XK, XL};

        for (int Unumb = 0; Unumb < 4; Unumb++)
        {
            for (int ws = 0; ws < Wsize; ws++)
            {
                Uarg = Uargs(Xparam[Unumb], ws, dparam[Unumb], signdparam[Unumb]);
                w = Uarg[0];
                x = Uarg[1];
                y = Uarg[2];
                z = Uarg[3];
                if (signdparam[Unumb] > 0)
                {
                    a_elem = a.at(w, x, y, z, dparam[Unumb]);
                }
                else
                {
                    a_elem = PauliInv(a.at(w, x, y, z, dparam[Unumb]));
                }
                Ulist.insert(Ulist.end(), a_elem.begin(), a_elem.end());
            }
        }
        return (Ulist);
    }
    double site_action(int size, int i1, int i2, int i3, int i4, int direct1)
    {
        double S = 12 * (size == 1);
        double si;

        for (int direct2 = 0; direct2 < 4; direct2++)
        {
            if (direct2 == direct1)
            {
                continue;
            }
            for (int revd1 = 0; revd1 < 2; revd1++)
            {
                for (int revd2 = 0; revd2 < 2; revd2++)
                {
                    si = 0.5 * pow(-1, size == 1) * Trace(Neighbours(i1, i2, i3, i4, direct1, direct2, pow(-1, revd1), pow(-1, revd2), size));
                    S += si;
                }
            }
        }
        return (S);
    }
    double Action(int size = 1)
    {
        double S = 0;
        for (int i1 = 0; i1 < Lattice_length; i1++)
        {
            for (int i2 = 0; i2 < Lattice_length; i2++)
            {
                for (int i3 = 0; i3 < Lattice_length; i3++)
                {
                    for (int i4 = 0; i4 < Lattice_length; i4++)
                    {
                        for (int i5 = 0; i5 < 4; i5++)
                        {
                            S += site_action(size, i1, i2, i3, i4, i5);
                        }
                    }
                }
            }
        }
        return (S);
    }
    void Average_plaquette()
    {
        Avplaq_data.push_back(Action() / Nplaq);
    }
    void Wloop_expct()
    {
        int Wmax;
        if (Lattice_length == 6 || Lattice_length == 4)
        {
            Wmax = Lattice_length;
        }
        else if (Lattice_length == 2)
        {
            Wmax = 3;
        }
        else
        {
            Wmax = 7;
        }
        Wloop_data.push_back(1 - Action() / Nplaq);
        for (int WS = 2; WS < Wmax; WS++)
        {
            Wloop_data.push_back(Action(WS) / Nplaq);
        }
    }
    std::vector<double> New_element(double beta, int i1, int i2, int i3, int i4, int d1)
    {
        std::vector<double> New_a_elem;
        std::vector<double> USum{0, 0, 0, 0};
        std::vector<double> neighprod;
        for (int d = 0; d < 4; d++)
        {
            if (d == d1)
            {
                continue;
            }
            for (int revd = 0; revd < 2; revd++)
            {
                neighprod = Neighprod(i1, i2, i3, i4, d1, d, revd);
                for (int di = 0; di < 4; di++)
                {
                    USum[di] += neighprod[di];
                }
            }
        }
        double k = sqrt(PauliDet(USum));
        New_a_elem.push_back(get_ao(beta, k));
        double avv = sqrt(1 - pow(New_a_elem[0], 2)) / sqrt(3);
        std::vector<double> av = {avv, avv, avv};
        std::vector<double> a_vect = Rotate_3Dvector_random(av);
        New_a_elem.push_back(a_vect[0]);
        New_a_elem.push_back(a_vect[1]);
        New_a_elem.push_back(a_vect[2]);
        return (New_a_elem);
    }
    void Touchheat(double Beta)
    {
        std::vector<double> elem;
        int linknumb;
        a_array New_a(Lattice_length);

        for (int i1 = 0; i1 < Lattice_length; i1++)
        {
            for (int i2 = 0; i2 < Lattice_length; i2++)
            {
                for (int i3 = 0; i3 < Lattice_length; i3++)
                {
                    for (int i4 = 0; i4 < Lattice_length; i4++)
                    {
                        for (int i5 = 0; i5 < 4; i5++)
                        {
                            elem = New_element(Beta, i1, i2, i3, i4, i5);
                            linknumb = a.getIndex(i1, i2, i3, i4, i5);
                            New_a.Setlink(linknumb, elem);
                        }
                    }
                }
            }
        }
        a = New_a;

        Average_plaquette();
    }
    void Heatbath(int Iterations, double Beta)
    {
        for (int i = 0; i < Iterations; i++)
        {
            Touchheat(Beta);
        }
    }
};
#endif