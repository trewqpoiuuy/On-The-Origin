/*
 * VectorLib.h
 *
 *  Created on: Jan 16, 2014
 *      Author: WBoettcher
 */

#ifndef VECTORLIB_H_
#define VECTORLIB_H_
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <limits.h>

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

struct ResourceCache {
	long long int water;
	long long int nitrogen;
	long long int phosphorus;
	long long int potassium;
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

long long int absdiff(long long int a, long long int b = 0)
{
	if ((a-b) > 0)
		return (a-b);
	else
		return (b-a);
}

int sign(long long int value)
{
	if (value >= 0)
		return 1;
	else
		return -1;
}

unsigned int WaterGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].water;
}

//returns the int value for the nitrogen resource at a given position
unsigned int NitrogenGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].nitrogen;
}

//returns the int value for the phosphorus resource at a given position
unsigned int PhosphorusGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].phosphorus;
}

//returns the int value for the potassium resource at a given position
unsigned int PotassiumGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].potassium;
}

string PlantIDGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID;
}

void PlantIDAssign(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
{
	ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID = plantID;
}

bool PlantIDCheck(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID = " ")
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
int ResourceChange(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string resource, signed long long int change, unsigned int min = 0, unsigned int max = UINT_MAX)
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

	if (sum < min)
	{
		returnvalue = *resourcepointer - min;
		*resourcepointer = min;
		return returnvalue;
	}

	else if (sum > max)
	{
		returnvalue = *resourcepointer - max;
		*resourcepointer = max;
		return returnvalue;
	}

	else
	{
		*resourcepointer += change;
		return -change;
	}

}

