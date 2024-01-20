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
	int Lsize;							   // size of the lattice
	int numb4vect;						   // number of elements (vectors of size4) stored
	std::vector<std::vector<double>> data; // vector of elements(links which themselves are 4D vectors)
	a_array(){};
	a_array(int Lsize) : Lsize(Lsize), numb4vect(pow(Lsize, 4) * 4), data(numb4vect, std::vector<double>(4)) {}
	/**Instead of storing multidimensional vector simply create a 1D vector and translate the "indices" using getIndex*/
	std::vector<double> &at(int i1, int i2, int i3, int i4, int i5);
	/**Translate multi-index to single index*/
	int getIndex(int i1, int i2, int i3, int i4, int i5);
	/**Set a link element of given index to the contents of the vector elem*/
	void Setlink(int linknumb, std::vector<double> elem);
	/**Either set all links to (1,0,0,0) or random element (of determiant 1)*/
	void init(bool ini_cond);
};
#endif