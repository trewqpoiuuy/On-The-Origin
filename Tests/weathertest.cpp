/*
 * weather.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: WBoettcher
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include "weather.h"
#include "VectorIntLib.h"
using namespace std;
using namespace weather;
int main()
{
	cout << "Start\n";

	ResourceCache atmosphere = initializeweather(rain, sun, wind);

/*
	Weather rain;
	rain.halflife = 30;
	rain.intensity_max = 10000;
	rain.intensitychange_max = 100;

	Weather wind;
	wind.halflife = 100;
	wind.intensity_max = 1000;
	wind.intensitychange_max = 10;
	wind.directionchange_max = 1;

	Weather sun;
	sun.cyclereset = 24;
	sun.intensity_max = 1100;
	sun.intensity_min = 900;
	sun.intensitychange_max = 10;
*/
	for(unsigned int i = 0; i<300; i++)
	{
		rain.cycle();

		cout << "\nrain e|i|i_t: "
		<< rain.exists
		<< "|" << rain.intensity
		<< "|" << rain.intensity_target;
	}

	for(unsigned int i = 0; i<300; i++)
	{
		wind.cycle();

		cout << "\nwind e|d|d_t|i|i_t: "
		<< wind.exists
		<< "|" << wind.direction
		<< "|" << wind.direction_target
		<< "|" << wind.intensity
		<< "|" << wind.intensity_target;
	}

	for(unsigned int i = 0; i<300; i++)
	{
		sun.cycle();

		cout << "\nsun e|c|i|i_t: "
		<< sun.exists
		<< "|" << sun.cyclecounter
		<< "|" << sun.intensity
		<< "|" << sun.intensity_target;
	}
	cout <<"\nRAND_MAX: " << RAND_MAX;

	//testing weather functions on resources
