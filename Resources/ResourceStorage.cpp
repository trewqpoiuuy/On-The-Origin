/*
 * ArrayTest.cpp
 *
 *  Created on: Jan 8, 2014
 *      Author: WBoettcher
 */
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>

using namespace std;

//Creates a structure for resources for use as arguments
struct ResourceStruct {
	int base;
	int mod;
	int baseT;
	int modT;
} water, nitrogen, phosphorus, potassium;

string PopVector
		(
		bool Topsoil,
		ResourceStruct water,
		ResourceStruct nitrogen,
		ResourceStruct phosphorus,
		ResourceStruct potassium
		)
{
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

//creates a vector of strings, and populates it using resource structure inputs and the PopVector function
vector<string> InitVector(int length, int width, int depth, int TopsoilDepth, ResourceStruct water, ResourceStruct nitrogen, ResourceStruct phosphorus, ResourceStruct potassium)
{
	vector<string> ResourceVector(length*width*depth);

	if (depth > TopsoilDepth) //checks for reasonableness of depth and TopsoilDepth values
	{
		for (int x=0; x<length; x++)
			for (int y=0; y<width; y++)
				for (int z=0; z<depth; z++)
				{
					if (z<TopsoilDepth) //determines if z is sufficiently low to be topsoil
						ResourceVector[z+y*width+x*length*width]=PopVector(true, water, nitrogen, phosphorus, potassium);
					else
						ResourceVector[z+y*width+x*length*width]=PopVector(false, water, nitrogen, phosphorus, potassium);
				}
		return ResourceVector;
	}
	else
	{
		cout << "Error: Topsoil depth exceeds soil depth. Topsoil treated as 0.";
		InitVector(length, width, depth, 0, water, nitrogen, phosphorus, potassium);
	}
	return ResourceVector;
}


//Fills ResourceStructs water, nitrogen, phosphorus and potassium with variables (which must be defined before this function is executed)
ResourceStruct CreateResource(ResourceStruct TypeName, int Base, int Mod, int BaseT, int ModT)
	{
		TypeName.base = Base;
		TypeName.baseT = BaseT;
		TypeName.mod = Mod;
		TypeName.modT = ModT;

		return TypeName;
	}

//returns resource values for a given vector (w/ width, length, and depth) and coordinate set
string ResourceGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
	{
	return ResourceVector[z+y*width+x*length*width];
	}

int main() //calling the functions for testing purposes
{
	ResourceStruct water = CreateResource(water, 40, 10, 50, 5);
	ResourceStruct nitrogen = CreateResource(nitrogen, 40, 10, 50, 5);
	ResourceStruct phosphorus = CreateResource(phosphorus, 40, 10, 50, 5);
	ResourceStruct potassium = CreateResource(potassium, 40, 10, 50, 5);

	vector<string> ResourceVector = InitVector(100, 100, 10, 3, water, nitrogen, phosphorus, potassium);

	cout << ResourceGrab(3, 3, 3, 100, 100, 10, ResourceVector);
}
