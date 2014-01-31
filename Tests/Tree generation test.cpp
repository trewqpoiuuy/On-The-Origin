//#include "stdafx.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Resources/VectorIntLib.h" //delete 'Resources/' if broken
#include "Tree generation.h"
//#include "TreeRender.h"

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
	forest newForest; //all trees
	seed treeSeed=generateSeed();
	tree newTree=spawnTree(0,0,0,treeSeed, DimInfo, ResourceVector);
	newForest.trees.push_back(newTree); //put newTree inside forest.

	char choicy;
	cout << "(m)icro (single tree) or m(a)cro (landscape) testing?" << endl;
	cin >> choicy;

	if (choicy == 'm')
	{
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

				for(int f=0;f<newTree.branches.size()+1;f++) //prints out tree data
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
		//newTree = CalcXYZ(newTree);

		//renders the tree in a GLUT window
		//render(argc, argv, newTree);

		///// End 3-d render code /////
	}
	else //test multiple trees
	{
		int turn = 0;
		int turnstogo = 0;

		while (newForest.trees.size() > 0) // number of alive trees
		{

			cout << "Turn: " << turn << endl;
			cout << "Number of turns to continue?" << endl;
			cin >> turnstogo;

			if(turnstogo == -1)
			{
				newForest.trees.clear();
			}

			while(turnstogo > 0)
			{
				turn += 1;
				cout << "Turn: " << turn << endl;
				cout << "Turnstogo: " << turnstogo << endl;

				for(int f=0; f<newForest.trees.size(); f++) //feed the trees
				{
					cout << f;
					int feed = 25;
					newForest.trees.at(f).water=newForest.trees.at(f).water-ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "water", feed);
					newForest.trees.at(f).potassium=newForest.trees.at(f).potassium-ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "potassium", feed);
					newForest.trees.at(f).phosphorus=newForest.trees.at(f).phosphorus-ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "phosphorus", feed);
					newForest.trees.at(f).nitrogen=newForest.trees.at(f).nitrogen-ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "nitrogen", feed);

					//let the trees do their tree thing
					newForest.trees.at(f)=growBranch(newForest.trees.at(f), ResourceVector, DimInfo);
					cout << "check2";
					//newForest.trees.at(f)=upkeep(newForest.trees.at(f), ResourceVector, DimInfo); //  <--- BROKEN
					cout << "check3";
					newForest = reproduce(newForest, DimInfo, ResourceVector);
				}



				cout << "Number of trees: " << newForest.trees.size() << endl;
				cout << "Number of dead trees: " << newForest.deadtrees.size() << endl;

				for(int f=0; f<newForest.trees.size();f++)
				{
					cout << "Tree number: " << f+1 << endl;
					cout << "Number of Branches: " << newForest.trees.at(f).branches.size() << endl;
					cout << "Number of Dead Branches: " << newForest.trees.at(f).deadBranches.size() << endl;
				}
				turnstogo -= 1;

			}

		}
	}



}
