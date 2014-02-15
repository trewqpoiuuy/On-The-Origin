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
using namespace std;

int main()
{
	class Weather {
	public:
		bool exists = false;
		unsigned int halflife = 0;
		unsigned int cyclecounter = 0;
		unsigned int cyclereset = 0;

		long int intensity = 0;
		long int intensity_min = 0;
		long int intensity_max = 10000;
		long int intensity_target = 0;
		long int intensitychange_max;

		float direction = 180;
		float direction_target = 180;
		float directionchange_max = 0;

		void cycle()
		{
			//turns the weather pattern on and off at random
			if (halflife>0)
			{
				unsigned int existanceroll = rand()%halflife;
				if (existanceroll == halflife-1)
				{
					if (exists == true)
					{
						exists = false;
						//cout << "\ne false";
					}
					else
					{
						exists = true;
						//cout << "\ne true";
					}
				}
			}
			//turns the weather pattern on and off cyclically
			if (cyclereset > 0)
			{
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
	};

	cout << "Start\n";

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

	for(unsigned int i = 0; i<600; i++)
	{
		rain.cycle();

		cout << "\nrain e|i|i_t: "
		<< rain.exists
		<< "|" << rain.intensity
		<< "|" << rain.intensity_target;
	}

	for(unsigned int i = 0; i<600; i++)
	{
		wind.cycle();

		cout << "\nwind e|d|d_t|i|i_t: "
		<< wind.exists
		<< "|" << wind.direction
		<< "|" << wind.direction_target
		<< "|" << wind.intensity
		<< "|" << wind.intensity_target;
	}

	for(unsigned int i = 0; i<600; i++)
	{
		sun.cycle();

		cout << "\nsun e|c|i|i_t: "
		<< sun.exists
		<< "|" << sun.cyclecounter
		<< "|" << sun.intensity
		<< "|" << sun.intensity_target;
	}
	cout <<"\nRAND_MAX: " << RAND_MAX;
};




