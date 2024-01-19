#include <vector>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include "../misc/Util.h"

using namespace glm;

#ifndef A_CLASS
#define A_CLASS

//  The lattice
struct a_array
{
	a_array(){};
	a_array(int Lsize) : Lsize(Lsize), numb4vect(pow(Lsize, 4) * 4), data(numb4vect, std::vector<double>(4)) {}

	std::vector<double> &at(int i1, int i2, int i3, int i4, int i5)
	{
		int gind = getIndex(i1, i2, i3, i4, i5);
		return data[gind];
	}
	int getIndex(int i1, int i2, int i3, int i4, int i5)
	{
		assert(i1 >= 0 && i1 < Lsize);
		assert(i2 >= 0 && i2 < Lsize);
		assert(i3 >= 0 && i3 < Lsize);
		assert(i4 >= 0 && i4 < Lsize);
		assert(i5 >= 0 && i5 < 4);

		return i1 * 4 * pow(Lsize, 3) + i2 * 4 * pow(Lsize, 2) + i3 * 4 * Lsize + i4 * 4 + i5;
	}
	void Setlink(int linknumb, std::vector<double> elem)
	{

		data[linknumb][0] = elem[0];
		data[linknumb][1] = elem[1];
		data[linknumb][2] = elem[2];
		data[linknumb][3] = elem[3];
	}
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
	int Lsize, numb4vect;
	std::vector<std::vector<double>> data;
};
#endif