//spreads out resources at a given depth to the degree specified for each resource.
vector<ResourceCache> Mycelium(int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, vector<ResourceCache>& MyceliumCache,
		signed long long int waterchange,
		signed long long int nitrogenchange,
		signed long long int phosphoruschange,
		signed long long int potassiumchange
		)
{
	//finds the total value for each resource at a given depth.
	unsigned long long int watertotal = 0;
	unsigned long long int nitrogentotal = 0;
	unsigned long long int phosphorustotal = 0;
	unsigned long long int potassiumtotal = 0;
	for (int x=0; x<DimInfo.length; x++)
	{
		for (int y=0; y<DimInfo.width; y++)
		{
			signed long long int nonZ = (y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width);
			watertotal += ResourceVector[z+nonZ].water;
			nitrogentotal += ResourceVector[z+nonZ].nitrogen;
			phosphorustotal += ResourceVector[z+nonZ].phosphorus;
			potassiumtotal += ResourceVector[z+nonZ].potassium;
		}
	}

	//takes the averages of each resource per layer
	long long int wateravg = watertotal/(DimInfo.width*DimInfo.length);
	long long int nitrogenavg = nitrogentotal/(DimInfo.width*DimInfo.length);
	long long int phosphorusavg = phosphorustotal/(DimInfo.width*DimInfo.length);
	long long int potassiumavg = potassiumtotal/(DimInfo.width*DimInfo.length);

	//moves resources in the direction of the average, and records changes to the mycelium resource cache.

	ResourceCache ResourceHolder;

	for (int x=0; x<DimInfo.length; x++)
	{
		for (int y=0; y<DimInfo.width; y++)
		{
			if (waterchange != 0)
			{
				unsigned int water = WaterGrab(x, y, z, DimInfo, ResourceVector);
				if (water < wateravg)
				{
					ResourceHolder.water = ResourceChange(x, y, z, DimInfo, ResourceVector, "water", waterchange, 0, wateravg);
					MyceliumCache[z].water += ResourceHolder.water;
					watertotal -= ResourceHolder.water;
				}
				else if (water > wateravg)
				{
					ResourceHolder.water = ResourceChange(x, y, z, DimInfo, ResourceVector, "water", -waterchange, wateravg, UINT_MAX);
					MyceliumCache[z].water += ResourceHolder.water;
					watertotal -= ResourceHolder.water;
				}

			}

			if (nitrogenchange != 0)
			{
				unsigned int nitrogen = NitrogenGrab(x, y, z, DimInfo, ResourceVector);
				if (nitrogen < nitrogenavg)
				{
					ResourceHolder.nitrogen = ResourceChange(x, y, z, DimInfo, ResourceVector, "nitrogen", nitrogenchange, 0, nitrogenavg);
					MyceliumCache[z].nitrogen += ResourceHolder.nitrogen;
					nitrogentotal -= ResourceHolder.nitrogen;
				}
				else if (nitrogen > nitrogenavg)
				{
					ResourceHolder.nitrogen = ResourceChange(x, y, z, DimInfo, ResourceVector, "nitrogen", -nitrogenchange, nitrogenavg, UINT_MAX);
					MyceliumCache[z].nitrogen += ResourceHolder.nitrogen;
					nitrogentotal -= ResourceHolder.nitrogen;
				}
			}

			if (phosphoruschange != 0)
			{
				unsigned int phosphorus = PhosphorusGrab(x, y, z, DimInfo, ResourceVector);
				if (phosphorus < phosphorusavg)
				{
					ResourceHolder.phosphorus = ResourceChange(x, y, z, DimInfo, ResourceVector, "phosphorus", phosphoruschange, 0, phosphorusavg);
					MyceliumCache[z].phosphorus += ResourceHolder.phosphorus;
					phosphorustotal -= ResourceHolder.phosphorus;
				}
				else if (phosphorus > phosphorusavg)
				{
					ResourceHolder.phosphorus = ResourceChange(x, y, z, DimInfo, ResourceVector, "phosphorus", -phosphoruschange, phosphorusavg, UINT_MAX);
					MyceliumCache[z].phosphorus += ResourceHolder.phosphorus;
					phosphorustotal -= ResourceHolder.phosphorus;
				}
			}

			if (potassiumchange != 0)
			{
				unsigned int potassium = PotassiumGrab(x, y, z, DimInfo, ResourceVector);
				if (potassium < potassiumavg)
				{
					ResourceHolder.potassium = ResourceChange(x, y, z, DimInfo, ResourceVector, "potassium", potassiumchange, 0, potassiumavg);
					MyceliumCache[z].potassium += ResourceHolder.potassium;
					potassiumtotal -= ResourceHolder.potassium;
				}
				else if (potassium > potassiumavg)
				{
					ResourceHolder.potassium = ResourceChange(x, y, z, DimInfo, ResourceVector, "potassium", -potassiumchange, potassiumavg, UINT_MAX);
					MyceliumCache[z].potassium += ResourceHolder.potassium;
					potassiumtotal -= ResourceHolder.potassium;
				}
			}

		}
	}

	long long int waterreplace = MyceliumCache[z].water/(DimInfo.width*DimInfo.length);
	long long int nitrogenreplace = MyceliumCache[z].nitrogen/(DimInfo.width*DimInfo.length);
	long long int phosphorusreplace = MyceliumCache[z].phosphorus/(DimInfo.width*DimInfo.length);
	long long int potassiumreplace = MyceliumCache[z].potassium/(DimInfo.width*DimInfo.length);

	ResourceHolder.water = 0;
	ResourceHolder.nitrogen = 0;
	ResourceHolder.phosphorus = 0;
	ResourceHolder.potassium = 0;

	for (int x=0; x<DimInfo.length; x++)
	{
		for (int y=0; y<DimInfo.width; y++)
		{
			ResourceHolder.water += ResourceChange(x, y, z, DimInfo, ResourceVector, "water", waterreplace, 0, wateravg);
			ResourceHolder.nitrogen += ResourceChange(x, y, z, DimInfo, ResourceVector, "nitrogen", nitrogenreplace, 0, nitrogenavg);
			ResourceHolder.phosphorus += ResourceChange(x, y, z, DimInfo, ResourceVector, "phosphorus", phosphorusreplace, 0, phosphorusavg);
			ResourceHolder.potassium += ResourceChange(x, y, z, DimInfo, ResourceVector, "potassium", potassiumreplace, 0, potassiumavg);
		}
	}

	MyceliumCache[z].water += ResourceHolder.water;
	watertotal -= ResourceHolder.water;

	MyceliumCache[z].nitrogen += ResourceHolder.nitrogen;
	nitrogentotal -= ResourceHolder.nitrogen;
	MyceliumCache[z].phosphorus += ResourceHolder.phosphorus;
	phosphorustotal -= ResourceHolder.phosphorus;
	MyceliumCache[z].potassium += ResourceHolder.potassium;
	potassiumtotal -= ResourceHolder.potassium;

	ResourceCache tempaverage;

	tempaverage.water = watertotal/(DimInfo.width*DimInfo.length);
	tempaverage.nitrogen = nitrogentotal/(DimInfo.width*DimInfo.length);
	tempaverage.phosphorus = phosphorustotal/(DimInfo.width*DimInfo.length);
	tempaverage.potassium = potassiumtotal/(DimInfo.width*DimInfo.length);

/*
	tempaverage.water = wateravg;
	tempaverage.nitrogen = nitrogenavg;
	tempaverage.phosphorus = phosphorusavg;
	tempaverage.potassium = potassiumavg;
*/

	ResourceCache temptotal;
	temptotal.water = watertotal;
	temptotal.nitrogen = nitrogentotal;
	temptotal.phosphorus = phosphorustotal;
	temptotal.potassium = potassiumtotal;

	vector<ResourceCache> MycoOutput(2);
	MycoOutput[0] = tempaverage;
	MycoOutput[1] = temptotal;

	return MycoOutput;
}

