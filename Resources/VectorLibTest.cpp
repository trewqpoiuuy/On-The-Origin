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
#include "VectorLib.h"

using namespace std;
using namespace veclib;

int main()
{
	//determines the depth, length, and width of the block of soil contained by the vector, also determines the depth of the topsoil.
	int depth = 10;
	int length = 100;
	int width = 100;
	int TopsoilDepth = 3;
	vector<string> ResourceVector=initializeResources(depth,length,width,TopsoilDepth);

	//generates random coordinates for testing
	int xtest = rand () % length;
	int ytest = rand () % width;
	int ztest = rand () % depth;

	//generates a random number to subtract from each resource.
	//int change = 0-((rand () %5)+1);
	int change = -5;

	//initializes ints for keeping track of the amount by which each resource is reduced

	//couts for testing purposes, uses random xyz coordinates (see above) to return the contents of the vector at that point
	cout << "x: " << xtest << "| y: " << ytest << "| z: " << ztest << "\n";
	cout << ResourceGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << "\nWater: ";
	int waterremaining = WaterGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << waterremaining;
	cout << "\nNitrogen: ";
	int nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << nitrogenremaining;
	cout << "\nPhosphorus: ";
	int phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << phosphorusremaining;
	cout << "\nPotassium: ";
	int potassiumremaining = PotassiumGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	cout << potassiumremaining;
	cout << "\nchange: " << change;

	//initializes ints for keeping track of the amount by which each resource is reduced
	int waterremoved = 0;
	int nitrogenremoved = 0;
	int phosphorusremoved = 0;
	int potassiumremoved = 0;

	//cycles through ResourceChange for remainingwater until there is no more water left.
	while (waterremaining!=0)
	{
		cout << "\nwater removed: " << waterremoved;
		cout << "\nwater remaining: " << waterremaining;
		waterremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "water", change);
		waterremaining = WaterGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	}
	cout << "\nfinal water removed: " << waterremoved;
	cout << "\nfinal water remaining: " << waterremaining;

	//same as above, but for nitrogen
	while (nitrogenremaining!=0)
	{
		cout << "\nnitrogen removed: " << nitrogenremoved;
		cout << "\nnitrogen remaining: " << nitrogenremaining;
		nitrogenremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "nitrogen", change);
		nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	}
	cout << "\nfinal nitrogen removed: " << nitrogenremoved;
	cout << "\nfinal nitrogen remaining: " << nitrogenremaining;

	//same as above, but for phosphorus
	while (phosphorusremaining!=0)
	{
		cout << "\nphosphorus removed: " << phosphorusremoved;
		cout << "\nphosphorus remaining: " << phosphorusremaining;
		phosphorusremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "phosphorus", change);
		phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	}
	cout << "\nfinal phosphorus removed: " << phosphorusremoved;
	cout << "\nfinal phosphorus remaining: " << phosphorusremaining;

	//same as above, but for potassium
	while (potassiumremaining!=0)
	{
		cout << "\npotassium removed: " << potassiumremoved;
		cout << "\npotassium remaining: " << potassiumremaining;
		potassiumremoved += ResourceChange(xtest, ytest, ztest, width, length, depth, ResourceVector, "potassium", change);
		potassiumremaining = PotassiumGrab(xtest, ytest, ztest, width, length, depth, ResourceVector);
	}
	cout << "\nfinal potassium removed: " << potassiumremoved;
	cout << "\nfinal potassium remaining: " << potassiumremaining;

	system("pause>null");
}



