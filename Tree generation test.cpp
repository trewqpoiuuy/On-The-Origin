#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Tree generation.h>
using namespace std;
seed generateSeed()
{
	seed treeSeed;
	/*
		~See Tree generation.h for the significance of these values~
	*/
	treeSeed.branchDensity=randFloat(0.5,2);
	treeSeed.angleVariance=randFloat(10,30);
	treeSeed.featureChance=randFloat(0,1);
	treeSeed.lengthVariance=randFloat(0,2);
	treeSeed.primaryColor[0]=randInt(0,255);
	treeSeed.primaryColor[1]=randInt(0,255);
	treeSeed.primaryColor[2]=randInt(0,255);
	treeSeed.secondaryColor[0]=randInt(0,255);
	treeSeed.secondaryColor[1]=randInt(0,255);
	treeSeed.secondaryColor[2]=randInt(0,255);
	treeSeed.tertiaryColor[0]=randInt(0,255);
	treeSeed.tertiaryColor[1]=randInt(0,255);
	treeSeed.tertiaryColor[2]=randInt(0,255);
	cout << "Seed Data:" << endl;
	cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << endl;
	cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << endl;
	cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << endl;
	cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance << " Length Variance: " << treeSeed.lengthVariance << endl;
	return treeSeed;
}
tree spawnTree(int x, int y,int z,seed treeSeed)
{	
	tree newTree;
	newTree.sunlight=0;   //
	newTree.water=0;      //
	newTree.phosphorus=0; //Resources
	newTree.nitrogen=0;   //
	newTree.potassium=0;  //
	newTree.isAlive=1;
	newTree.x=x;
	newTree.y=y;
	newTree.z=z;
	newTree.treeSeed=treeSeed;
	branch trunk;
	trunk.connection=0;
	trunk.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
	trunk.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
	trunk.length=randFloat(1,40);
	trunk.feature=0;
	newTree.branches.push_back(trunk);
	return newTree;
}
tree growBranch(tree newTree)
{
	branch newBranch;
	int branchWeighting=randInt(0,newTree.branches.size())*newTree.treeSeed.branchDensity; //Weights connection points
	if(branchWeighting>=newTree.branches.size())
	{
		branchWeighting=newTree.branches.size()-1; //Makes sure branches only connect to existing branches
	}
	newBranch.connection=(newTree.branches.size()-branchWeighting);
	newTree.branches.at(newBranch.connection-1).children.push_back(newTree.branches.size()+1);
	newBranch.xAngle=randFloat(-newTree.treeSeed.angleVariance,newTree.treeSeed.angleVariance);
	newBranch.yAngle=randFloat(-newTree.treeSeed.angleVariance,newTree.treeSeed.angleVariance);
	newBranch.length=randFloat(1,20)*newTree.treeSeed.lengthVariance;
	newBranch.feature=0;
	int featureChance=rand()%100;
	if(featureChance/100<newTree.treeSeed.featureChance)
	{
		newBranch.feature=randInt(1,3);
	}
	newTree.branches.push_back(newBranch);
	newTree.sunlight=newTree.sunlight-newBranch.length/2;
	newTree.water=newTree.water-newBranch.length;
	newTree.nitrogen=newTree.nitrogen-newBranch.length/1.5; //Resource usage based solely on length for now. This will likely change.
	newTree.potassium=newTree.potassium-newBranch.length*1.5;
	newTree.phosphorus=newTree.phosphorus-newBranch.length*1.25;
	return newTree;
}
int main()
{
	srand(time(NULL));
	int uselessThing=rand(); //First value of rand is the seed, so this gets rid of it
	seed treeSeed=generateSeed();
	tree newTree=spawnTree(0,0,0,treeSeed);
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
		cout << "potassium this turn?" << endl;				//Prompts are temporary, will be replaced by game engine commands
		cin >> potassiumAdded;
		newTree.potassium=newTree.potassium+potassiumAdded;
		cout << "phosphorus this turn?" << endl;
		cin >> phosphorusAdded;
		newTree.phosphorus=newTree.phosphorus+phosphorusAdded;
		cout << "nitrogen this turn?" << endl;
		cin >> nitrogenAdded;
		newTree.nitrogen=newTree.nitrogen+nitrogenAdded;
			while(newTree.sunlight>=10 && newTree.water>=20 && newTree.nitrogen>=15 && newTree.potassium>=30 && newTree.phosphorus>=25)
			{
				newTree=growBranch(newTree);
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