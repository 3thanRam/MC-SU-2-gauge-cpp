#include <vector>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "../misc/Util.h"

using namespace glm;

#ifndef A_CLASS
#define A_CLASS

/** Where & how the links are created,set,stored,accessed,.. */
struct a_array
{
	a_array(){};
	a_array(int Lsize) : Lsize(Lsize), numb4vect(pow(Lsize, 4) * 4), data(numb4vect, std::vector<double>(4)) {}

	/**Instead of storing multidimensional vector simply create a 1D vector and translate the "indices" using getIndex*/
	std::vector<double> &at(int i1, int i2, int i3, int i4, int i5)
	{
		int gind = getIndex(i1, i2, i3, i4, i5);
		return data[gind];
	}
	/**Translate multi-index to single index*/
	int getIndex(int i1, int i2, int i3, int i4, int i5)
	{
		assert(i1 >= 0 && i1 < Lsize);
		assert(i2 >= 0 && i2 < Lsize);
		assert(i3 >= 0 && i3 < Lsize);
		assert(i4 >= 0 && i4 < Lsize);
		assert(i5 >= 0 && i5 < 4);

		return i1 * 4 * pow(Lsize, 3) + i2 * 4 * pow(Lsize, 2) + i3 * 4 * Lsize + i4 * 4 + i5;
	}
	/**Set a link element of given index to the contents of the vector elem*/
	void Setlink(int linknumb, std::vector<double> elem)
	{

		data[linknumb][0] = elem[0];
		data[linknumb][1] = elem[1];
		data[linknumb][2] = elem[2];
		data[linknumb][3] = elem[3];
	}
	/**Either set all links to (1,0,0,0) or random element (of determiant 1)*/
	void init(bool ini_cond = 0)
	{
		std::vector<double> elem(4);
		for (int linknumb = 0; linknumb < numb4vect; linknumb++)
		{
			if (ini_cond == 0)
			{
				elem = {1, 0, 0, 0};
			}
			else
			{
				elem = get_ini_rand_elem();
			}
			Setlink(linknumb, elem);
		}
	}
	int Lsize;							   // size of the lattice
	int numb4vect;						   // number of elements (vectors of size4) stored
	std::vector<std::vector<double>> data; // vector of elements(links which themselves are 4D vectors)
};
#endif
