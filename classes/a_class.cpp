#include "a_class.h"

std::vector<double> a_array::at(int i1, int i2, int i3, int i4, int i5)
{
    int linknumb = getIndex(i1, i2, i3, i4, i5);
    assert(data.size() - (linknumb + 3) >= 0);
    return std::vector<double>(data.begin() + linknumb, data.begin() + linknumb + 4);
}

/**Translate multi-index to single index*/
int a_array::getIndex(int i1, int i2, int i3, int i4, int i5)
{
    assert(i1 >= 0 && i1 < Lsize);
    assert(i2 >= 0 && i2 < Lsize);
    assert(i3 >= 0 && i3 < Lsize);
    assert(i4 >= 0 && i4 < Lsize);
    assert(i5 >= 0 && i5 < 4);
    return (i1 * 4 * pow(Lsize, 3) + i2 * 4 * Lsize * Lsize + i3 * 4 * Lsize + i4 * 4 + i5) * 4;
}
/**Either set all links to (1,0,0,0) or random element (of determiant 1)*/
void a_array::init(bool ini_cond = 0)
{
    std::vector<double> elem(4);
    for (int linknumb = 0; linknumb < 4 * numb4vect; linknumb = linknumb + 4)
    {
        // std::vector<double> elem = (ini_cond) ? get_ini_rand_elem() : std::vector<double>{1, 0, 0, 0};
        if (ini_cond)
        {
            elem = get_ini_rand_elem();
        }
        else
        {
            elem = {1, 0, 0, 0};
        }
        assert(data.size() - (linknumb + 3) >= 0);
        std::copy(elem.begin(), elem.end(), data.begin() + linknumb);
    }
}