/*
	rain.halflife = 0;
	rain.exists = true;
	rain.intensity = 6000;
	rain.intensity_target = 10000;
*/
	//determines the depth, length, and width of the block of soil contained by the vector, also determines the depth of the topsoil.
	int depth = 10;
	int length = 100;
	int width = 100;
	int TopsoilDepth = 3;

	//initializes and fills the object for storing dimensional information about the vector
	DimensionStruct DimInfo;
	DimInfo.length = length;
	DimInfo.width = width;
	DimInfo.depth = depth;
	DimInfo.TopsoilDepth = TopsoilDepth;

	//initializes the vector
	vector<VectorStruct> ResourceVector=initializeResources(DimInfo);

	//generates random coordinates for testing
	int xtest = rand () % length;
	int ytest = rand () % width;
	int ztest = rand () % depth;

	//finds the total amount of water at (xtest, ytest)
	ResourceCache initial;
	ResourceCache current;
	vector<ResourceCache> cache_new;
	cache_new.resize(DimInfo.depth);
	vector<ResourceCache> cache_old;
	cache_old.resize(DimInfo.depth);
	initial.water = 0;
	initial.nitrogen = 0;
	initial.phosphorus = 0;
	initial.potassium = 0;

	for (unsigned int z = 0; z<DimInfo.depth; z++)
	{
		initial.water += WaterGrab(xtest, ytest, z, DimInfo, ResourceVector);
		initial.nitrogen += NitrogenGrab(xtest, ytest, z, DimInfo, ResourceVector);
		initial.phosphorus += PhosphorusGrab(xtest, ytest, z, DimInfo, ResourceVector);
		initial.potassium += PotassiumGrab(xtest, ytest, z, DimInfo, ResourceVector);
	}

	//saves the initial value of atmosphere.water
	long long int atmosphericwater_initial = atmosphere.water;

	//prints reference values
	cout << "\n\nreference values (pre-rain)";
	cout << "\nrain e|i|i_t: "
		<< rain.exists
		<< "|" << rain.intensity
		<< "|" << rain.intensity_target;
	current.water = 0;
	current.nitrogen = 0;
	current.phosphorus = 0;
	current.potassium = 0;
	for (unsigned int z = 0; z<DimInfo.depth; z++)
	{
		current.water += WaterGrab(xtest, ytest, z, DimInfo, ResourceVector);
		current.nitrogen += NitrogenGrab(xtest, ytest, z, DimInfo, ResourceVector);
		current.phosphorus += PhosphorusGrab(xtest, ytest, z, DimInfo, ResourceVector);
		current.potassium += PotassiumGrab(xtest, ytest, z, DimInfo, ResourceVector);
		cache_old[z].water = WaterGrab(xtest, ytest, z, DimInfo, ResourceVector);
		cache_old[z].nitrogen = NitrogenGrab(xtest, ytest, z, DimInfo, ResourceVector);
		cache_old[z].phosphorus = PhosphorusGrab(xtest, ytest, z, DimInfo, ResourceVector);
		cache_old[z].potassium = PotassiumGrab(xtest, ytest, z, DimInfo, ResourceVector);
		cout << "\nw/n/p/k at (" << xtest << ", " << ytest << ", " << z << "): "
				<< WaterGrab(xtest, ytest, z, DimInfo, ResourceVector)
				<< ", " << NitrogenGrab(xtest, ytest, z, DimInfo, ResourceVector)
				<< ", " << PhosphorusGrab(xtest, ytest, z, DimInfo, ResourceVector)
				<< ", " << PotassiumGrab(xtest, ytest, z, DimInfo, ResourceVector);
	}
	cout << "\natmosphere.water: " << atmosphere.water;

	//runs rainfall with rain cycling
	for(unsigned int i = 0; i<100; i++)
	{
		cout << "\n\niteration " << i;
		rain.cycle();
		cout << "\nrain e|i|i_t: "
			<< rain.exists
			<< "|" << rain.intensity
			<< "|" << rain.intensity_target;
		sun.cycle();
		cout << "\nsun e|c|i|i_t: "
		<< sun.exists
		<< "|" << sun.cyclecounter
		<< "|" << sun.intensity
		<< "|" << sun.intensity_target;
		sunshine(DimInfo, ResourceVector, sun, atmosphere);
//		if (atmosphere.water >= (rain.intensity*DimInfo.width*DimInfo.length))
//		{
			rainfall(DimInfo, ResourceVector, rain, atmosphere);
			current.water = 0;
			current.nitrogen = 0;
			current.phosphorus = 0;
			current.potassium = 0;
			for (unsigned int z = 0; z<DimInfo.depth; z++)
			{
				current.water += WaterGrab(xtest, ytest, z, DimInfo, ResourceVector);
				current.nitrogen += NitrogenGrab(xtest, ytest, z, DimInfo, ResourceVector);
				current.phosphorus += PhosphorusGrab(xtest, ytest, z, DimInfo, ResourceVector);
				current.potassium += PotassiumGrab(xtest, ytest, z, DimInfo, ResourceVector);
				cache_new[z].water = WaterGrab(xtest, ytest, z, DimInfo, ResourceVector);
				cache_new[z].nitrogen = NitrogenGrab(xtest, ytest, z, DimInfo, ResourceVector);
				cache_new[z].phosphorus = PhosphorusGrab(xtest, ytest, z, DimInfo, ResourceVector);
				cache_new[z].potassium = PotassiumGrab(xtest, ytest, z, DimInfo, ResourceVector);
				cout << "\nw/n/p/k at (" << xtest << ", " << ytest << ", " << z << "): "
						<< WaterGrab(xtest, ytest, z, DimInfo, ResourceVector)
						<< ", " << NitrogenGrab(xtest, ytest, z, DimInfo, ResourceVector)
						<< ", " << PhosphorusGrab(xtest, ytest, z, DimInfo, ResourceVector)
						<< ", " << PotassiumGrab(xtest, ytest, z, DimInfo, ResourceVector);
			}
			for (unsigned int z = 0; z<DimInfo.depth; z++)
			{
				cout << "\nw/n/p/k diff at (" << xtest << ", " << ytest << ", " << z << "): "
						<< cache_new[z].water - cache_old[z].water
						<< ", " << cache_new[z].nitrogen - cache_old[z].nitrogen
						<< ", " << cache_new[z].phosphorus - cache_old[z].phosphorus
						<< ", " << cache_new[z].potassium - cache_old[z].potassium;
				cache_old[z].water = cache_new[z].water;
				cache_old[z].nitrogen = cache_new[z].nitrogen;
				cache_old[z].phosphorus = cache_new[z].phosphorus;
				cache_old[z].potassium = cache_new[z].potassium;
			}

			cout << "\natmosphere.water: " << atmosphere.water;
			//cout << "\nwatertotal: " << current.water;
			cout << "\nw/n/p/k net change (broken if not all 0's): " << (current.water - initial.water) + ((atmosphere.water - atmosphericwater_initial)/(DimInfo.width*DimInfo.length))
					<< ", " << current.nitrogen - initial.nitrogen
					<< ", " << current.phosphorus - initial.phosphorus
					<< ", " << current.potassium - initial.potassium;
//		}
	}
};




