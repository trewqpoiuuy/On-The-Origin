/*
 * weather.h
 *
 *  Created on: Feb 14, 2014
 *      Author: WBoettcher
 */

#ifndef WEATHER_H_
#define WEATHER_H_
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include "VectorIntLib.h"
using namespace std;
using namespace veclib;
namespace weather
{
class Weather {
	public:
		bool exists = false;
		unsigned int halflife = 0;
		unsigned int cyclecounter = 0;
		unsigned int cyclereset = 0;

		long long int intensity = 0;
		long long int intensity_min = 0;
		long long int intensity_max = 10000;
		long long int intensity_target = 0;
		long long int intensitychange_max;

		float direction = 180;
		float direction_target = 180;
		float directionchange_max = 0;

		void cycle()
		{
			//turns the weather pattern on and off at random
			if (halflife > 0)
			{
				//cout << "\nHL: " << halflife;
				unsigned int existanceroll = rand()%halflife;
				if (existanceroll == halflife-1)
				{
					if (exists == true)
					{
						exists = false;
					}
					else
					{
						exists = true;
					}
				}
			}
			//turns the weather pattern on and off cyclically
			if (cyclereset > 0)
			{
				//cout << "\nCR: " << cyclereset;
				if (cyclecounter == cyclereset-1)
				{
					cyclecounter = 0;
					if (exists == true)
					{
						exists = false;
						intensity = 0;
						intensity_target = 0;
					}
					else
					{
						exists = true;
						intensity = intensity_min;
						intensity_target = intensity_min;
					}
				}
				else
					cyclecounter+=1;
			}

			//sets target for intensity
			if (exists == false)
				intensity_target = 0;
			else if (intensity == intensity_target)
				intensity_target = intensity_min + rand()%(intensity_max-intensity_min);

			//moves intensity in the direction of the target
			if (intensity<intensity_target)
			{
				intensity += ((rand()%intensitychange_max)+1);
				if (intensity>intensity_target) //prevents intensity from overshooting the target
					intensity = intensity_target;
			}
			else if (intensity>intensity_target)
			{
				if (exists == false)
					intensity = intensity/2; //drops intensity exponentially if the weather phenomenon doesn't exist
				intensity -= ((rand()%intensitychange_max)+1);
				if (intensity<intensity_target) //prevents intensity from overshooting the target
					intensity = intensity_target;
			}

			//checks for the applicability of direction
			if (directionchange_max > 0)
			{
				//sets target for direction
				if (direction == direction_target)
					direction_target = (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/360)));

				//moves direction towards the target
				if (direction<direction_target)
				{
					direction += (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/directionchange_max))); //random float between 0 and directionchange_max
						if (direction>direction_target) //prevents direction from overshooting the target
							direction = direction_target;
				}
				else if (direction>direction_target)
				{
					direction -= (static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/directionchange_max)));
					if (direction<direction_target) //prevents direction from overshooting the target
						direction = direction_target;
				}
			}
		}
	} rain, wind, sun;

ResourceCache initializeweather(Weather& rain, Weather& sun, Weather& wind)
{
	rain.halflife = 30;
	rain.intensity_max = 10000;
	rain.intensitychange_max = 100;

	wind.halflife = 100;
	wind.intensity_max = 1000;
	wind.intensitychange_max = 10;
	wind.directionchange_max = 1;

	sun.cyclereset = 24;
	sun.intensity_max = 1100;
	sun.intensity_min = 900;
	sun.intensitychange_max = 10;

	ResourceCache atmosphere;
	atmosphere.water = LONG_MAX;
	return atmosphere;
}

