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

	int leaves; // 0=noleaves 1=10 leaves. Leaves store water and sunlight
	int wood; // wood stores phosphorus and nitrogen. Will change later.

	int feature; //0=nothing, 1=flower, 2=fruit. More to come
	bool isAlive;
	vector<int> children; //Branches connected to this one

	//3d stuff
	vector<float> xyzPos;
	vector<float> pRot;
};
struct seed
{
	float branchDensity; //The higher the value, the lower branches attach 
	float angleVariance; //Variation in angle
	float featureChance; //likelihood of generating features
	float lengthVariance; //lower value-> shorter branches

	float leafDensity; // Amount of leaf coverage per branch.
	float canopyWeight; // higher the value, the higher on the tree that leaves grow

	int primaryColor[3]; //Bark color
	int secondaryColor[3]; //Leaf color
	int tertiaryColor[3]; //Flower/fruit color
};
struct tree
{
	int sunlight;   //
	int water;      //
	int phosphorus; // Current Resources
	int nitrogen;   //
	int potassium;  //
	int sunlightcap;  //
	int watercap;     //
	int phosporuscap; // Max capacity of resource that the tree can hold
	int nitrogencap;  // currently unused
	int potassiumcap; //
	int x;
	int y;
	int z;
	bool isAlive;
	int reproduced; // 0 = nil 1 = ready 2 = done
	vector<branch> branches;
	vector<branch> deadBranches;
	seed treeSeed;

	//3d stuff (placeholder for determing color)
	float maxvals[3];
};

struct forest
{
	int treenum; //useless right now
	vector<tree> trees;
	vector<tree> deadtrees;
};

seed generateSeed() //Completely new seed with no inheritance
{
	seed treeSeed;
	/*
		~See Tree generation.h for the significance of these values~
	*/
	treeSeed.branchDensity=randFloat(0.5,2);
	treeSeed.angleVariance=randFloat(10,30);
	treeSeed.featureChance=randFloat(0,1);
	treeSeed.lengthVariance=randFloat(0,2);

	treeSeed.leafDensity=randInt(0,200);
	treeSeed.canopyWeight=randFloat(0,10);

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
	//cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << endl;
	//cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << endl;
	//cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << endl;
	//cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance << " Length Variance: " << treeSeed.lengthVariance << endl;
	return treeSeed;
}
tree spawnTree(int x, int y, int z, seed treeSeed, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
{	
	tree newTree;
	newTree.sunlight=0;   //
	newTree.water=WaterGrab(x, y, z,  DimInfo, ResourceVector);      //
	newTree.phosphorus=PhosphorusGrab(x, y, z,  DimInfo, ResourceVector); //Resources
	newTree.nitrogen=NitrogenGrab(x, y, z,  DimInfo, ResourceVector);   //
	newTree.potassium=PotassiumGrab(x, y, z,  DimInfo, ResourceVector);  //
	newTree.isAlive=true;
	newTree.reproduced=0;
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
	trunk.isAlive=1;
	newTree.branches.push_back(trunk);
	//newForest.trees.push_back(newTree);
	return newTree;
}
tree growBranch(tree newTree, vector<VectorStruct> ResourceVector,DimensionStruct DimInfo)
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
	newBranch.isAlive=1;
	int featureChance=rand()%100;
	if(featureChance/100<newTree.treeSeed.featureChance)
	{
		newBranch.feature=randInt(1,3);
	}
	newTree.branches.push_back(newBranch);
	newTree.sunlight=newTree.sunlight-newBranch.length*.25;
	newTree.water=newTree.water+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "water", newBranch.length*.5);
	newTree.nitrogen=newTree.nitrogen+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "nitrogen", newBranch.length*.375);
	newTree.potassium=newTree.potassium+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "potassium", newBranch.length*.75);
	newTree.phosphorus=newTree.phosphorus+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "phosphorus", newBranch.length*.625);

	if(newTree.branches.size() == 20)
	{
		newTree.reproduced = 1;
	}


	return newTree;
}

tree upkeep(tree newTree, vector<VectorStruct> ResourceVector,DimensionStruct DimInfo)
{
	cout << "Upkeepan";
	int totalLength=0;
	for(int i=0; i<newTree.branches.size(); i++)
	{
		totalLength+=newTree.branches.at(i).length;
		if(newTree.sunlight-totalLength*.025<0 || newTree.water-totalLength*.05<0 || newTree.nitrogen-totalLength*.0375<0 || newTree.potassium-totalLength*.075<0 || newTree.phosphorus-totalLength*.0625<0)
		{
		cout << "test2";
			for(int j=1; j<newTree.branches.size()-i; j++)
			{
				//cout << "killing a branch";
				newTree.branches.at(newTree.branches.size()-j).isAlive=0;
				//cout << "adding it to dead branches";
				newTree.deadBranches.push_back(newTree.branches.at(newTree.branches.size()-j));
				//cout << "erasing from live branches";
				newTree.branches.erase(newTree.branches.end()-j);
				cout << "Ash Ketchup";
			}
			
			if(newTree.branches.size() == 1) //if the tree runs out of branches, it's dead.
			{
				//cout << "tree is kill";
				newTree.isAlive = false;
			}
		}
	}
	newTree.sunlight=newTree.sunlight-totalLength*.025;
	
	newTree.water=newTree.water-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "water",-(totalLength*.05));
	newTree.nitrogen=newTree.nitrogen-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "nitrogen", -(totalLength*.0375));
	newTree.potassium=newTree.potassium-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "potassium", -(totalLength*.075));
	newTree.phosphorus=newTree.phosphorus-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "phosphorus", -(totalLength*.0625));
	return newTree;

}



forest reaper(forest newForest) //checks forest for trees with isAlive false, and moves them to deadtrees.
{
	for(int f = 0; f < newForest.trees.size(); f++)
	{
		
		if(newForest.trees.at(f).isAlive == false)
		{
			cout << "Don't fear the reaper";
			newForest.deadtrees.push_back(newForest.trees.at(f));
			newForest.trees.erase(newForest.trees.begin()+f);
		}
	}
	return newForest;
}

forest reproduce(forest newForest, DimensionStruct DimInfo, vector<VectorStruct> ResourceVector)
{
	for(int f = 0; f < newForest.trees.size(); f++)
	{
		if(newForest.trees.at(f).reproduced == 1)
		{
			newForest.trees.at(f).reproduced = 2;
			tree oldTree = newForest.trees.at(f);
			seed newSeed = generateSeed();
			tree newTree = spawnTree(oldTree.x+randFloat(-1,1), oldTree.y, oldTree.z+randFloat(-1,1), newSeed, DimInfo, ResourceVector);
			newForest.trees.push_back(newTree);
		}
	}
	return newForest;
}



