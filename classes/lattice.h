#include "../classes/a_class.h"

#ifndef LATTICE
#define LATTICE

struct Lattice
{
    unsigned int Lattice_length; // length of the lattice
    bool ini_cond;               // initial state of array a: {1,0,0,0}(0) or random(1)
    int totnumb_orientlinks;     // 4 * pow(Lat_Length_set, 4)
    int totnumb_elements;        // 4*totnumb_orientlinks
    int Nplaq;                   // degen(direction1)*degen(direction2)=2*(3*2) -> =12 * totnumb_orientlinks
    int Multithreadmode;
    a_array a;
    std::vector<double> Avplaq_data;
    std::vector<double> Wloop_data;
    Lattice() : Lattice_length(0){};
    Lattice(int Lattice_length, bool ini_cond, int Multithreadmode) : Lattice_length(Lattice_length), ini_cond(ini_cond), Multithreadmode(Multithreadmode), a(Lattice_length), totnumb_orientlinks(4 * pow(Lattice_length, 4)), totnumb_elements(4 * totnumb_orientlinks), Nplaq(12 * totnumb_orientlinks)
    {
        a.init(ini_cond);
    }
    /** Returns the product of links to (ind1,ind2,ind3,ind4) (itself excluded) forming a plaquette
     * in plane (d1,d2) with corresponding directions  (+1,signd2)
     */
    std::vector<double> Neighprod(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int revd2)
    {
        std::vector<double> Ulinks = Neighbours(ind1, ind2, ind3, ind4, d1, d2, 1, pow(-1, revd2));
        std::vector<double> U3(Ulinks.end() - 4, Ulinks.end());
        std::vector<double> U2(Ulinks.end() - 8, Ulinks.end() - 4);
        std::vector<double> U1(Ulinks.end() - 12, Ulinks.end() - 8);

        std::vector<double> Uprod = Paulimult(U1, Paulimult(U2, U3));
        return (Uprod);
    }
    /** Returns ordered elements of links forming wilson loop
     *  of size Wsize starting at (ind1,ind2,ind3,ind4)
     * in plane (d1,d2) with corresponding directions  signd1,signd2
     */
    std::vector<double> Neighbours(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int signd1, int signd2, int Wsize = 1)
    {
        std::vector<double> Ulist(16 * Wsize);
        std::vector<double> a_elem;
        a_elem.reserve(4);
        std::vector<int> Xini{ind1, ind2, ind3, ind4};
        int w, x, y, z;
        auto Uargs = [this](std::vector<int> const &Xijkl, int Ws, int dd, int signdd)
        {std::vector<int> outvec(4);for(int d=0;d<4;d++){if(signdd>0){outvec[d]=Mod(Xijkl[d]+Ws*(d==dd),Lattice_length);}else{outvec[d]=Mod(Xijkl[d]-(1+Ws)*(d==dd),Lattice_length);}}return(outvec); };

        int indshift1[4] = {Wsize * (d1 == 0), Wsize * (d1 == 1), Wsize * (d1 == 2), Wsize * (d1 == 3)};
        int indshift2[4] = {Wsize * (d2 == 0), Wsize * (d2 == 1), Wsize * (d2 == 2), Wsize * (d2 == 3)};

        std::vector<int> XJ{Mod(Xini[0] + indshift1[0], Lattice_length), Mod(Xini[1] + indshift1[1], Lattice_length), Mod(Xini[2] + indshift1[2], Lattice_length), Mod(Xini[3] + indshift1[3], Lattice_length)};
        std::vector<int> XK{Mod(Xini[0] + indshift1[0] + indshift2[0], Lattice_length), Mod(Xini[1] + indshift1[1] + indshift2[1], Lattice_length), Mod(Xini[2] + indshift1[2] + indshift2[2], Lattice_length), Mod(Xini[3] + indshift1[3] + indshift2[3], Lattice_length)};
        std::vector<int> XL{Mod(Xini[0] + indshift2[0], Lattice_length), Mod(Xini[1] + indshift2[1], Lattice_length), Mod(Xini[2] + indshift2[2], Lattice_length), Mod(Xini[3] + indshift2[3], Lattice_length)};
        std::vector<int> Uarg;

        int dparam[4] = {d1, d2, d1, d2};
        int signdparam[4] = {signd1, signd2, -signd1, -signd2};
        std::vector<std::vector<int>> Xparam = {Xini, XJ, XK, XL};

        int Uind;
        for (int Unumb = 0; Unumb < 4; Unumb++)
        {
            for (int ws = 0; ws < Wsize; ws++)
            {
                Uarg = Uargs(Xparam[Unumb], ws, dparam[Unumb], signdparam[Unumb]);
                w = Uarg[0];
                x = Uarg[1];
                y = Uarg[2];
                z = Uarg[3];
                a_elem = a.at(w, x, y, z, dparam[Unumb]);
                if (signdparam[Unumb] < 0)
                {
                    a_elem[1] *= -1;
                    a_elem[2] *= -1;
                    a_elem[3] *= -1;
                }
                Uind = 4 * (Unumb * Wsize + ws);
                Ulist[Uind] = a_elem[0];
                Ulist[Uind + 1] = a_elem[1];
                Ulist[Uind + 2] = a_elem[2];
                Ulist[Uind + 3] = a_elem[3];
            }
        }
        return Ulist;
    }
    /** Returns the contribution to the action of the wilson loop of a given size of a given site*/
    double site_action(int size, int i1, int i2, int i3, int i4, int direct1)
    {
        double S = 12 * (size == 1);
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
                    S += 0.5 * pow(-1, size == 1) * Trace(Neighbours(i1, i2, i3, i4, direct1, direct2, pow(-1, revd1), pow(-1, revd2), size));
                }
            }
        }
        return S;
    }
    /** Returns the total action of the system resulting from the wilson loops of a given size
     * Since there are a lot of repeated independent operations it is sometimes advantageous to use threading to speed up the calculations
     */
    double Action(int size = 1)
    {
        double S = 0;

        auto mp_action = [this, size](int ind4, int param3)
        {
            double Sm = 0;
            std::vector<double> elem;
            counter<size_t, 3> my_counter({0, 0, 0}, {Lattice_length, Lattice_length, Lattice_length});
            while (my_counter)
            {
                for (int ind3 = Lattice_length * param3 / 2; ind3 < Lattice_length * (param3 + 1) / 2; ind3++)
                {
                    Sm += site_action(size, my_counter._<0>(), my_counter._<1>(), my_counter._<2>(), ind3, ind4);
                }
                ++my_counter;
            }
            return Sm;
        };
        if (Multithreadmode == 2)
        {
            std::vector<std::future<double>> futures;
            for (int c = 0; c < 4; c++)
            {
                for (int c2 = 0; c2 < 2; c2++)
                {
                    futures.push_back(std::async(mp_action, c, c2));
                }
            }
            for (auto &f : futures)
            {
                S += f.get();
            }
        }
        else
        {
            counter<size_t, 5> my_counter({0, 0, 0, 0, 0}, {Lattice_length, Lattice_length, Lattice_length, Lattice_length, 4});
            while (my_counter)
            {
                S += site_action(size, my_counter._<0>(), my_counter._<1>(), my_counter._<2>(), my_counter._<3>(), my_counter._<4>());
                ++my_counter;
            }
        }
        return S;
    }
    void Average_plaquette()
    {
        // Technically 4*Average plaquette since each plaquette is inserted 4 times once for each link
        // But removed factor in accordance with article
        Avplaq_data.push_back(Action() / Nplaq);
    }
    /** Returns the expectation value of different sized wilson loops for the system  */
    void Wloop_expct()
    {
        int Wmax = MaxWilsonloop(Lattice_length);
        Wloop_data.push_back(1 - Action() / Nplaq);
        for (int WS = 2; WS < Wmax; WS++)
        {
            Wloop_data.push_back(Action(WS) / Nplaq);
        }
    }
    /** New link to replace old one according to the Boltzmann factor  */
    std::vector<double> New_element(double beta, int i1, int i2, int i3, int i4, int d1)
    {
        std::vector<double> New_a_elem(4);
        std::vector<double> USum{0, 0, 0, 0};
        std::vector<double> neighprod(4);
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
        New_a_elem[0] = get_ao(beta, k);
        double avv = sqrt(1 - pow(New_a_elem[0], 2)) / sqrt(3);
        std::vector<double> av = {avv, avv, avv};
        std::vector<double> a_vect = Rotate_3Dvector_random(av);
        New_a_elem[1] = a_vect[0];
        New_a_elem[2] = a_vect[1];
        New_a_elem[3] = a_vect[2];
        return (New_a_elem);
    }
    /** Replaces each link in the lattice by touching a heatbath to each link
     * Since there are a lot of repeated independent operations it is sometimes advantageous to use threading to speed up the calculations
     */
    void Touchheat(double Beta)
    {
        a_array New_a(Lattice_length);
        auto mp_touchheat = [this, Beta](int ind4, int param3)
        {
            std::vector<std::pair<int, std::vector<double>>> mpdata;
            mpdata.reserve(pow(Lattice_length, 4));
            std::vector<double> elem;
            elem.reserve(4);
            int linknumb;
            counter<size_t, 3> my_counter({0, 0, 0}, {Lattice_length, Lattice_length, Lattice_length});
            while (my_counter)
            {
                for (int ind3 = Lattice_length * param3 / 2; ind3 < Lattice_length * (param3 + 1) / 2; ind3++)
                {
                    elem = New_element(Beta, my_counter._<0>(), my_counter._<1>(), my_counter._<2>(), ind3, ind4);
                    linknumb = my_counter._<0>() * 4 * pow(Lattice_length, 3) + my_counter._<1>() * 4 * pow(Lattice_length, 2) + my_counter._<2>() * 4 * Lattice_length + ind3 * 4 + ind4;
                    mpdata.emplace_back(std::make_pair(linknumb, elem));
                }
                ++my_counter;
            }
            return mpdata;
        };
        if (Multithreadmode == 2)
        {
            std::vector<std::future<std::vector<std::pair<int, std::vector<double>>>>> futures;
            for (int c = 0; c < 4; c++)
            {
                for (int c2 = 0; c2 < 2; c2++)
                {
                    futures.push_back(std::async(mp_touchheat, c, c2));
                }
            }
            for (auto &f : futures)
            {
                for (auto &elem : f.get())
                {
                    New_a.Setlink(std::get<0>(elem), std::get<1>(elem));
                }
            }
        }
        else
        {
            std::vector<double> elem;
            elem.reserve(4);
            int linknumb;
            counter<size_t, 5> my_counter({0, 0, 0, 0, 0}, {Lattice_length, Lattice_length, Lattice_length, Lattice_length, 4});
            while (my_counter)
            {
                elem = New_element(Beta, my_counter._<0>(), my_counter._<1>(), my_counter._<2>(), my_counter._<3>(), my_counter._<4>());
                linknumb = a.getIndex(my_counter._<0>(), my_counter._<1>(), my_counter._<2>(), my_counter._<3>(), my_counter._<4>());
                New_a.Setlink(linknumb, elem);
                ++my_counter;
            }
        }
        a = New_a;

        Average_plaquette();
    }
    /** Repeated application of touching a heatbath to each link variable to achieve equilibrium*/
    void Heatbath(int Iterations, double Beta)
    {
        for (int i = 0; i < Iterations; i++)
        {
            Touchheat(Beta);
        }
    }
};
#endif