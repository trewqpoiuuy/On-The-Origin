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
int WaterGrab(int x, int y, int z, int width, int length, int depth, vector<string>& ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return result/1000000;
}

//returns the int value for the nitrogen resource at a given position
int NitrogenGrab(int x, int y, int z, int width, int length, int depth, vector<string>& ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return (result/10000)%100;
}

//returns the int value for the phosphorus resource at a given position
int PhosphorusGrab(int x, int y, int z, int width, int length, int depth, vector<string>& ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return (result%10000)/100;
}

//returns the int value for the potassium resource at a given position
int PotassiumGrab(int x, int y, int z, int width, int length, int depth, vector<string>& ResourceVector)
{
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int result;
	ss >> result;
	return result%100;
}
//returns the contents of the vector at the designated coordinated
string ResourceGrab(int x, int y, int z, int width, int length, int depth, vector<string>& ResourceVector)
{
	return ResourceVector[z+(y*depth)+(x*depth*width)];
}

//changes the specified resource at the specified point the specified amount, and returns the amount that it was changed.
int ResourceChange(int x, int y, int z, int width, int length, int depth, vector<string>& ResourceVector, string resource, int change)
{
	//finds the string for the correct point in ResourceVector, and sets tempresources equal to its int equivalent.
	stringstream ss(ResourceVector[z+(y*depth)+(x*depth*width)]);
	int tempresources;
	ss >> tempresources;

	//separates tempresources into individual resources
	int water = tempresources/1000000;
	int nitrogen = (tempresources/10000)%100;
	int phosphorus = (tempresources%10000)/100;
	int potassium = tempresources%100;

	//adds change where appropriate
	//if change would reduce the resource below 0, sets resource to 0, and sets returnvalue to the amount that could be removed from the resource.
	//if change would increase the resource above 99, sets resource to 99, and sets returnvalue to the amount that could be added to the resource.
	//otherwise, sets returnvalue to -(change)
	int returnvalue;
	if (resource=="water")
	{
		water += change;
		if (water<0)
		{
			water=0;
			returnvalue = tempresources/1000000;
		}
		else if (water>99)
		{
			water = 99;
			returnvalue = 99-(tempresources/1000000);
		}
		else
		{
			returnvalue = 0 - change;
		}
		//cout << "\n Water: "<< water;
		//cout << "\n returnvalue: " << returnvalue;
	}
	else if (resource=="nitrogen")
	{
		nitrogen += change;
		if (nitrogen<0)
		{
			nitrogen=0;
			returnvalue = (tempresources/10000)%100;
		}
		else if (nitrogen>99)
		{
			nitrogen = 99;
			returnvalue = 99-(tempresources/10000)%100;
		}
		else
			returnvalue = 0 - change;
	}
	else if (resource=="phosphorus")
	{
		phosphorus += change;
		if (phosphorus<0)
		{
			phosphorus=0;
			returnvalue = (tempresources%10000)/100;
		}
		else if (phosphorus>99)
		{
			phosphorus = 99;
			returnvalue = 99-(tempresources%10000)/100;
		}
		else
			returnvalue = 0 - change;
	}
	else if (resource=="potassium")
	{
		potassium += change;
		if (potassium<0)
		{
			potassium=0;
			returnvalue = tempresources%100;
		}
		else if (potassium>99)
		{
			potassium = 99;
			returnvalue = 99-tempresources%100;
		}
		else
			returnvalue = 0 - change;
	}
	else
		{
		cout << "\nError: invalid resource";
		return 0;
		}
	//puts the resources back into a string, while adding 0's if the any of the resources have less than 2 digits.
	//this preserves the formatting, and keeps the string readable.
	ostringstream PlaceHolder;
	//water
	if (water<10)
		PlaceHolder << 0 << water;
	else
		PlaceHolder << water;
	//nitrogen
	if (nitrogen<10)
		PlaceHolder << 0 << nitrogen;
	else
		PlaceHolder << nitrogen;
	//phosphorus
	if (phosphorus<10)
		PlaceHolder << 0 << phosphorus;
	else
		PlaceHolder << phosphorus;
	//potassium
	if (potassium<10)
		PlaceHolder << 0 << potassium;
	else
		PlaceHolder << potassium;

	//puts the string back into ResourceVector
	string StrOutput = PlaceHolder.str();
	ResourceVector[z+(y*depth)+(x*depth*width)] = StrOutput;
	//cout << "\n StrOutput: "<< StrOutput;

	//returns the value by which the desired resource was REDUCED
	return returnvalue;
}
vector<string> initializeResources(int depth, int length, int width, int TopsoilDepth)
{
	ResourceStruct water = CreateResource(water, 40, 10, 50, 5);
	ResourceStruct nitrogen = CreateResource(nitrogen, 40, 10, 50, 5);
	ResourceStruct phosphorus = CreateResource(phosphorus, 40, 10, 50, 5);
	ResourceStruct potassium = CreateResource(potassium, 40, 10, 50, 5);

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
	return ResourceVector;
}
};

#endif /* VECTORLIB_H_ */
