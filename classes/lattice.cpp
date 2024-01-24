#include "lattice.h"
// Add value of site_action at site indices to S
void Lattice::UpdateS(double &S, int size, int i0, int i1, int i2, int i3, int i4)
{
    S += site_action(size, i0, i1, i2, i3, i4);
}
// Add value of site_action hidden inside future to S
void Lattice::UpdateS(double &S, std::future<double> &future)
{
    S += future.get();
}
// Set link identified with linknumb of arr to contents of elem generated with New_element
void Lattice::Updatearray(a_array &arr, double beta, int i0, int i1, int i2, int i3, int i4)
{
    std::vector<double> elem = New_element(beta, i0, i1, i2, i3, i4);
    int linknumb = arr.getIndex(i0, i1, i2, i3, i4);
    assert(arr.data.size() - (linknumb + 3) >= 0);
    std::copy(elem.begin(), elem.end(), arr.data.begin() + linknumb);
}
/**Set link (identified with elem[0]) of arr to contents of elem[1]
 * with elem obtained from calling futures (which is itself New_element called with threading) */
void Lattice::Updatearray(a_array &arr, std::future<std::vector<std::pair<int, std::vector<double>>>> &futures)
{
    for (auto &elem : futures.get())
    {
        int linknumb = std::get<0>(elem);
        std::vector<double> Newelem = std::get<1>(elem);
        assert(arr.data.size() - (linknumb + 3) >= 0);
        std::copy(Newelem.begin(), Newelem.end(), arr.data.begin() + linknumb);
    }
}
/** Returns the product of links to (ind1,ind2,ind3,ind4) (itself excluded) forming a plaquette
 * in plane (d1,d2) with corresponding directions  (+1,signd2)
 */
std::vector<double> Lattice::Neighprod(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int revd2)
{
    std::vector<double> Ulinks = Neighbours(ind1, ind2, ind3, ind4, d1, d2, 1, pow(-1, revd2));
    std::vector<double> U3(Ulinks.end() - 4, Ulinks.end());
    std::vector<double> U2(Ulinks.end() - 8, Ulinks.end() - 4);
    std::vector<double> U1(Ulinks.end() - 12, Ulinks.end() - 8);

    std::vector<double> Uprod = Paulimult(U1, Paulimult(U2, U3));
    return (Uprod);
}
/** Returns ordered elements of links forming wilson loop
 *  -Of size Wsize starting at (ind1,ind2,ind3,ind4)
 *  -In plane (d1,d2) with corresponding directions  signd1,signd2
 */
std::vector<double> Lattice::Neighbours(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int signd1, int signd2, int Wsize)
{
    std::vector<double> Ulist(16 * Wsize);
    std::vector<double> a_elem(4);
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
            /**At the end Ulist will look like:
             * U0*Wsize,U1*Wsize,U2*Wsize,U3*Wsize
             * But with unpacked elements (each link has 4 elements)
             */
            Ulist[Uind] = a_elem[0];
            Ulist[Uind + 1] = a_elem[1];
            Ulist[Uind + 2] = a_elem[2];
            Ulist[Uind + 3] = a_elem[3];
        }
    }
    return Ulist;
}
/** Returns the contribution to the action of the wilson loop of a given size of a given site*/
double Lattice::site_action(int size, int i1, int i2, int i3, int i4, int direct1)
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
double Lattice::Action(int size)
{
    double S = 0;
    if (Multithreadmode != 2)
    {
        fullcounter(S, size, [this](double &S0, int size, int i1, int i2, int i3, int i4, int direct1)
                    { return UpdateS(S0, size, i1, i2, i3, i4, direct1); });
    }
    else
    {
        std::vector<std::future<double>> futures;
        auto update_target = [this](double &S0, std::future<double> &future)
        { UpdateS(S0, future); };
        auto init_fct = [this]()
        {
            double S0 = 0;
            return S0;
        };
        auto mpupdate_fct = [this](double &S0, int size, int i1, int i2, int i3, int i4, int direct1)
        { return UpdateS(S0, size, i1, i2, i3, i4, direct1); };
        mpcounter(S, size, futures, update_target, init_fct, mpupdate_fct);
    }
    return S;
}
void Lattice::Average_plaquette()
{
    // Technically 4*Average plaquette since each plaquette is inserted 4 times once for each link it contains
    Avplaq_data.push_back(Action() / (Nplaq));
}
/** Returns the expectation value of different sized wilson loops for the system  */
void Lattice::Wloop_expct()
{
    int Wmax = MaxWilsonloop(Lattice_length);
    Wloop_data.push_back(1 - (Action() / Nplaq));
    for (int WS = 2; WS < Wmax; WS++)
    {
        Wloop_data.push_back(Action(WS) / (Nplaq));
    }
}
/** New link to replace old one according to the Boltzmann factor  */
std::vector<double> Lattice::New_element(double beta, int i1, int i2, int i3, int i4, int d1)
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
    double vect3length = sqrt(1 - New_a_elem[0] * New_a_elem[0]);
    std::vector<double> a_3vect = Randirectionvector(vect3length);
    New_a_elem[1] = a_3vect[0];
    New_a_elem[2] = a_3vect[1];
    New_a_elem[3] = a_3vect[2];
    assert(abs(PauliDet(New_a_elem) - 1) < 1e-10);
    return (New_a_elem);
}
/** Replaces each link in the lattice by touching a heatbath to each link
 * Since there are a lot of repeated independent operations it is sometimes advantageous to use threading to speed up the calculations
 */
void Lattice::Touchheat(double Beta)
{
    a_array New_a(Lattice_length);
    if (Multithreadmode == 2)
    {
        std::vector<std::future<std::vector<std::pair<int, std::vector<double>>>>> futures;
        auto update_target = [this](a_array &arr, std::future<std::vector<std::pair<int, std::vector<double>>>> &future)
        { return Updatearray(arr, future); };
        auto init_fct = [this]()
        {
            std::vector<std::pair<int, std::vector<double>>> mpdata;
            mpdata.reserve(2 * pow(Lattice_length, 4));
            return mpdata;
        };
        auto mpupdate_fct = [this](std::vector<std::pair<int, std::vector<double>>> &mpdata, double Beta, int ind0, int ind1, int ind2, int ind3, int ind4)
        {
            std::vector<double> elem = New_element(Beta, ind0, ind1, ind2, ind3, ind4);
            int linknumb = (ind0 * 4 * pow(Lattice_length, 3) + ind1 * 4 * Lattice_length * Lattice_length + ind2 * 4 * Lattice_length + ind3 * 4 + ind4) * 4;
            assert(3 + linknumb < 4 * totnumb_orientlinks);
            mpdata.emplace_back(linknumb, elem);
        };
        mpcounter(New_a, Beta, futures, update_target, init_fct, mpupdate_fct);
    }
    else
    {
        fullcounter(New_a, Beta, [this](a_array &arr, double Beta, int i1, int i2, int i3, int i4, int direct1)
                    { return Updatearray(arr, Beta, i1, i2, i3, i4, direct1); });
    }
    a = New_a;

    Average_plaquette();
}
/** Repeated application of touching a heatbath to each link variable to achieve equilibrium*/
void Lattice::Heatbath(int Iterations, double Beta)
{
    for (int i = 0; i < Iterations; i++)
    {
        Touchheat(Beta);
        assert(a.data.size() == 4 * totnumb_orientlinks);
    }
};