void rainfall (DimensionStruct& DimInfo, vector<VectorStruct>& ResourceVector, Weather& rain, ResourceCache& atmosphere)
{
	if(atmosphere.water>=(rain.intensity*DimInfo.width*DimInfo.length)) //if the atmosphere has sufficient water to run a full iteration of rainfall.
	{
		ResourceCache temp_resourcecache;
		temp_resourcecache.water = 0;
		temp_resourcecache.nitrogen = 0;
		temp_resourcecache.phosphorus = 0;
		temp_resourcecache.potassium = 0;

		for (unsigned int x=0; x<DimInfo.length; x++)
		{
			for (unsigned int y=0; y<DimInfo.width; y++)
			{
				atmosphere.water -= rain.intensity;
				atmosphere.water += resourcedrop(x, y, DimInfo, ResourceVector, "water", rain.intensity, rain.intensity_max/DimInfo.depth);
///*
				if (rain.intensity > rain.intensity_max/DimInfo.depth or WaterGrab(x, y, 0, DimInfo, ResourceVector) == UINT_MAX) //if water is added to layers of soil below depth 0
				{
					//takes resources out of the top layer of topsoil
					temp_resourcecache.nitrogen += ResourceChange(x, y, 0, DimInfo, ResourceVector, "nitrogen", rain.intensity*(-(DimInfo.TopsoilDepth+1)));
					temp_resourcecache.phosphorus += ResourceChange(x, y, 0, DimInfo, ResourceVector, "phosphorus", rain.intensity*(-(DimInfo.TopsoilDepth+1)));
					temp_resourcecache.potassium += ResourceChange(x, y, 0, DimInfo, ResourceVector, "potassium", rain.intensity*(-(DimInfo.TopsoilDepth+1)));
					//cout << "\nrain.intensity*(-(DimInfo.TopsoilDepth+1)): " << rain.intensity*(-(DimInfo.TopsoilDepth+1));
					//cout << "\n(-(DimInfo.TopsoilDepth+1)): " << (-(DimInfo.TopsoilDepth+1));

					ResourceChange(x, y, 0, DimInfo, ResourceVector, "nitrogen",
							resourcedrop(x, y, DimInfo, ResourceVector, "nitrogen", temp_resourcecache.nitrogen,
									temp_resourcecache.nitrogen/((rain.intensity*DimInfo.depth)/rain.intensity_max)));
					ResourceChange(x, y, 0, DimInfo, ResourceVector, "phosphorus",
							resourcedrop(x, y, DimInfo, ResourceVector, "phosphorus", temp_resourcecache.phosphorus,
									temp_resourcecache.phosphorus/((rain.intensity*DimInfo.depth)/rain.intensity_max)));
					ResourceChange(x, y, 0, DimInfo, ResourceVector, "potassium",
							resourcedrop(x, y, DimInfo, ResourceVector, "potassium", temp_resourcecache.potassium,
									temp_resourcecache.potassium/((rain.intensity*DimInfo.depth)/rain.intensity_max)));

					temp_resourcecache.water = 0;
					temp_resourcecache.nitrogen = 0;
					temp_resourcecache.phosphorus = 0;
					temp_resourcecache.potassium = 0;

					//cout << "\ntemp_resourcecache.nitrogen: " << temp_resourcecache.nitrogen;
				}
//*/
			}
		}
	}
	else //turns off rain if the atmosphere is too dry to support it
	{
		rain.exists = false;
		rain.intensity_target = 0;
		cout << "\natmosphere dry";
	}
}

void sunshine(DimensionStruct& DimInfo, vector<VectorStruct>& ResourceVector, Weather& sun, ResourceCache& atmosphere)
{
	for (unsigned int x=0; x<DimInfo.length; x++)
	{
		for (unsigned int y=0; y<DimInfo.width; y++)
		{
			atmosphere.water += sun.intensity;
			atmosphere.water -= resourcedrop(x, y, DimInfo, ResourceVector, "water", -sun.intensity, sun.intensity_max/DimInfo.TopsoilDepth);
			if (atmosphere.water > LONG_MAX)
				sun.intensity_target -= sun.intensity_target/20;
		}
	}
}

}
#endif /* WEATHER_H_ */
