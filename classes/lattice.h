#include "../classes/a_class.h"

#ifndef LATTICE
#define LATTICE

struct Lattice
{
    unsigned int Lattice_length;     // length of the lattice
    bool ini_cond;                   // initial state of array a: {1,0,0,0}(0) or random(1)
    int totnumb_orientlinks;         // 4 * pow(Lat_Length_set, 4)
    int Nplaq;                       // degeneracy(direction1)*degeneracy(direction2)=2x(3x2) -> =12 x totnumb_orientlinks (x4 of direct1 degeneracy factor already included in totnumb_orientlinks)
    a_array a;                       // Where the links are stored
    int Multithreadmode;             // If and how the tasks are split using threading and futures
    std::vector<double> Avplaq_data; // vector of Average plaquette at each iteration
    std::vector<double> Wloop_data;  // vector of expectation value of different sized wilson loops
    Lattice() : Lattice_length(0){};
    Lattice(int Lattice_length_set, bool ini_cond_set, int Multithreadmode_set)
        : Lattice_length(Lattice_length_set),
          ini_cond(ini_cond_set),
          totnumb_orientlinks(4 * pow(Lattice_length_set, 4)),
          Nplaq(12 * totnumb_orientlinks),
          a(Lattice_length_set),
          Multithreadmode(Multithreadmode_set)
    {
        if (Lattice_length < 4)
        {
            Multithreadmode = 0;
        }
        a.init(ini_cond);
    }

    /** In case of no threading perfom update a given variable using a given function & fixed parameter for each counter call
     * the function will be either site_action or New element
     */
    template <typename TargetType, typename FixedType, typename Callable>
    auto fullcounter(TargetType &targetvariable, FixedType fixedparam, Callable &&update_target)
    {
        counter<size_t, 5> my_counter({0, 0, 0, 0, 0}, {Lattice_length, Lattice_length, Lattice_length, Lattice_length, 4});
        while (my_counter)
        {
            update_target(targetvariable, fixedparam, my_counter._<0>(), my_counter._<1>(), my_counter._<2>(), my_counter._<3>(), my_counter._<4>());
            ++my_counter;
        }
    }
    /** In case of threading perfom async calls of mp_lambda function with a fixed parameter
     * each call contains a reduced counter but together they cover the same range
     * the fundamental function is either site_action or New element
     * Finally retrieve & use the results of the calls to update the target variable
     */
    template <typename TargetType, typename FixedType, typename FutureType, typename Callable1, typename Callable2, typename Callable3>
    auto mpcounter(TargetType &targetvariable, FixedType fixedparam, FutureType &futures, Callable1 &&update_target, Callable2 &&init_fct, Callable3 &&mpupdate_fct)
    {
        auto mp_lambda = [this, fixedparam, init_fct, mpupdate_fct](int ind4, int param3)
        {
            counter<size_t, 3> my_counter({0, 0, 0}, {Lattice_length, Lattice_length, Lattice_length});
            auto targetvar = init_fct();
            while (my_counter)
            {
                for (int ind3 = Lattice_length * param3 / 2; ind3 < Lattice_length * (param3 + 1) / 2; ind3++)
                {
                    mpupdate_fct(targetvar, fixedparam, my_counter._<0>(), my_counter._<1>(), my_counter._<2>(), ind3, ind4);
                }
                ++my_counter;
            }
            return targetvar;
        };

        //
        /**Care must be taken into how the process is divided
         * Here the task is spilt into 8 parts*/
        for (int c = 0; c < 4; c++)
        {
            for (int c2 = 0; c2 < 2; c2++)
            {
                futures.push_back(std::async(mp_lambda, c, c2));
            }
        }
        for (auto &f : futures)
        {
            update_target(targetvariable, f);
        }
    }
    // Add value of site_action at site indices to S
    void UpdateS(double &S, int size, int i0, int i1, int i2, int i3, int i4);
    // Add value site_action hidden inside future to S
    void UpdateS(double &S, std::future<double> &future);
    //  Set link identified with linknumb of arr to contents of elem generated with New_element
    void Updatearray(a_array &arr, double beta, int i0, int i1, int i2, int i3, int i4);
    /**Set link (identified with elem[0]) of arr to contents of elem[1]
     * with elem obtained from calling futures (which is itself New_element called with threading) */
    void Updatearray(a_array &arr, std::future<std::vector<std::pair<int, std::vector<double>>>> &futures);
    /** Returns the product of links to (ind1,ind2,ind3,ind4) (itself excluded) forming a plaquette
     * in plane (d1,d2) with corresponding directions  (+1,signd2)
     */
    std::vector<double> Neighprod(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int revd2);
    /** Returns ordered elements of links forming wilson loop
     *  of size Wsize starting at (ind1,ind2,ind3,ind4)
     * in plane (d1,d2) with corresponding directions  signd1,signd2
     */
    std::vector<double> Neighbours(int ind1, int ind2, int ind3, int ind4, int d1, int d2, int signd1, int signd2, int Wsize = 1);
    /** Returns the contribution to the action of the wilson loop of a given size of a given site*/
    double site_action(int size, int i1, int i2, int i3, int i4, int direct1);
    /** Returns the total action of the system resulting from the wilson loops of a given size
     * Since there are a lot of repeated independent operations it is sometimes advantageous to use threading to speed up the calculations
     */
    double Action(int size = 1);
    void Average_plaquette();
    /** Returns the expectation value of different sized wilson loops for the system  */
    void Wloop_expct();
    /** New link to replace old one according to the Boltzmann factor  */
    std::vector<double> New_element(double beta, int i1, int i2, int i3, int i4, int d1);
    /** Replaces each link in the lattice by touching a heatbath to each link
     * Since there are a lot of repeated independent operations it is sometimes advantageous to use threading to speed up the calculations
     */
    void Touchheat(double Beta);
    /** Repeated application of touching a heatbath to each link variable to achieve equilibrium*/
    void Heatbath(int Iterations, double Beta);
};
#endif