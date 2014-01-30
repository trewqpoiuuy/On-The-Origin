#include "stdafx.h" //Visual Studio specific include (comment out if not using VS)

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "VectorIntLib.h"
#include "Tree generation.h"

//For 3-d rendering
#include "TreeRender.h"

using namespace std;
using namespace veclib;

int main(int argc, char **argv)
{
	srand(time(NULL));
	int uselessThing=rand(); //First value of rand is the seed, so this gets rid of it
	int depth = 10;
	int length = 100;
	int width = 100;
	int TopsoilDepth = 3;
	DimensionStruct DimInfo;
	DimInfo.length = length;
	DimInfo.width = width;
	DimInfo.depth = depth;
	DimInfo.TopsoilDepth = TopsoilDepth;	
	vector<VectorStruct> ResourceVector=initializeResources(depth, length, width, TopsoilDepth);
	seed treeSeed=generateSeed();
	tree newTree=spawnTree(0,0,0,treeSeed, DimInfo, ResourceVector);
	while(newTree.isAlive==1)
	{
		int sunlightAdded;
		int waterAdded;
		int potassiumAdded;
		int phosphorusAdded;
		int nitrogenAdded;
		cout << "sunlight this turn?" << endl;
		cin >> sunlightAdded;
		newTree.sunlight=newTree.sunlight+sunlightAdded;
		cout << "water this turn?" << endl;
		cin >> waterAdded;
		newTree.water=newTree.water-ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "water", waterAdded);
		cout << "potassium this turn?" << endl;				//Prompts are temporary, will be replaced by game engine commands
		cin >> potassiumAdded;
		newTree.potassium=newTree.potassium-ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "potassium", potassiumAdded);
		cout << "phosphorus this turn?" << endl;
		cin >> phosphorusAdded;
		newTree.phosphorus=newTree.phosphorus-ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "phosphorus", phosphorusAdded);
		cout << "nitrogen this turn?" << endl;
		cin >> nitrogenAdded;
		newTree.nitrogen=newTree.nitrogen-ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "nitrogen", nitrogenAdded);			while(newTree.sunlight>=10 && newTree.water>=20 && newTree.nitrogen>=15 && newTree.potassium>=30 && newTree.phosphorus>=25)
		{
			newTree=growBranch(newTree, ResourceVector, DimInfo);
			newTree=upkeep(newTree, ResourceVector, DimInfo);
		}
		for(int f=0;f<newTree.branches.size();f++) //prints out tree data
		{
			cout << "Branch #"  << f+1 << ": " ;
			cout << "Connection Point: " << newTree.branches.at(f).connection << " ";
			cout << "X Angle: " << newTree.branches.at(f).xAngle << " ";
			cout << "Y Angle: " << newTree.branches.at(f).yAngle << " ";
			cout <<  "Length: " << newTree.branches.at(f).length << " ";
			cout << "Feature: " << newTree.branches.at(f).feature << " " << endl;
			cout << "Children: ";
			for(int g=0;g<newTree.branches.at(f).children.size();g++)
			{
				cout << newTree.branches.at(f).children.at(g) <<" ";
			}
			cout << endl;
		}
		cout << "Sunlight: " << newTree.sunlight << " Water: " << newTree.water << " Phosphorus: " << newTree.phosphorus << " Potassium: " << newTree.potassium << " Nitrogen: " << newTree.nitrogen << endl;
		
		///// Begin 3-d render code /////
		//exits generation
		cout << "Kill? 0 for yes, 1 for no" << endl;
		cin >> newTree.isAlive;
	}
	//calculates XYZ coordinates for each branch
	newTree = CalcXYZ(newTree);

	//renders the tree in a GLUT window
	render(argc, argv, newTree);

	///// End 3-d render code /////
}