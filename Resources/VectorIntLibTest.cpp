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
#include "VectorIntLib.h"

using namespace std;
using namespace veclib;

int main()
{
	//determines the depth, length, and width of the block of soil contained by the vector, also determines the depth of the topsoil.
	int depth = 10;
	int length = 100;
	int width = 100;
	int TopsoilDepth = 3;
	vector<VectorStruct> ResourceVector=initializeResources(depth,length,width,TopsoilDepth);

	//generates random coordinates for testing
	int xtest = rand () % length;
	int ytest = rand () % width;
	int ztest = rand () % depth;

	//generates a random number to subtract from each resource.
	//int change = 0-((rand () %5)+1);
	signed long long int change = -10;

	//initializes ints for keeping track of the amount by which each resource is reduced

	//couts for testing purposes, uses random xyz coordinates (see above) to return the contents of the vector at that point
	cout << "x: " << xtest << "| y: " << ytest << "| z: " << ztest << "\n";

	cout << "\nWater: ";
	cout <<  WaterGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	cout << "\nNitrogen: ";
	cout << NitrogenGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	cout << "\nPhosphorus: ";
	cout << PhosphorusGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	cout << "\nPotassium: ";
	cout << PotassiumGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	cout << "\nchange: " << change;

	//initializes ints for keeping track of the amount by which each resource is reduced
	signed long long int waterremoved = 0;
	unsigned int waterremaining = WaterGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	signed long long int nitrogenremoved = 0;
	unsigned int nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	signed long long int phosphorusremoved = 0;
	unsigned int phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	signed long long int potassiumremoved = 0;
	unsigned int potassiumremaining = PotassiumGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);

	//cycles through ResourceChange for remaining water until there is no more water left.
	while (waterremaining!=0)
	{
		waterremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "water", change);
		waterremaining = WaterGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\nwater removed: " << waterremoved;
		cout << "\nwater remaining: " << waterremaining;
	}

	//same as above, but for nitrogen
	while (nitrogenremaining!=0)
	{
		nitrogenremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "nitrogen", change);
		nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\nnitrogen removed: " << nitrogenremoved;
		cout << "\nnitrogen remaining: " << nitrogenremaining;
	}

	//same as above, but for phosphorus
	while (phosphorusremaining!=0)
	{
		phosphorusremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "phosphorus", change);
		phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\nphosphorus removed: " << phosphorusremoved;
		cout << "\nphosphorus remaining: " << phosphorusremaining;
	}

	//same as above, but for potassium
	while (potassiumremaining!=0)
	{
		potassiumremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "potassium", change);
		potassiumremaining = PotassiumGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\npotassium removed: " << potassiumremoved;
		cout << "\npotassium remaining: " << potassiumremaining;
	}

	//reverses everything, such that the resources are being added to.

	change = 1000000000;

	while (waterremaining!=4292967296-1)
	{
		waterremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "water", change);
		waterremaining = WaterGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\nwater removed: " << waterremoved;
		cout << "\nwater remaining: " << waterremaining;
	}

	//same as above, but for nitrogen
	while (nitrogenremaining!=4292967296-1)
	{
		nitrogenremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "nitrogen", change);
		nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\nnitrogen removed: " << nitrogenremoved;
		cout << "\nnitrogen remaining: " << nitrogenremaining;
	}

	//same as above, but for phosphorus
	while (phosphorusremaining!=4292967296-1)
	{
		phosphorusremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "phosphorus", change);
		phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\nphosphorus removed: " << phosphorusremoved;
		cout << "\nphosphorus remaining: " << phosphorusremaining;
	}

	//same as above, but for potassium
	while (potassiumremaining!=4292967296-1)
	{
		potassiumremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "potassium", change);
		potassiumremaining = PotassiumGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
		cout << "\npotassium removed: " << potassiumremoved;
		cout << "\npotassium remaining: " << potassiumremaining;
	}
}



