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
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <limits.h>
#include <algorithm>
#include <iterator>

using namespace std;
namespace veclib
{
//Creates a structure for resources for use as arguments

//for testing purposes, ignore
class TestCondition {
public:

	bool water;
	bool nitrogen;
	bool phosphorus;
	bool potassium;

	int alltrue()
	{
		int i = water+nitrogen+phosphorus+potassium;
		if (i == 4)
			return true;
		else
			return false;
	}

	void clear()
	{
		water = false;
		nitrogen = false;
		phosphorus = false;
		potassium = false;
	}
};

//for storing base values for each resource pre-generation
struct ResourceStruct {
	long long int base;
	long long int mod;
	long long int baseT;
	long long int modT;
};

//for holding resource values in the resource vector
struct VectorStruct {
	unsigned int water;
	unsigned int nitrogen;
	unsigned int phosphorus;
	unsigned int potassium;
	vector<string> plantID;
};

//for holding dimensional information for the resource vector
struct DimensionStruct {
	unsigned int length;
	unsigned int width;
	unsigned int depth;
	unsigned int TopsoilDepth;
};

//for holding resources outside of soil, such as in Mycelium
struct ResourceCache {
	signed long long int water;
	signed long long int nitrogen;
	signed long long int phosphorus;
	signed long long int potassium;
};

//Fills ResourceStructs water, nitrogen, phosphorus and potassium with variables (which must be defined before this function is executed)
void CreateResource(ResourceStruct& TypeName, long long int Base, long long int Mod, long long int BaseT, long long int ModT)
	{
		TypeName.base = Base;
		TypeName.baseT = BaseT;
		TypeName.mod = Mod;
		TypeName.modT = ModT;
	}

//takes the absolute difference of 2 values.
long long int absdiff(long long int a, long long int b = 0)
{
	if ((a-b) > 0)
		return (a-b);
	else
		return (b-a);
}

//returns the sign of a value (1 or -1)
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

vector<string> PlantIDGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID;
}

