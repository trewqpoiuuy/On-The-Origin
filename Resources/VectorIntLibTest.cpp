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
#include <limits.h>

using namespace std;
using namespace veclib;

int main()
{
	//determines the depth, length, and width of the block of soil contained by the vector, also determines the depth of the topsoil.
	/*
	int depth = 10;
	int length = 100;
	int width = 100;
	int TopsoilDepth = 3;
	*/
	int depth = 2;
	int length = 5;
	int width = 5;
	int TopsoilDepth = 1;

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

	//initializes a number to subtract from each resource.
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

	while (waterremaining!=UINT_MAX)
	{
		waterremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "water", change);
		waterremaining = WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nwater removed: " << waterremoved;
		cout << "\nwater remaining: " << waterremaining;
	}

	//same as above, but for nitrogen
	while (nitrogenremaining!=UINT_MAX)
	{
		nitrogenremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "nitrogen", change);
		nitrogenremaining = NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nnitrogen removed: " << nitrogenremoved;
		cout << "\nnitrogen remaining: " << nitrogenremaining;
	}

	//same as above, but for phosphorus
	while (phosphorusremaining!=UINT_MAX)
	{
		phosphorusremoved += ResourceChange(xtest, ytest, ztest, DimInfo, ResourceVector, "phosphorus", change);
		phosphorusremaining = PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nphosphorus removed: " << phosphorusremoved;
		cout << "\nphosphorus remaining: " << phosphorusremaining;
	}

	//same as above, but for potassium
	while (potassiumremaining!=UINT_MAX)
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

	//Mycelium testing below ------------------------------------------------------------------------------------------------------------

		cout << "\nMycelium Testing...";

	//setting resource values for testing purposes


		ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].water = 0;
		cout << "\nWater: " << WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].nitrogen = 0;
		cout << "\nNitrogen: " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].phosphorus = 0;
		cout << "\nPhosphorus: " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].potassium = 0;
		cout << "\nPotassium: " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	//initializing mycelium
		vector<ResourceCache> MyceliumCache = initializeMycelium(DimInfo.depth);
		cout << "\nMycelium initialized...";

	//printing mycelium values for reference
		cout << "\nMyceliumCache[z].water: " << MyceliumCache[ztest].water;
		cout << "\nMyceliumCache[z].nitrogen: " << MyceliumCache[ztest].nitrogen;
		cout << "\nMyceliumCache[z].phosphorus: " << MyceliumCache[ztest].phosphorus;
		cout << "\nMyceliumCache[z].potassium: " << MyceliumCache[ztest].potassium;

	//more reference values
		cout << "\nMyceliumCache[z].water: " << MyceliumCache[ztest].water;
		cout << "\nWater: " << WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nMyceliumCache[z].nitrogen: " << MyceliumCache[ztest].nitrogen;
		cout << "\nNitrogen: " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nMyceliumCache[z].phosphorus: " << MyceliumCache[ztest].phosphorus;
		cout << "\nPhosphorus: " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
		cout << "\nMyceliumCache[z].potassium: " << MyceliumCache[ztest].potassium;
		cout << "\nPotassium: " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

	//finds the average value for each resource at the test depth.
		unsigned long long int watertotal = 0;
		unsigned long long int nitrogentotal = 0;
		unsigned long long int phosphorustotal = 0;
		unsigned long long int potassiumtotal = 0;
		for (int x=0; x<DimInfo.length; x++)
		{
			for (int y=0; y<DimInfo.width; y++)
			{
				signed long long int nonZ = (y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width);
				watertotal += ResourceVector[ztest+nonZ].water;
				nitrogentotal += ResourceVector[ztest+nonZ].nitrogen;
				phosphorustotal += ResourceVector[ztest+nonZ].phosphorus;
				potassiumtotal += ResourceVector[ztest+nonZ].potassium;
			}
		}
		unsigned int wateravg = watertotal/(DimInfo.width*DimInfo.length);
		unsigned int nitrogenavg = nitrogentotal/(DimInfo.width*DimInfo.length);
		unsigned int phosphorusavg = phosphorustotal/(DimInfo.width*DimInfo.length);
		unsigned int potassiumavg = potassiumtotal/(DimInfo.width*DimInfo.length);


	//repeats Mycelium until the test values = the averages
		vector<ResourceCache> MycoOutput(2);

		//cout << "\nabs(wateravg - WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector)): " << abs(wateravg - WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector));
		while (
				(wateravg - (MyceliumCache[ztest].water/(DimInfo.width*DimInfo.length))) - WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
				and (nitrogenavg - (MyceliumCache[ztest].nitrogen/(DimInfo.width*DimInfo.length))) - NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
				and (phosphorusavg - (MyceliumCache[ztest].phosphorus/(DimInfo.width*DimInfo.length))) - PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
				and (potassiumavg - (MyceliumCache[ztest].potassium/(DimInfo.width*DimInfo.length))) - PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
			)
		{
			MycoOutput = Mycelium(ztest, DimInfo, ResourceVector, MyceliumCache, 10, 10, 10, 10);
			cout << "\nw/n/p/k avgs: " << wateravg << ", " << nitrogenavg << ", " << phosphorusavg << ", " << potassiumavg;
			cout << "\nw/n/p/k: "
					<< WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
					<< ", " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
					<< ", " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
					<< ", " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
			cout << "\nMyco w/n/p/k: "
					<< MyceliumCache[ztest].water/(DimInfo.width*DimInfo.length)
					<< ", " << MyceliumCache[ztest].nitrogen/(DimInfo.width*DimInfo.length)
					<< ", " << MyceliumCache[ztest].phosphorus/(DimInfo.width*DimInfo.length)
					<< ", " << MyceliumCache[ztest].potassium/(DimInfo.width*DimInfo.length);
			/*
			wateravg = MycoOutput[0].water;
			nitrogenavg = MycoOutput[0].nitrogen;
			phosphorusavg = MycoOutput[0].phosphorus;
			potassiumavg = MycoOutput[0].potassium;
			*/
		}

		if (MycoOutput[1].water + MyceliumCache[ztest].water == watertotal)
			cout << "\nMycelium function working for water (small outlier)";
		else
		{
			cout << "\navg: " << wateravg
			<< ", resource amount at test xyz: " << WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
			<< ", MycoTotal: " << MycoOutput[1].water
			<< ", MycoCache: " << MyceliumCache[ztest].water
			<< ", resource total: " << watertotal;
		}

		if (MycoOutput[1].nitrogen + MyceliumCache[ztest].nitrogen == nitrogentotal)
			cout << "\nMycelium function working for nitrogen (small outlier)";
		else
		{
			cout << "\navg: " << nitrogenavg
			<< ", resource amount at test xyz: " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
			<< ", MycoTotal: " << MycoOutput[1].nitrogen
			<< ", MycoCache: " << MyceliumCache[ztest].nitrogen
			<< ", resource total: " << nitrogentotal;
		}


		if (MycoOutput[1].phosphorus + MyceliumCache[ztest].phosphorus == phosphorustotal)
			cout << "\nMycelium function working for phosphorus (small outlier)";
		else
				{
					cout << "\navg: " << phosphorusavg
					<< ", resource amount at test xyz: " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
					<< ", MycoTotal: " << MycoOutput[1].phosphorus
					<< ", MycoCache: " << MyceliumCache[ztest].phosphorus
					<< ", resource total: " << phosphorustotal;
				}

		if (MycoOutput[1].potassium + MyceliumCache[ztest].potassium == potassiumtotal)
			cout << "\nMycelium function working for potassium (small outlier)";
		else
				{
					cout << "\navg: " << potassiumavg
					<< ", resource amount at test xyz: " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
					<< ", MycoTotal: " << MycoOutput[1].potassium
					<< ", MycoCache: " << MyceliumCache[ztest].potassium
					<< ", resource total: " << potassiumtotal;
				}

