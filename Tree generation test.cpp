#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Tree generation.h>
using namespace std;
int main()
{
	srand(time(NULL));
	int uselessThing=rand(); //First value of rand is the seed, so this gets rid of it
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
	int sunlight=0;   //
	int water=0;      //
	int phosphorus=0; //Resources
	int nitrogen=0;   //
	int potassium=0;  //
	bool isAlive=1;
	vector<branch> tree; //Stores all the branches
	branch trunk;
	trunk.connection=0;
	trunk.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
	trunk.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
	trunk.length=randFloat(1,40);
	trunk.feature=0;
	tree.push_back(trunk);
	while(isAlive==1)
	{
		int sunlightAdded;
		int waterAdded;
		int potassiumAdded;
		int phosphorusAdded;
		int nitrogenAdded;
		cout << "sunlight this turn?" << endl;
		cin >> sunlightAdded;
		sunlight=sunlight+sunlightAdded;
		cout << "water this turn?" << endl;
		cin >> waterAdded;
		water=water+waterAdded;
		cout << "potassium this turn?" << endl;				//Prompts are temporary, will be replaced by game engine commands
		cin >> potassiumAdded;
		potassium=potassium+potassiumAdded;
		cout << "phosphorus this turn?" << endl;
		cin >> phosphorusAdded;
		phosphorus=phosphorus+phosphorusAdded;
		cout << "nitrogen this turn?" << endl;
		cin >> nitrogenAdded;
		nitrogen=nitrogen+nitrogenAdded;
		while(sunlight>=10 && water>=20 && nitrogen>=15 && potassium>=30 && phosphorus>=25)
		{
			branch newBranch;
			int branchWeighting=randInt(0,tree.size())*treeSeed.branchDensity; //Weights connection points
			if(branchWeighting>=tree.size())
			{
				branchWeighting=tree.size()-1; //Makes sure branches only connect to existing branches
			}
			newBranch.connection=(tree.size()-branchWeighting);
			tree.at(newBranch.connection-1).children.push_back(tree.size()+1);
			newBranch.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
			newBranch.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
			newBranch.length=randFloat(1,20)*treeSeed.lengthVariance;
			newBranch.feature=0;
			int featureChance=rand()%100;
			if(featureChance/100<treeSeed.featureChance)
			{
				newBranch.feature=randInt(1,3);
			}
			tree.push_back(newBranch);
			sunlight=sunlight-newBranch.length/2;
			water=water-newBranch.length;
			nitrogen=nitrogen-newBranch.length/1.5; //Resource usage based solely on length for now. This will likely change.
			potassium=potassium-newBranch.length*1.5;
			phosphorus=phosphorus-newBranch.length*1.25;
		}
		for(int f=0;f<tree.size();f++) //prints out tree data
		{
			cout << "Branch #"  << f+1 << ": " ;
			cout << "Connection Point: " << tree.at(f).connection << " ";
			cout << "X Angle: " << tree.at(f).xAngle << " ";
			cout << "Y Angle: " << tree.at(f).yAngle << " ";
			cout <<  "Length: " << tree.at(f).length << " ";
			cout << "Feature: " << tree.at(f).feature << " " << endl;
			cout << "Children: ";
			for(int g=0;g<tree.at(f).children.size();g++)
			{
				cout << tree.at(f).children.at(g) <<" ";
			}
			cout << endl;
		}
	cout << "Sunlight: " << sunlight << " Water: " << water << " Phosphorus: " << phosphorus << " Potassium: " << potassium << " Nitrogen: " << nitrogen << endl;
	}
}