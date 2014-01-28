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

struct VectorStruct {
	unsigned int water;
	unsigned int nitrogen;
	unsigned int phosphorus;
	unsigned int potassium;
	string plantID;
};

struct DimensionStruct {
	int length;
	int width;
	int depth;
	int TopsoilDepth;
};

//Fills ResourceStructs water, nitrogen, phosphorus and potassium with variables (which must be defined before this function is executed)
void CreateResource(ResourceStruct& TypeName, int Base, int Mod, int BaseT, int ModT)
	{
		TypeName.base = Base;
		TypeName.baseT = BaseT;
		TypeName.mod = Mod;
		TypeName.modT = ModT;
	}

//returns the int value for the water resource at a given position
int WaterGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].water;
}

//returns the int value for the nitrogen resource at a given position
int NitrogenGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].nitrogen;
}

//returns the int value for the phosphorus resource at a given position
int PhosphorusGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].phosphorus;
}

//returns the int value for the potassium resource at a given position
int PotassiumGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].potassium;
}

string PlantIDGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID;
}

void PlantIDAssign(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
{
	ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID = plantID;
}

bool PlantIDCheck(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector, string plantID = " ")
{
	if (ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.size() == 0)
		return false;
	else
	{
		if (plantID == ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID)
			return true;
		else if (plantID == " ")
			return true;
		else
			return false;
	}
}

//changes the specified resource at the specified point the specified amount, and returns the amount that it was changed.
int ResourceChange(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string resource, signed long long int change)
{
	signed long long int returnvalue;
	unsigned int * resourcepointer;

	//finds the correct resource, sets resourcepointer
	if (resource == "water")
		resourcepointer = &ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].water;
	else if (resource == "nitrogen")
		resourcepointer = &ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].nitrogen;
	else if (resource == "phosphorus")
		resourcepointer = &ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].phosphorus;
	else
		resourcepointer = &ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].potassium;

	signed long long int sum = *resourcepointer+change;

	if (sum < 0)
	{
		returnvalue = *resourcepointer;
		*resourcepointer = 0;
		return returnvalue;
	}

	else if (sum > 4292967296-1)
	{
		returnvalue = *resourcepointer - (4292967296-1); //4292967296-1 used for find/replace utility if the max unsigned int size changes.
		*resourcepointer = 4292967296-1;
		return returnvalue;
	}

	else
	{
		*resourcepointer += change;
		return (0 - change);
	}

}

vector<VectorStruct> initializeResources(int depth, int length, int width, int TopsoilDepth)
{
	ResourceStruct water;
	CreateResource(water, 40, 10, 50, 5);
	ResourceStruct nitrogen;
	CreateResource(nitrogen, 40, 10, 50, 5);
	ResourceStruct phosphorus;
	CreateResource(phosphorus, 40, 10, 50, 5);
	ResourceStruct potassium;
	CreateResource(potassium, 40, 10, 50, 5);

	//creates the vector for storing variable
	vector<VectorStruct> ResourceVector;
	ResourceVector.resize(depth*width*length);

	//fills the vector
	for (int x=0; x<length; x++)
		for (int y=0; y<width; y++)
			for (int z=0; z<depth; z++)
			{
				srand (time(NULL));
				if (z>=TopsoilDepth)
				{
					ResourceVector[z+(y*depth)+(x*width*depth)].water = rand() % water.mod + water.base;
					ResourceVector[z+(y*depth)+(x*width*depth)].nitrogen = rand() % nitrogen.mod + nitrogen.base;
					ResourceVector[z+(y*depth)+(x*width*depth)].phosphorus = rand() % phosphorus.mod + phosphorus.base;
					ResourceVector[z+(y*depth)+(x*width*depth)].potassium = rand() % potassium.mod + potassium.base;
				}
				else
				{
					ResourceVector[z+(y*depth)+(x*width*depth)].water = rand() % water.modT + water.baseT;
					ResourceVector[z+(y*depth)+(x*width*depth)].nitrogen = rand() % nitrogen.modT + nitrogen.baseT;
					ResourceVector[z+(y*depth)+(x*width*depth)].phosphorus = rand() % phosphorus.modT + phosphorus.baseT;
					ResourceVector[z+(y*depth)+(x*width*depth)].potassium = rand() % potassium.modT + potassium.baseT;
				}
			}
	return ResourceVector;
}
};

#endif /* VECTORLIB_H_ */
