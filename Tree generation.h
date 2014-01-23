#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace veclib;
using namespace std;
float randFloat(float min, float max)
{
	//Returns a random floating point number between min and max
	float r=min+(static_cast <float> (rand())/static_cast <float> (RAND_MAX/(max-min)));
	return r;
}
int randInt(int min, int max)
{
	//Returns a random integer between min and max
	int r=min+(rand()/(RAND_MAX/(max-min)));
	return r;
}
struct branch
{
	int connection; //What the branch connects to
	float xAngle; //
	float yAngle; //Polar coordinates relative to connection
	float length; //
	int feature; //0=nothing, 1=flower, 2=fruit. More to come
	vector<int> children; //Branches connected to this one
};
struct seed
{
	float branchDensity; //The higher the value, the lower branches attach 
	float angleVariance; //Variation in angle
	float featureChance; //likelihood of generating features
	float lengthVariance; //lower value-> shorter branches
	int primaryColor[3]; //Bark color
	int secondaryColor[3]; //Leaf color
	int tertiaryColor[3]; //Flower/fruit color
};
struct tree
{
	int sunlight;   //
	int water;      //
	int phosphorus; //Resources
	int nitrogen;   //
	int potassium;  //
	int x;
	int y;
	int z;
	bool isAlive;
	vector<branch> branches;
	seed treeSeed;
};
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
tree spawnTree(int x, int y, int z, seed treeSeed, int width, int length, int depth, vector<string> ResourceVector)
{	
	tree newTree;
	newTree.sunlight=0;   //
	newTree.water=WaterGrab(x, y, z, width, length, depth, ResourceVector);      //
	newTree.phosphorus=PhosphorusGrab(x, y, z, width, length, depth, ResourceVector); //Resources
	newTree.nitrogen=NitrogenGrab(x, y, z, width, length, depth, ResourceVector);   //
	newTree.potassium=PotassiumGrab(x, y, z, width, length, depth, ResourceVector);  //
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
tree growBranch(tree newTree, vector<string> ResourceVector,int width, int length, int depth)
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
	ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "water", newBranch.length);
	newTree.nitrogen=newTree.nitrogen-newBranch.length/1.5; //Resource usage based solely on length for now. This will likely change.
	ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "nitrogen", newBranch.length/1.5);
	newTree.potassium=newTree.potassium-newBranch.length*1.5;
	ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "potassium", newBranch.length*1.5);
	newTree.phosphorus=newTree.phosphorus-newBranch.length*1.25;
	ResourceChange(newTree.x, newTree.y, newTree.z, width, length, depth, ResourceVector, "phosphorus", newBranch.length*1.25);
	return newTree;
}