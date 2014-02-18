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
//for testing purposes, ignore
class TestCondition {
public:
	bool water;
	bool nitrogen;
	bool phosphorus;
	bool potassium;

	bool alltrue()
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
	long int length;
	long int width;
	long int depth;
	long int TopsoilDepth;
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

//returns the int value for the water resource at a given position
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

//returns all plantIDs at a given position
vector<string> PlantIDGrab(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	return ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID;
}

//adds a given plantID at a given position
void PlantIDAssign(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
{
	int size = ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.size();
	ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.resize(size+1);
	ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID[size] = plantID;
}

//removes a given plantID at a given position
void PlantIDRemove(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
{
	for (unsigned int i=0; i<ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.size(); i++)
			{
				if (plantID == ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID[i])
					ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.erase(ResourceVector[z+(y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width)].plantID.begin() + i);
			}
}

//checks for a given plantID at a given position; if no plantID is provided, checks for the presence of any plantIDs
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

//changes the specified resource at the specified point the specified amount, and returns the negative amount that it was changed.
long long int ResourceChange(int x, int y, int z, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string resource, signed long long int change, long long int min = 0, long long int max = UINT_MAX)
{
	signed long long int returnvalue;
	unsigned int * resourcepointer;

	//keeps max and min from breaking the function
	if (max>UINT_MAX)
		max=UINT_MAX;
	if (min<0)
		min = 0;

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

	//takes the averages of each resource at the given depth.
	long long int wateravgL = watertotalL/(DimInfo.width*DimInfo.length);
	long long int nitrogenavgL = nitrogentotalL/(DimInfo.width*DimInfo.length);
	long long int phosphorusavgL = phosphorustotalL/(DimInfo.width*DimInfo.length);
	long long int potassiumavgL = potassiumtotalL/(DimInfo.width*DimInfo.length);

	//moves resources in the direction of the average, and records changes to the mycelium resource cache.
	ResourceCache ResourceHolder;
	for (unsigned int x=0; x<DimInfo.length; x++)
	{
		for (unsigned int y=0; y<DimInfo.width; y++)
		{
			//checks for any requested change to water
			if (waterchange != 0)
			{
				unsigned int water = WaterGrab(x, y, z, DimInfo, ResourceVector);
				if (water < wateravgL) //If water in ResourceVector is less than the average, water is moved to ResourceVector from MyceliumCache by waterchange such that the water in ResourceVector does not exceed wateravgL.
				{
					ResourceHolder.water = ResourceChange(x, y, z, DimInfo, ResourceVector, "water", waterchange, 0, wateravgL);
					MyceliumCache[z].water += ResourceHolder.water;
					watertotalL -= ResourceHolder.water;
				}
				else if (water > wateravgL) //If water in ResourceVector is more than the average, water is moved to MyceliumCache from ResourceVector by waterchange such that the water in ResourceVector does not drop below wateravgL.
				{
					ResourceHolder.water = ResourceChange(x, y, z, DimInfo, ResourceVector, "water", -waterchange, wateravgL, UINT_MAX);
					MyceliumCache[z].water += ResourceHolder.water;
					watertotalL -= ResourceHolder.water;
				}

			}
			//repeats for nitrogen
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
			//repeats for phosphorus
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
			//repeats for potassium
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
		}
	}

	//determines how much of each resource Mycelium should attempt to add/remove from each space in ResourceVector at depth z
	long long int waterreplace = MyceliumCache[z].water/(DimInfo.width*DimInfo.length);
	long long int nitrogenreplace = MyceliumCache[z].nitrogen/(DimInfo.width*DimInfo.length);
	long long int phosphorusreplace = MyceliumCache[z].phosphorus/(DimInfo.width*DimInfo.length);
	long long int potassiumreplace = MyceliumCache[z].potassium/(DimInfo.width*DimInfo.length);

	//resets ResourceHolder
	ResourceHolder.water = 0;
	ResourceHolder.nitrogen = 0;
	ResourceHolder.phosphorus = 0;
	ResourceHolder.potassium = 0;

	//attempts to return balance of MyceliumCache to ResourceVector
	for (unsigned int x=0; x<DimInfo.length; x++)
	{
		for (unsigned int y=0; y<DimInfo.width; y++)
		{
			if (sign(waterreplace) == 1) //checks to see if waterreplace is positive, and chooses a maximum or minimum accordingly.
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
	//updates MyceliumCache and watertotalL
	MyceliumCache[z].water += ResourceHolder.water;
	watertotalL -= ResourceHolder.water;
	MyceliumCache[z].nitrogen += ResourceHolder.nitrogen;
	nitrogentotalL -= ResourceHolder.nitrogen;
	MyceliumCache[z].phosphorus += ResourceHolder.phosphorus;
	phosphorustotalL -= ResourceHolder.phosphorus;
	MyceliumCache[z].potassium += ResourceHolder.potassium;
	potassiumtotalL -= ResourceHolder.potassium;

	//everything else determines the return
	ResourceCache tempaverage;
	tempaverage.water = watertotalL/(DimInfo.width*DimInfo.length);
	tempaverage.nitrogen = nitrogentotalL/(DimInfo.width*DimInfo.length);
	tempaverage.phosphorus = phosphorustotalL/(DimInfo.width*DimInfo.length);
	tempaverage.potassium = potassiumtotalL/(DimInfo.width*DimInfo.length);

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

//initializes ResourceVector and fills it with resources in quantities determined by the presets
vector<VectorStruct> initializeResources(DimensionStruct DimInfo)
{
	//creates structs for holding resource presets
	ResourceStruct water;
	ResourceStruct nitrogen;
	ResourceStruct phosphorus;
	ResourceStruct potassium;
	//populates ResourceStructs with presets
	CreateResource(water, UINT_MAX/4, UINT_MAX/10, UINT_MAX/3, UINT_MAX/8);
	CreateResource(nitrogen, UINT_MAX/8, UINT_MAX/20, UINT_MAX/6, UINT_MAX/16);
	CreateResource(phosphorus, UINT_MAX/10, UINT_MAX/24, UINT_MAX/8, UINT_MAX/20);
	CreateResource(potassium, UINT_MAX/12, UINT_MAX/28, UINT_MAX/10, UINT_MAX/24);

	//creates the vector for storing resources
	vector<VectorStruct> ResourceVector;
	ResourceVector.resize(DimInfo.depth*DimInfo.width*DimInfo.length);

	//fills the vector with pseudo-random unsigned integers within the range defined by the presets
	srand (time(NULL));
	for (unsigned int x=0; x<DimInfo.length; x++)
		for (unsigned int y=0; y<DimInfo.width; y++)
			for (unsigned int z=0; z<DimInfo.depth; z++)
			{
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

//writes DimInfo, ResourceVector, and MyceliumCache to a save file
void saveresources(DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, vector<ResourceCache>& MyceliumCache)
{
	//creates save file
	ofstream savefile;
	savefile.open("savefile.txt");

	//saves DimInfo on line 0
	savefile
	<< DimInfo.length
	<< " " << DimInfo.width
	<< " " << DimInfo.depth;

	//saves MyceliumCache on lines 1 through MyceliumCache.size()
	for (unsigned int i = 0; i < MyceliumCache.size(); i++)
	{
		savefile
		<< "\n" << MyceliumCache[i].water
		<< " " << MyceliumCache[i].nitrogen
		<< " " << MyceliumCache[i].phosphorus
		<< " " << MyceliumCache[i].potassium;
	}

	//saves ResourceVector on lines MyceliumCache.size() + 1) through (ResourceVector.size() + MyceliumCache.size())
	for (unsigned int j = 0; j < ResourceVector.size(); j++)
	{
		savefile
		<< "\n" << ResourceVector[j].water
		<< " " << ResourceVector[j].nitrogen
		<< " " << ResourceVector[j].phosphorus
		<< " " << ResourceVector[j].potassium;
	}

	//closes the save file
	savefile.close();
}

void loadresources(DimensionStruct& DimInfo, vector<VectorStruct>& ResourceVector, vector<ResourceCache>& MyceliumCache)
{
	//opens the save file
	ifstream savefile;
	savefile.open("savefile.txt");

	//creates a string variable for the current line, creates a counter for determining the line number
	string line;
	unsigned long long int linecounter = 0;

	//places the contents of the save file back into DimInfo, ResourceVector and MyceliumCache
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
		for (unsigned int i = 0; i<strvalues.size(); i++)
		{
			stringstream convert(strvalues[i]);
			if ( !(convert >> intvalues[i]) )
				intvalues[i]=0;
		}
		if (linecounter > DimInfo.depth) //loads values for ResourceVector
			{
			ResourceVector[linecounter-(1+DimInfo.depth)].water = intvalues[0];
			ResourceVector[linecounter-(1+DimInfo.depth)].nitrogen = intvalues[1];
			ResourceVector[linecounter-(1+DimInfo.depth)].phosphorus = intvalues[2];
			ResourceVector[linecounter-(1+DimInfo.depth)].potassium = intvalues[3];
			}
		else if (linecounter > 0) //loads values for MyceliumCache
			{
			MyceliumCache[linecounter-1].water = intvalues [0];
			MyceliumCache[linecounter-1].nitrogen = intvalues [1];
			MyceliumCache[linecounter-1].phosphorus = intvalues [2];
			MyceliumCache[linecounter-1].potassium = intvalues [3];
			}
		else //loads values for DimInfo
			{
			DimInfo.length = intvalues[0];
			DimInfo.width = intvalues[1];
			DimInfo.depth = intvalues[2];
			}

		//updates linecounter
		linecounter += 1;
	}

	//closes the save file once all data has been read
	savefile.close();
}

long long int resourcedrop(int x, int y, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string resource, long long int change, long long int change_per_z)
{
	long long int tempresource = (-1)*change;
	//cout << "\ntempresource: " << tempresource;
	//cout << "\nchange: " << change;
	unsigned int z = 0;
	while (tempresource!=0)
	{
		if (z==DimInfo.depth)
			return -tempresource;
		else
		{
			if (change>0)
			{
				if (resource == "water")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, 0, WaterGrab(x, y, z, DimInfo, ResourceVector)+change_per_z);
				else if (resource == "nitrogen")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, 0, NitrogenGrab(x, y, z, DimInfo, ResourceVector)+change_per_z);
				else if (resource == "phosphorus")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, 0, PhosphorusGrab(x, y, z, DimInfo, ResourceVector)+change_per_z);
				else if (resource == "potassium")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, 0, PotassiumGrab(x, y, z, DimInfo, ResourceVector)+change_per_z);
			}
			else if (change<0)
			{
				if (resource == "water")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, WaterGrab(x, y, z, DimInfo, ResourceVector)-change_per_z, UINT_MAX);
				else if (resource == "nitrogen")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, NitrogenGrab(x, y, z, DimInfo, ResourceVector)-change_per_z, UINT_MAX);
				else if (resource == "phosphorus")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, PhosphorusGrab(x, y, z, DimInfo, ResourceVector)-change_per_z, UINT_MAX);
				else if (resource == "potassium")
					tempresource -= ResourceChange(x, y, z, DimInfo, ResourceVector, resource, -tempresource, PotassiumGrab(x, y, z, DimInfo, ResourceVector)-change_per_z, UINT_MAX);
			}
			z+=1;
		}
	}
	return 0;
}

};

#endif /* VECTORLIB_H_ */
