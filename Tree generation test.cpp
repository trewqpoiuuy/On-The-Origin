#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "VectorLib.h"
#include "Tree generation.h"

using namespace std;
using namespace veclib;

int main()
{
	srand(time(NULL));
	int uselessThing=rand(); //First value of rand is the seed, so this gets rid of it
	int depth = 10;
	int length = 100;
	int width = 100;
	int TopsoilDepth = 3;
	vector<string> ResourceVector=initializeResources(depth,length,width,TopsoilDepth);
	seed treeSeed=generateSeed();
	tree newTree=spawnTree(0,0,0,treeSeed, width, length, depth, ResourceVector);
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
		newTree.water=newTree.water+waterAdded;
		ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "water", waterAdded);
		cout << "potassium this turn?" << endl;				//Prompts are temporary, will be replaced by game engine commands
		cin >> potassiumAdded;
		newTree.potassium=newTree.potassium+potassiumAdded;
		ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "potassium", potassiumAdded);
		cout << "phosphorus this turn?" << endl;
		cin >> phosphorusAdded;
		newTree.phosphorus=newTree.phosphorus+phosphorusAdded;
		ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "phosphorus", phosphorusAdded);
		cout << "nitrogen this turn?" << endl;
		cin >> nitrogenAdded;
		newTree.nitrogen=newTree.nitrogen+nitrogenAdded;
		ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "nitrogen", nitrogenAdded);
			while(newTree.sunlight>=10 && newTree.water>=20 && newTree.nitrogen>=15 && newTree.potassium>=30 && newTree.phosphorus>=25)
			{
				newTree=growBranch(newTree, ResourceVector, width, length, depth);
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
	}
}