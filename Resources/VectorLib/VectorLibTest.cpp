/*
 * VectorLibTest.cpp
 *
 *  Created on: Jan 16, 2014
 *      Author: WBoettcher
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <VectorLib.h>

using namespace std;
using namespace veclib;

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
	system("pause>null");
}