//initializes and fills the mycelium resource cache
vector<ResourceCache> initializeMycelium(DimensionStruct DimInfo)
	{
		vector<ResourceCache> MyceliumCache;
		MyceliumCache.resize(DimInfo.depth);

		for (int z=0; z<DimInfo.depth; z++)
		{
			MyceliumCache[z].water = 0;
			MyceliumCache[z].nitrogen = 0;
			MyceliumCache[z].phosphorus = 0;
			MyceliumCache[z].potassium = 0;
		}
		return MyceliumCache;
	}

vector<VectorStruct> initializeResources(DimensionStruct DimInfo)
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
	ResourceVector.resize(DimInfo.depth*DimInfo.width*DimInfo.length);

	//fills the vector
	for (int x=0; x<DimInfo.length; x++)
		for (int y=0; y<DimInfo.width; y++)
			for (int z=0; z<DimInfo.depth; z++)
			{
				srand (time(NULL));

				unsigned int xyz = (z+(y*DimInfo.depth)+(x*DimInfo.width*DimInfo.depth));

				if (z>=DimInfo.TopsoilDepth)
				{
					ResourceVector[xyz].water = rand() % water.mod + water.base;
					ResourceVector[xyz].nitrogen = rand() % nitrogen.mod + nitrogen.base;
					ResourceVector[xyz].phosphorus = rand() % phosphorus.mod + phosphorus.base;
					ResourceVector[xyz].potassium = rand() % potassium.mod + potassium.base;
				}
				else
				{
					ResourceVector[xyz].water = rand() % water.modT + water.baseT;
					ResourceVector[xyz].nitrogen = rand() % nitrogen.modT + nitrogen.baseT;
					ResourceVector[xyz].phosphorus = rand() % phosphorus.modT + phosphorus.baseT;
					ResourceVector[xyz].potassium = rand() % potassium.modT + potassium.baseT;
				}
			}
	return ResourceVector;

}
};

#endif /* VECTORLIB_H_ */
