
// #include <misc_functions.h>
#include <vector>
#include <math.h>
#include "Util.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

#ifndef A_CLASS
#define A_CLASS

//  The lattice
struct a_array
{
	a_array(){};
	a_array(int Lsize) : Lsize(Lsize), data(pow(Lsize, 4) * 16, std::vector<double>(4)) {}

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
	void Setlink(int linknumb, double elemw, double elemx, double elemy, double elemz)
	{

		data[linknumb][0] = elemw;
		data[linknumb][1] = elemx;
		data[linknumb][2] = elemy;
		data[linknumb][3] = elemz;
	}
	void init(bool ini_cond = 0)
	{
		std::vector<double> elem;
		for (int i1 = 0; i1 < Lsize; i1++)
		{
			for (int i2 = 0; i2 < Lsize; i2++)
			{
				for (int i3 = 0; i3 < Lsize; i3++)
				{
					for (int i4 = 0; i4 < Lsize; i4++)
					{
						for (int i5 = 0; i5 < 4; i5++)
						{
							int linknumb = getIndex(i1, i2, i3, i4, i5);
							if (ini_cond == 0)
							{
								elem = {1, 0, 0, 0};
							}
							else
							{
								elem = get_ini_rand_elem();
							}
							Setlink(linknumb, elem[0], elem[1], elem[2], elem[3]);
						}
					}
				}
			}
		}
	}
	int Lsize;
	std::vector<std::vector<double>> data;
};
#endif
