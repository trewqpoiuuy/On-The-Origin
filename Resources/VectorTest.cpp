/*
 * VectorTest.cpp

 *
 *  Created on: Jan 15, 2014
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

//returns a string based on the given resource base and mod values
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
		cout << "topsoil " << StrOutput << "\n";
		return StrOutput;
	}
	else
	{
		ostringstream PlaceHolder;
		PlaceHolder << rand() % water.mod + water.base << rand() % nitrogen.mod + nitrogen.base << rand() % phosphorus.mod + phosphorus.base << rand() % potassium.mod + potassium.base;
		string StrOutput = PlaceHolder.str();
		cout << "subsoil " << StrOutput << "\n";
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

string ResourceGrab(int x, int y, int z, int width, int length, int depth, vector<string> ResourceVector)
	{
	return ResourceVector[z+(y*depth)+(x*depth*width)];
	}

int main()
{
	//determines the depth, length, and width of the block of soil contained by the vector, also determines the depth of the topsoil.
	int depth = 10;
	int length = 100;
	int width = 100;
	int TopsoilDepth = 3;

	//creates resources
	ResourceStruct water = CreateResource(water, 40, 10, 50, 5);
	ResourceStruct nitrogen = CreateResource(nitrogen, 30, 10, 50, 5);
	ResourceStruct phosphorus = CreateResource(phosphorus, 20, 10, 50, 5);
	ResourceStruct potassium = CreateResource(potassium, 10, 10, 50, 5);

	//creates the vector for storing variable
	vector<string> ResourceVector;
	ResourceVector.resize(depth*width*length);

	//calls the PopVector function sequentially
	for (int x=0; x<length; x++)
		for (int y=0; y<width; y++)
			for (int z=0; z<depth; z++)
			{
				if (z>TopsoilDepth)
				ResourceVector[z+(y*depth)+(x*width*depth)] = PopVector(false, water, nitrogen, phosphorus, potassium);

				else
				ResourceVector[z+(y*depth)+(x*width*depth)] = PopVector(true, water, nitrogen, phosphorus, potassium);
			}


	//generates random coordinates for testing
	int xtest = rand () % length;
	int ytest = rand () % width;
	int ztest = rand () % depth;

	//couts for testing purposes, uses random xyz coordinates (see above) to return the contents of the vector at that point
	cout << "x: " << xtest << "| y: " << ytest << "| z: " << ztest << "\n";
	cout << ResourceGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << "\nWater: ";
	cout << WaterGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << "\nNitrogen: ";
	cout << NitrogenGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << "\nPhosphorus: ";
	cout << PhosphorusGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << "\nPotassium: ";
	cout << PotassiumGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << "\nCapacity: ";
	cout << ResourceVector.capacity();
}