//----------------------------------------------------------------------------------------------------------------------
		//setting resource values for testing purposes

				ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].water = 100;
				cout << "\nWater: " << WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
				ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].nitrogen = 100;
				cout << "\nNitrogen: " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
				ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].phosphorus = 100;
				cout << "\nPhosphorus: " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
				ResourceVector[ztest+(ytest*DimInfo.depth)+(xtest*DimInfo.depth*DimInfo.width)].potassium = 100;
				cout << "\nPotassium: " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

				MyceliumCache[ztest].water = 0;
				MyceliumCache[ztest].nitrogen = 0;
				MyceliumCache[ztest].phosphorus = 0;
				MyceliumCache[ztest].potassium = 0;

			//printing mycelium values for reference
				cout << "\nMyceliumCache[z].water: " << MyceliumCache[ztest].water;
				cout << "\nMyceliumCache[z].nitrogen: " << MyceliumCache[ztest].nitrogen;
				cout << "\nMyceliumCache[z].phosphorus: " << MyceliumCache[ztest].phosphorus;
				cout << "\nMyceliumCache[z].potassium: " << MyceliumCache[ztest].potassium;

			//more reference values
				cout << "\nMyceliumCache[z].water: " << MyceliumCache[ztest].water;
				cout << "\nWater: " << WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
				cout << "\nMyceliumCache[z].nitrogen: " << MyceliumCache[ztest].nitrogen;
				cout << "\nNitrogen: " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
				cout << "\nMyceliumCache[z].phosphorus: " << MyceliumCache[ztest].phosphorus;
				cout << "\nPhosphorus: " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
				cout << "\nMyceliumCache[z].potassium: " << MyceliumCache[ztest].potassium;
				cout << "\nPotassium: " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);

			//finds the average value for each resource at the test depth.
				watertotal = 0;
				nitrogentotal = 0;
				phosphorustotal = 0;
				potassiumtotal = 0;
				for (int x=0; x<DimInfo.length; x++)
				{
					for (int y=0; y<DimInfo.width; y++)
					{
						signed long long int nonZ = (y*DimInfo.depth)+(x*DimInfo.depth*DimInfo.width);
						watertotal += ResourceVector[ztest+nonZ].water;
						nitrogentotal += ResourceVector[ztest+nonZ].nitrogen;
						phosphorustotal += ResourceVector[ztest+nonZ].phosphorus;
						potassiumtotal += ResourceVector[ztest+nonZ].potassium;
					}
				}
				wateravg = watertotal/(DimInfo.width*DimInfo.length);
				nitrogenavg = nitrogentotal/(DimInfo.width*DimInfo.length);
				phosphorusavg = phosphorustotal/(DimInfo.width*DimInfo.length);
				potassiumavg = potassiumtotal/(DimInfo.width*DimInfo.length);


			//repeats Mycelium until the test values = the averages


				//cout << "\nabs(wateravg - WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector)): " << abs(wateravg - WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector));
				while (
						(wateravg - (MyceliumCache[ztest].water/(DimInfo.width*DimInfo.length))) - WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
						and (nitrogenavg - (MyceliumCache[ztest].nitrogen/(DimInfo.width*DimInfo.length))) - NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
						and (phosphorusavg - (MyceliumCache[ztest].phosphorus/(DimInfo.width*DimInfo.length))) - PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
						and (potassiumavg - (MyceliumCache[ztest].potassium/(DimInfo.width*DimInfo.length))) - PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector) != 0
					)
				{
					MycoOutput = Mycelium(ztest, DimInfo, ResourceVector, MyceliumCache, 10, 10, 10, 10);
					cout << "\nw/n/p/k avgs: " << wateravg << ", " << nitrogenavg << ", " << phosphorusavg << ", " << potassiumavg;
					cout << "\nw/n/p/k: "
							<< WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
							<< ", " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
							<< ", " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
							<< ", " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector);
					cout << "\nMyco w/n/p/k: "
							<< MyceliumCache[ztest].water/(DimInfo.width*DimInfo.length)
							<< ", " << MyceliumCache[ztest].nitrogen/(DimInfo.width*DimInfo.length)
							<< ", " << MyceliumCache[ztest].phosphorus/(DimInfo.width*DimInfo.length)
							<< ", " << MyceliumCache[ztest].potassium/(DimInfo.width*DimInfo.length);
					/*
					wateravg = MycoOutput[0].water;
					nitrogenavg = MycoOutput[0].nitrogen;
					phosphorusavg = MycoOutput[0].phosphorus;
					potassiumavg = MycoOutput[0].potassium;
					*/
				}

				if (MycoOutput[1].water + MyceliumCache[ztest].water == watertotal)
					cout << "\nMycelium function working for water (large outlier)";
				else
				{
					cout << "\navg: " << wateravg
					<< ", resource amount at test xyz: " << WaterGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
					<< ", MycoTotal: " << MycoOutput[1].water
					<< ", MycoCache: " << MyceliumCache[ztest].water
					<< ", resource total: " << watertotal;
				}

				if (MycoOutput[1].nitrogen + MyceliumCache[ztest].nitrogen == nitrogentotal)
					cout << "\nMycelium function working for nitrogen (large outlier)";
				else
				{
					cout << "\navg: " << nitrogenavg
					<< ", resource amount at test xyz: " << NitrogenGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
					<< ", MycoTotal: " << MycoOutput[1].nitrogen
					<< ", MycoCache: " << MyceliumCache[ztest].nitrogen
					<< ", resource total: " << nitrogentotal;
				}


				if (MycoOutput[1].phosphorus + MyceliumCache[ztest].phosphorus == phosphorustotal)
					cout << "\nMycelium function working for phosphorus (large outlier)";
				else
						{
							cout << "\navg: " << phosphorusavg
							<< ", resource amount at test xyz: " << PhosphorusGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
							<< ", MycoTotal: " << MycoOutput[1].phosphorus
							<< ", MycoCache: " << MyceliumCache[ztest].phosphorus
							<< ", resource total: " << phosphorustotal;
						}

				if (MycoOutput[1].potassium + MyceliumCache[ztest].potassium == potassiumtotal)
					cout << "\nMycelium function working for potassium (large outlier)";
				else
						{
							cout << "\navg: " << potassiumavg
							<< ", resource amount at test xyz: " << PotassiumGrab(xtest, ytest, ztest, DimInfo, ResourceVector)
							<< ", MycoTotal: " << MycoOutput[1].potassium
							<< ", MycoCache: " << MyceliumCache[ztest].potassium
							<< ", resource total: " << potassiumtotal;
						}

//testing sign and absdiff
		cout << "\nnegative: " << sign(-1234567);
		cout << "\npositive: " << sign(1234567);

		cout << "\npositive: " << absdiff(1);
		cout << "\npositive: " << absdiff(1, 2);
		cout << "\npositive: " << absdiff(2, 1);


}



