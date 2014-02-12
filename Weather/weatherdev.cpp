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
		unsigned int intensity = 0;
		long int intensity_roc = 0;
		unsigned int halflife;
		int direction = 180;
		int direction_roc = 0;
		unsigned int intensity_max;
		unsigned int intensitychange_max;

		void existanceroll()
		{
			unsigned int roll = rand()%halflife;
			if (roll==(halflife-1))
			{
				if (exists == true)
				{
					exists = false;
					intensity = 0;
				}
				else
					exists = true;
			}
		}

		int directionroll()
		{
			int directionchange = rand()%1;
			int sign = rand()%2;
			if (sign==1)
				directionchange=(-1)*directionchange;
			return directionchange;
		}

		long int intensityroll()
		{
			int intensitychange = rand()%(intensitychange_max/15);
			int sign = rand()%2;
			if (sign==1)
				intensitychange=(-1)*intensitychange;
			return intensitychange;
		}

		void cycle()
		{
			existanceroll();
			if (exists == true)
			{
				int directionchange = directionroll();
				if (direction_roc + directionchange > 5)
					direction_roc = 5;
				else if (direction_roc + directionchange < -5)
					direction_roc = -5;
				else
					direction_roc += directionchange;

				long int intensitychange = intensityroll();
				if (intensity_roc + intensitychange > intensitychange_max)
					direction_roc = intensitychange_max;
				else if (intensity_roc + intensitychange < (-1)*intensitychange_max)
					direction_roc = (-1)*intensitychange_max;
				else
					intensity_roc += intensitychange;

				if (direction + direction_roc > 360)
					direction = (direction + direction_roc)%360;
				else if (direction + direction_roc < 0)
					direction = 360+((direction + direction_roc)%360);
				else
					direction+=directionchange;

				if (intensity + intensity_roc > intensity_max)
					intensity = intensity_max;
				else if (intensity + intensity_roc <= 0)
				{
					exists = false;
					intensity = 0;
				}
				else
					intensity+=intensity_roc;

			}
		}
	} test;

	test.halflife = 20;
	test.intensity_max = 10000;
	test.intensitychange_max = 1000;

	for(unsigned int i = 0; i<10000; i++)
	{
		test.cycle();
		cout << "\nexists: " << test.exists;
		cout << "\ndirection: " << test.direction;
		cout << "\nintensity: " << test.intensity;

	}
};




