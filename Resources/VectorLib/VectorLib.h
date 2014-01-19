/*
 * VectorLib.h
 *
 *  Created on: Jan 16, 2014
 *      Author: WBoettcher
 */

#ifndef VECTORLIB_H_
#define VECTORLIB_H_
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;
namespace veclib
{
//Creates a structure for resources for use as arguments

struct ResourceStruct {
	int base;
	int mod;
	int baseT;
	int modT;
};

//Fills ResourceStructs water, nitrogen, phosphorus and potassium with variables (which must be defined before this function is executed)
ResourceStruct CreateResource(ResourceStruct TypeName, int Base, int Mod, int BaseT, int ModT)
	{
		TypeName.base = Base;
		TypeName.baseT = BaseT;
		TypeName.mod = Mod;
		TypeName.modT = ModT;

		return TypeName;
	}

//returns a string based on the given base and mod values for each resource.
string PopVector
		(
		bool Topsoil,
		ResourceStruct water,
		ResourceStruct nitrogen,
		ResourceStruct phosphorus,
		ResourceStruct potassium
		)
{
	srand (time(NULL));
	if (Topsoil==true) //determines if the function should return values for topsoil
	{
		//generates pseudo-random variables based on parameter presets for each resource, and returns them in the form of a string
		ostringstream PlaceHolder;
		PlaceHolder << rand() % water.modT + water.baseT << rand() % nitrogen.modT + nitrogen.baseT << rand() % phosphorus.modT + phosphorus.baseT << rand() % potassium.modT + potassium.baseT;
		string StrOutput = PlaceHolder.str();
		return StrOutput;
	}
	else
	{
		ostringstream PlaceHolder;
		PlaceHolder << rand() % water.mod + water.base << rand() % nitrogen.mod + nitrogen.base << rand() % phosphorus.mod + phosphorus.base << rand() % potassium.mod + potassium.base;
		string StrOutput = PlaceHolder.str();
		return StrOutput;
	}
}

//returns the int value for the water resource at a given position
int WaterGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return result/1000000;
}

//returns the int value for the nitrogen resource at a given position
int NitrogenGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return (result/10000)%100;
}

//returns the int value for the phosphorus resource at a given position
int PhosphorusGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return (result%10000)/100;
}

//returns the int value for the potassium resource at a given position
int PotassiumGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return result%100;
}
//returns the contents of the vector at the designated coordinated
string ResourceGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
	{
	return ResourceVector[z+(y*depth)+(x*depth*width)];
	}

};

#endif /* VECTORLIB_H_ */
