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

	//initializes and fills the object for storing dimensional information about the vector
	DimensionStruct DimInfo;
	DimInfo.length = length;
	DimInfo.width = width;
	DimInfo.depth = depth;
	DimInfo.TopsoilDepth = TopsoilDepth;

	//generates random coordinates for testing
	int xtest = rand () % length;
	int ytest = rand () % width;
	int ztest = rand () % depth;

	//generates a random number to subtract from each resource.
	signed long long int change = -10;

	//couts for testing purposes, uses random xyz coordinates (see above) to return the contents of the vector at that point
	cout << "x: " << xtest << "| y: " << ytest << "| z: " << ztest << "\n";

	cout << "\nWater: ";
	cout <<  WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	cout << "\nNitrogen: ";
	cout << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	cout << "\nPhosphorus: ";
	cout << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	cout << "\nPotassium: ";
	cout << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	cout << "\nchange: " << change;

	//initializes ints for keeping track of the amount by which each resource is reduced
	signed long long int waterremoved = 0;
	unsigned int waterremaining = WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	signed long long int nitrogenremoved = 0;
	unsigned int nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	signed long long int phosphorusremoved = 0;
	unsigned int phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	signed long long int potassiumremoved = 0;
	unsigned int potassiumremaining = PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	//cycles through ResourceChange for remaining water until there is no more water left.
	while (waterremaining!=0)
	{
		waterremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "water", change);
		waterremaining = WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nwater removed: " << waterremoved;
		cout << "\nwater remaining: " << waterremaining;
	}

	//same as above, but for nitrogen
	while (nitrogenremaining!=0)
	{
		nitrogenremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "nitrogen", change);
		nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nnitrogen removed: " << nitrogenremoved;
		cout << "\nnitrogen remaining: " << nitrogenremaining;
	}

	//same as above, but for phosphorus
	while (phosphorusremaining!=0)
	{
		phosphorusremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "phosphorus", change);
		phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nphosphorus removed: " << phosphorusremoved;
		cout << "\nphosphorus remaining: " << phosphorusremaining;
	}

	//same as above, but for potassium
	while (potassiumremaining!=0)
	{
		potassiumremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "potassium", change);
		potassiumremaining = PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\npotassium removed: " << potassiumremoved;
		cout << "\npotassium remaining: " << potassiumremaining;
	}

	//reverses everything, such that the resources are being added to.

	change = 1000000000;

	while (waterremaining!=4292967296-1)
	{
		waterremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "water", change);
		waterremaining = WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nwater removed: " << waterremoved;
		cout << "\nwater remaining: " << waterremaining;
	}

	//same as above, but for nitrogen
	while (nitrogenremaining!=4292967296-1)
	{
		nitrogenremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "nitrogen", change);
		nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nnitrogen removed: " << nitrogenremoved;
		cout << "\nnitrogen remaining: " << nitrogenremaining;
	}

	//same as above, but for phosphorus
	while (phosphorusremaining!=4292967296-1)
	{
		phosphorusremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "phosphorus", change);
		phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nphosphorus removed: " << phosphorusremoved;
		cout << "\nphosphorus remaining: " << phosphorusremaining;
	}

	//same as above, but for potassium
	while (potassiumremaining!=4292967296-1)
	{
		potassiumremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "potassium", change);
		potassiumremaining = PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\npotassium removed: " << potassiumremoved;
		cout << "\npotassium remaining: " << potassiumremaining;
	}

	//testing ID's...
	cout << "\nPlantIDGrab: " << PlantIDGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
	cout << "\nPlantIDCheck: " << PlantIDCheck(xtest, ytest, ztest, DimInfo, ResourceVector);
	cout << "\ninserting plantID 'testing1234'";
	PlantIDAssign(xtest, ytest, ztest, DimInfo, ResourceVector, "testing1234");
	cout << "\nPlantIDGrab: " << PlantIDGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
	cout << "\nPlantIDCheck: " << PlantIDCheck(xtest, ytest, ztest, DimInfo, ResourceVector);
	cout << "\nFalse PlantIDCheck: " << PlantIDCheck(xtest, ytest, ztest, DimInfo, ResourceVector, "nopenopenope");
	cout << "\nSpecific PlantIDCheck: " << PlantIDCheck(xtest, ytest, ztest, DimInfo, ResourceVector, "testing1234");
}