void PlantIDAssign(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
{
	int size = ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.size();
	ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.resize(size+1);
	ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID[size] = plantID;
}

void PlantIDRemove(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
{
	for (unsigned int i=0; i<ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.size(); i++)
			{
				if (plantID == ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID[i])
					ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.erase(ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.begin() + i);
			}
}

bool PlantIDCheck(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID = " ")
{
	if (ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.size() == 0)
		return false;
	else
	{
		if (plantID == " ")
			return true;
		for (unsigned int i=0; i<ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.size(); i++)
		{
			if (plantID == ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID[i])
				return true;
		}

		if (plantID != " ")
			return false;
	}
	cout << "\nError, PlantIDCheck broken.";
	return false;
}

//changes the specified resource at the specified point the specified amount, and returns the amount that it was changed.
long long int ResourceChange(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string resource, signed long long int change, long long int min = 0, long long int max = UINT_MAX)
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

	signed long long int sum = (*resourcepointer)+change;

	//cout << "\n\nsum: " << sum;
	//cout << "\nchange: " << change;
	//cout << "\nresource: " << *resourcepointer;
	if (sum < min)
	{
		returnvalue = *resourcepointer - min;
		*resourcepointer = min;
		//cout << "\nsum < min, return: " << returnvalue;
		return returnvalue;
	}

	else if (sum > max)
	{
		returnvalue = *resourcepointer - max;
		*resourcepointer = max;
		//cout << "\nsum > min, return: " << returnvalue;
		return returnvalue;
	}

	else
	{
		*resourcepointer += change;
		//cout << "\nmin<sum<max, return: " << -change;
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
	unsigned long long int watertotalL = 0;
	unsigned long long int nitrogentotalL = 0;
	unsigned long long int phosphorustotalL = 0;
	unsigned long long int potassiumtotalL = 0;
	for (unsigned int x=0; x<DimInfo.length; x++)
	{
		for (unsigned int y=0; y<DimInfo.width; y++)
		{
			signed long long int nonZ = (y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width);
			watertotalL += ResourceVector[z+nonZ].water;
			nitrogentotalL += ResourceVector[z+nonZ].nitrogen;
			phosphorustotalL += ResourceVector[z+nonZ].phosphorus;
			potassiumtotalL += ResourceVector[z+nonZ].potassium;
		}
	}

	//takes the averages of each resource per layer
	long long int wateravgL = watertotalL/(DimInfo.width*DimInfo.length);
	long long int nitrogenavgL = nitrogentotalL/(DimInfo.width*DimInfo.length);
	long long int phosphorusavgL = phosphorustotalL/(DimInfo.width*DimInfo.length);
	long long int potassiumavgL = potassiumtotalL/(DimInfo.width*DimInfo.length);

	//moves resources in the direction of the average, and records changes to the mycelium resource cache.

	ResourceCache ResourceHolder;
/*
	ResourceCache MyceliumCacheChange;
	MyceliumCacheChange.water = MyceliumCache[z].water;
	MyceliumCacheChange.nitrogen = MyceliumCache[z].nitrogen;
	MyceliumCacheChange.phosphorus = MyceliumCache[z].phosphorus;
	MyceliumCacheChange.potassium = MyceliumCache[z].potassium;
*/
	for (unsigned int x=0; x<DimInfo.length; x++)
	{
		for (unsigned int y=0; y<DimInfo.width; y++)
		{
			if (waterchange != 0)
			{
				unsigned int water = WaterGrab(x, y, z, DimInfo, ResourceVector);
				if (water < wateravgL)
				{
					ResourceHolder.water = ResourceChange(x, y, z, DimInfo, ResourceVector, "water", waterchange, 0, wateravgL);
					MyceliumCache[z].water += ResourceHolder.water;
					watertotalL -= ResourceHolder.water;
				}
				else if (water > wateravgL)
				{
					ResourceHolder.water = ResourceChange(x, y, z, DimInfo, ResourceVector, "water", -waterchange, wateravgL, UINT_MAX);
					MyceliumCache[z].water += ResourceHolder.water;
					watertotalL -= ResourceHolder.water;
				}

			}

			if (nitrogenchange != 0)
			{
				unsigned int nitrogen = NitrogenGrab(x, y, z, DimInfo, ResourceVector);
				if (nitrogen < nitrogenavgL)
				{
					ResourceHolder.nitrogen = ResourceChange(x, y, z, DimInfo, ResourceVector, "nitrogen", nitrogenchange, 0, nitrogenavgL);
					MyceliumCache[z].nitrogen += ResourceHolder.nitrogen;
					nitrogentotalL -= ResourceHolder.nitrogen;
				}
				else if (nitrogen > nitrogenavgL)
				{
					ResourceHolder.nitrogen = ResourceChange(x, y, z, DimInfo, ResourceVector, "nitrogen", -nitrogenchange, nitrogenavgL, UINT_MAX);
					MyceliumCache[z].nitrogen += ResourceHolder.nitrogen;
					nitrogentotalL -= ResourceHolder.nitrogen;
				}
			}

			if (phosphoruschange != 0)
			{
				unsigned int phosphorus = PhosphorusGrab(x, y, z, DimInfo, ResourceVector);
				if (phosphorus < phosphorusavgL)
				{
					ResourceHolder.phosphorus = ResourceChange(x, y, z, DimInfo, ResourceVector, "phosphorus", phosphoruschange, 0, phosphorusavgL);
					MyceliumCache[z].phosphorus += ResourceHolder.phosphorus;
					phosphorustotalL -= ResourceHolder.phosphorus;
				}
				else if (phosphorus > phosphorusavgL)
				{
					ResourceHolder.phosphorus = ResourceChange(x, y, z, DimInfo, ResourceVector, "phosphorus", -phosphoruschange, phosphorusavgL, UINT_MAX);
					MyceliumCache[z].phosphorus += ResourceHolder.phosphorus;
					phosphorustotalL -= ResourceHolder.phosphorus;
				}
			}

			if (potassiumchange != 0)
			{
				unsigned int potassium = PotassiumGrab(x, y, z, DimInfo, ResourceVector);
				if (potassium < potassiumavgL)
				{
					ResourceHolder.potassium = ResourceChange(x, y, z, DimInfo, ResourceVector, "potassium", potassiumchange, 0, potassiumavgL);
					MyceliumCache[z].potassium += ResourceHolder.potassium;
					potassiumtotalL -= ResourceHolder.potassium;
				}
				else if (potassium > potassiumavgL)
				{
					ResourceHolder.potassium = ResourceChange(x, y, z, DimInfo, ResourceVector, "potassium", -potassiumchange, potassiumavgL, UINT_MAX);
					MyceliumCache[z].potassium += ResourceHolder.potassium;
					potassiumtotalL -= ResourceHolder.potassium;
				}
			}
/*
			int coutOn = 0;

			if (coutOn == 1)
			{
				cout << "\n\nw/n/p/k change: "
						<< waterchange << ", "
						<< nitrogenchange << ", "
						<< phosphoruschange << ", "
						<< potassiumchange;
				cout << "\nw/n/p/k total: "
						<< watertotalL << ", "
						<< nitrogentotalL << ", "
						<< phosphorustotalL << ", "
						<< potassiumtotalL;
				cout << "\nw/n/p/k avg: "
						<< wateravgL << ", "
						<< nitrogenavgL << ", "
						<< phosphorusavgL << ", "
						<< potassiumavgL;
				cout << "\nw/n/p/k MycoCache: "
						<< MyceliumCache[z].water << ", "
						<< MyceliumCache[z].nitrogen << ", "
						<< MyceliumCache[z].phosphorus << ", "
						<< MyceliumCache[z].potassium;
				cout << "\nw/n/p/k MycoChange: "
						<< MyceliumCache[z].water - MyceliumCacheChange.water << ", "
						<< MyceliumCache[z].nitrogen - MyceliumCacheChange.nitrogen << ", "
						<< MyceliumCache[z].phosphorus - MyceliumCacheChange.phosphorus << ", "
						<< MyceliumCache[z].potassium - MyceliumCacheChange.potassium;
			}
*/
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

	for (unsigned int x=0; x<DimInfo.length; x++)
	{
		for (unsigned int y=0; y<DimInfo.width; y++)
		{
			if (sign(waterreplace) == 1)
				ResourceHolder.water += ResourceChange(x, y, z, DimInfo, ResourceVector, "water", waterreplace, 0, wateravgL);
			else
				ResourceHolder.water += ResourceChange(x, y, z, DimInfo, ResourceVector, "water", waterreplace, wateravgL, UINT_MAX);

			if (sign(nitrogenreplace) == 1)
				ResourceHolder.nitrogen += ResourceChange(x, y, z, DimInfo, ResourceVector, "nitrogen", nitrogenreplace, 0, nitrogenavgL);
			else
				ResourceHolder.nitrogen += ResourceChange(x, y, z, DimInfo, ResourceVector, "nitrogen", nitrogenreplace, nitrogenavgL, UINT_MAX);

			if (sign(phosphorusreplace) == 1)
				ResourceHolder.phosphorus += ResourceChange(x, y, z, DimInfo, ResourceVector, "phosphorus", phosphorusreplace, 0, phosphorusavgL);
			else
				ResourceHolder.phosphorus += ResourceChange(x, y, z, DimInfo, ResourceVector, "phosphorus", phosphorusreplace, phosphorusavgL, UINT_MAX);

			if (sign(potassiumreplace) == 1)
				ResourceHolder.potassium += ResourceChange(x, y, z, DimInfo, ResourceVector, "potassium", potassiumreplace, 0, potassiumavgL);
			else
				ResourceHolder.potassium += ResourceChange(x, y, z, DimInfo, ResourceVector, "potassium", potassiumreplace, potassiumavgL, UINT_MAX);
		}
	}

	MyceliumCache[z].water += ResourceHolder.water;
	watertotalL -= ResourceHolder.water;

	MyceliumCache[z].nitrogen += ResourceHolder.nitrogen;
	nitrogentotalL -= ResourceHolder.nitrogen;
	MyceliumCache[z].phosphorus += ResourceHolder.phosphorus;
	phosphorustotalL -= ResourceHolder.phosphorus;
	MyceliumCache[z].potassium += ResourceHolder.potassium;
	potassiumtotalL -= ResourceHolder.potassium;

	ResourceCache tempaverage;

	tempaverage.water = watertotalL/(DimInfo.width*DimInfo.length);
	tempaverage.nitrogen = nitrogentotalL/(DimInfo.width*DimInfo.length);
	tempaverage.phosphorus = phosphorustotalL/(DimInfo.width*DimInfo.length);
	tempaverage.potassium = potassiumtotalL/(DimInfo.width*DimInfo.length);

/*
	tempaverage.water = wateravg;
	tempaverage.nitrogen = nitrogenavg;
	tempaverage.phosphorus = phosphorusavg;
	tempaverage.potassium = potassiumavg;
*/

	ResourceCache temptotal;
	temptotal.water = watertotalL;
	temptotal.nitrogen = nitrogentotalL;
	temptotal.phosphorus = phosphorustotalL;
	temptotal.potassium = potassiumtotalL;

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

		for (unsigned int z=0; z<DimInfo.depth; z++)
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
	int RecBaseSet = 0;

	ResourceStruct water;
	CreateResource(water, 0, 0, 0, 0);
	ResourceStruct nitrogen;
	CreateResource(nitrogen, 0, 0, 0, 0);
	ResourceStruct phosphorus;
	CreateResource(phosphorus, 0, 0, 0, 0);
	ResourceStruct potassium;
	CreateResource(potassium, 0, 0, 0, 0);

	if (RecBaseSet == 0)
	{
		CreateResource(water, UINT_MAX/4, UINT_MAX/10, UINT_MAX/3, UINT_MAX/8);
		CreateResource(nitrogen, UINT_MAX/8, UINT_MAX/20, UINT_MAX/6, UINT_MAX/16);
		CreateResource(phosphorus, UINT_MAX/10, UINT_MAX/24, UINT_MAX/8, UINT_MAX/20);
		CreateResource(potassium, UINT_MAX/12, UINT_MAX/28, UINT_MAX/10, UINT_MAX/24);
	}

	else if (RecBaseSet == 1)
	{
		CreateResource(water, UINT_MAX/4, UINT_MAX/10, UINT_MAX/3, UINT_MAX/8);
		CreateResource(nitrogen, UINT_MAX/4, UINT_MAX/10, UINT_MAX/3, UINT_MAX/8);
		CreateResource(phosphorus, UINT_MAX/4, UINT_MAX/10, UINT_MAX/3, UINT_MAX/8);
		CreateResource(potassium, UINT_MAX/4, UINT_MAX/10, UINT_MAX/3, UINT_MAX/8);
	}

	//creates the vector for storing variable
	vector<VectorStruct> ResourceVector;
	ResourceVector.resize(DimInfo.depth*DimInfo.width*DimInfo.length);

	//fills the vector
	for (unsigned int x=0; x<DimInfo.length; x++)
		for (unsigned int y=0; y<DimInfo.width; y++)
			for (unsigned int z=0; z<DimInfo.depth; z++)
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

//for writing/reading files
void saveresources(DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, vector<ResourceCache>& MyceliumCache)
{
	ofstream savefile;
	savefile.open("savefile.txt");
/*
	savefile
	<< "length: " << DimInfo.length
	<< "\nwidth: " << DimInfo.width
	<< "\ndepth: " << DimInfo.depth
	<< "\n";

	savefile << "\nMycoCache";
	for (unsigned int i = 0; i < MyceliumCache.size(); i++)
	{
		savefile
		<< "\nwater[" << i << "]: " << MyceliumCache[i].water
		<< "\nnitrogen[" << i << "]: " << MyceliumCache[i].nitrogen
		<< "\nphosphorus[" << i << "]: " << MyceliumCache[i].phosphorus
		<< "\npotassium[" << i << "]: " << MyceliumCache[i].potassium;
	}

	savefile << "\n\nResourceVector";
	for (unsigned int j = 0; j < ResourceVector.size(); j++)
	{
		savefile
		<< "\nwater[" << j << "]: " << ResourceVector[j].water
		<< "\nnitrogen[" << j << "]: " <<ResourceVector[j].nitrogen
		<< "\nphosphorus[" << j << "]: " << ResourceVector[j].phosphorus
		<< "\npotassium[" << j << "]: " << ResourceVector[j].potassium;
	}
*/
	savefile
	<< DimInfo.length
	<< " " << DimInfo.width
	<< " " << DimInfo.depth;

	for (unsigned int i = 0; i < MyceliumCache.size(); i++)
	{
		savefile
		<< "\n" << MyceliumCache[i].water
		<< " " << MyceliumCache[i].nitrogen
		<< " " << MyceliumCache[i].phosphorus
		<< " " << MyceliumCache[i].potassium;
	}

	for (unsigned int j = 0; j < ResourceVector.size(); j++)
	{
		savefile
		<< "\n" << ResourceVector[j].water
		<< " " << ResourceVector[j].nitrogen
		<< " " << ResourceVector[j].phosphorus
		<< " " << ResourceVector[j].potassium;
	}

	savefile.close();
}

void loadresources(DimensionStruct& DimInfo, vector<VectorStruct>& ResourceVector, vector<ResourceCache>& MyceliumCache)
{
	ifstream savefile;
	savefile.open("savefile.txt");
	string line;
	unsigned long long int linecounter = 0;

	while(getline(savefile, line))
	{
		//breaks each line into its component values, places in vector strvalues
		istringstream iss(line);
		vector<string> strvalues;
		copy(istream_iterator<string>(iss),
		         istream_iterator<string>(),
		         back_inserter<vector<string> >(strvalues));

		//converts strings in vector strvalues to integers in vector intvalues
		vector<long long int> intvalues(strvalues.size());
/*
		cout << "\nstrvalues.size(): " << strvalues.size() << " contents: ";
			for (unsigned int j=0; j<strvalues.size(); j++)
				cout << strvalues[j] << ", ";
*/

		for (unsigned int i = 0; i<strvalues.size(); i++)
		{
			stringstream convert(strvalues[i]);
			if ( !(convert >> intvalues[i]) )
				intvalues[i]=0;
		}
		if (linecounter > DimInfo.depth)
			{
			ResourceVector[linecounter-(1+DimInfo.depth)].water = intvalues[0];
			ResourceVector[linecounter-(1+DimInfo.depth)].nitrogen = intvalues[1];
			ResourceVector[linecounter-(1+DimInfo.depth)].phosphorus = intvalues[2];
			ResourceVector[linecounter-(1+DimInfo.depth)].potassium = intvalues[3];
			}

		else if (linecounter > 0)
			{
			MyceliumCache[linecounter-1].water = intvalues [0];
			MyceliumCache[linecounter-1].nitrogen = intvalues [1];
			MyceliumCache[linecounter-1].phosphorus = intvalues [2];
			MyceliumCache[linecounter-1].potassium = intvalues [3];
			}
		else
			{
			DimInfo.length = intvalues[0];
			DimInfo.width = intvalues[1];
			DimInfo.depth = intvalues[2];
			}

		linecounter+=1;
	}

	savefile.close();
}

};

#endif /* VECTORLIB_H_ */
