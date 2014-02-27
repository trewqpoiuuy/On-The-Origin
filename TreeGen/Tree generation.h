#include <iostream>
#include <vector>
#include <string>
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

struct fire
{
	int fire;
	int x;
	int y;
	int z;
};
 
 
struct branch
{
       int connection; //What the branch connects to
       float xAngle; //
       float yAngle; //Polar coordinates relative to connection
       float length; //
       float diameter; // how thick the branch is compared to the original size. 1 = trunk, .1 = twig
	   //float width;
 
       int leafCount; // number of leaves
       //int age; // amount of turns this branch has been alive
 
       int feature; //0=nothing, 1=flower, 2=fruit, 3= seed pod. More to come
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

	   float leafSize; //size of leaf
       float leafDensity; // Amount of leaf coverage per branch.
       int canopyWeight; // higher the value, the higher on the tree that leaves grow
       int youth; // age that branch grows as many branches as possible
       int adult; // age that branch stops growing more branches and grows features instead
 
       float thickness; // aspiring thickness of the tree
       float diameterloss; //the change in thickness between each generation of branch.

       int primaryColor[3]; //Bark color
       int secondaryColor[3]; //Leaf color
       int tertiaryColor[3]; //Flower/fruit color
};
struct tree
{
	   int fire;
       int sunlight;   //
       int water;      //
       int phosphorus; // Current Resources
       int nitrogen;   //
       int potassium;  //
       int sunlightcap;  //
       int watercap;     //
       int phosphoruscap; // Max capacity of resource that the tree can hold
       int nitrogencap;  // currently unused
       int potassiumcap; //
       int age;
	   int roots;
       int x;
       int y;
       int z;
       bool isAlive;
       int reproduced; // 0 = nil 1 = ready 2 = done

       float thickness; // thickness of the tree

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
	   vector<fire> fires;
};
int absoluteIntMutation(int variable1, int variable2, int rads)
{
       int variable3 = variable1+variable2;
       if(randInt(0,10)==6)
       {
       variable3+= randInt(-rads, rads);
       }
       if(variable3 < 1 )
       {
              variable3 = 1;
       }
       variable3 = variable3/2;
      
       return variable3;
}
 
float absoluteFloatMutation(float variable1, float variable2, float rads)
{
       float variable3 = variable1+variable2;
       if(randInt(0,10)==6)
       {
       variable3+= randFloat(-rads, rads);
       }
       if(variable3 < .1 )
       {
              variable3 = .1;
       }
       variable3 = variable3/2;
      
       return variable3;
}
seed goForthAndMultiply(seed& seed1, seed& seed2)
{
	   int radiation = 1; //change this if you want to make the mutation rate higher or lower. 1 = default
       seed newSeed;
       newSeed = seed1;
       newSeed.primaryColor[0]= absoluteIntMutation(seed1.primaryColor[0],seed2.primaryColor[0], 1*radiation);
       newSeed.primaryColor[1]= absoluteIntMutation(seed1.primaryColor[1],seed2.primaryColor[1], 1*radiation);
       newSeed.primaryColor[2]= absoluteIntMutation(seed1.primaryColor[2],seed2.primaryColor[2], 1*radiation);
       newSeed.secondaryColor[0]= absoluteIntMutation(seed1.secondaryColor[0],seed2.secondaryColor[0], 1*radiation);
       newSeed.secondaryColor[1]= absoluteIntMutation(seed1.secondaryColor[1],seed2.secondaryColor[1], 1*radiation);
       newSeed.secondaryColor[2]= absoluteIntMutation(seed1.secondaryColor[2],seed2.secondaryColor[2], 1*radiation);
       newSeed.tertiaryColor[0]= absoluteIntMutation(seed1.tertiaryColor[0],seed2.tertiaryColor[0], 1*radiation);
       newSeed.tertiaryColor[1]= absoluteIntMutation(seed1.tertiaryColor[1],seed2.tertiaryColor[1], 1*radiation);
       newSeed.tertiaryColor[2]= absoluteIntMutation(seed1.tertiaryColor[2],seed2.tertiaryColor[2], 1*radiation);
       newSeed.branchDensity= absoluteFloatMutation(seed1.branchDensity,seed2.branchDensity, .01*radiation);
	   newSeed.leafDensity= absoluteFloatMutation(seed1.leafDensity,seed2.leafDensity, .01*radiation);
	   newSeed.leafSize= absoluteFloatMutation(seed1.leafSize,seed2.leafSize, .01*radiation);
       newSeed.angleVariance= absoluteFloatMutation(seed1.angleVariance,seed2.angleVariance, .01*radiation);
       newSeed.lengthVariance= absoluteFloatMutation(seed1.lengthVariance,seed2.lengthVariance, .01*radiation);
       newSeed.featureChance= absoluteFloatMutation(seed1.featureChance,seed2.featureChance, .01*radiation);
       newSeed.youth=absoluteIntMutation(seed1.youth,seed2.youth, 1*radiation);
       newSeed.adult=absoluteIntMutation(seed1.adult,seed2.adult, 1*radiation);
       newSeed.thickness = absoluteFloatMutation(seed1.thickness,seed2.thickness, .01*radiation);
       if(newSeed.thickness <= 0)
       {
    	   newSeed.thickness = .01;
       }
       newSeed.diameterloss = absoluteFloatMutation(seed1.diameterloss,seed2.diameterloss, .01*radiation);

       if(newSeed.diameterloss <= 0)
       {
           newSeed.diameterloss = .01;
       }

       if(newSeed.diameterloss >= .5)
       {
    	   newSeed.diameterloss = .49;
       }
       return newSeed;
}
 
seed generateSeed() //Completely new seed with no inheritance
{
       seed treeSeed;
       /*
              ~See Tree generation.h for the significance of these values~
       */
       treeSeed.branchDensity=randFloat(0.5,2);
       treeSeed.angleVariance=randFloat(10,30);
       treeSeed.featureChance=randFloat(0,.2);
       treeSeed.lengthVariance=randFloat(.1,2);
	   treeSeed.leafDensity=randFloat(.1,2);
 
       treeSeed.leafSize=randFloat(.01,1.00);
       treeSeed.canopyWeight=randFloat(0,10);
       treeSeed.youth = randInt(50,100);
       treeSeed.adult = randInt(treeSeed.youth+50, treeSeed.youth+100);
       treeSeed.thickness = randFloat(.1,1);
       treeSeed.diameterloss = randFloat(0,.1);
 
       treeSeed.primaryColor[0]=randInt(0,255);
       treeSeed.primaryColor[1]=randInt(0,255);
       treeSeed.primaryColor[2]=randInt(0,255);
       treeSeed.secondaryColor[0]=randInt(0,255);
       treeSeed.secondaryColor[1]=randInt(0,255);
       treeSeed.secondaryColor[2]=randInt(0,255);
       treeSeed.tertiaryColor[0]=randInt(0,255);
       treeSeed.tertiaryColor[1]=randInt(0,255);
       treeSeed.tertiaryColor[2]=randInt(0,255);
      
       //cout << "Seed Data:" << endl;
       //cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << endl;
       //cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << endl;
       //cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << endl;
       //cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance << " Length Variance: " << treeSeed.lengthVariance << endl;
       return treeSeed;
}
seed userDefinedSeed()
{
	seed treeSeed;
	cout << "Please enter values for: " << endl;
    /*
        ~See Tree generation.h for the significance of these values~
    */
	cout << "Branch Density:(0-2, decimals allowed) " << endl;
    cin >> treeSeed.branchDensity;
	cout << "Angle Variance:(0-90, decimals allowed)" << endl;
    cin >> treeSeed.angleVariance;
	cout << "Feature Chance:(0-1, decimals allowed) " << endl;
    cin >> treeSeed.featureChance;
	cout << "Length Variance:(0-2, decimals allowed) " << endl;
    cin >> treeSeed.lengthVariance;
	cout << "Leaf Density:(0-2, decimals allowed) " << endl;
	cin >> treeSeed.leafDensity;
	
	cout << "Leaf Size:(0-1, decimals allowed) " << endl;
    cin >> treeSeed.leafSize;
	cout << "Age at which the tree stops growing:(integer)" << endl;
    cin >> treeSeed.youth;
	cout << "Years after reaching maturity that the tree will live:" << endl;
    cin >> treeSeed.adult;
	cout << "Branch Thickness:(1-100, aspiring width of the trunk) " << endl;
	cin >> treeSeed.thickness;
 
	cout << "Primary Color:(0-255, integers) " << endl;
	cout << "R" << endl;
    cin >> treeSeed.primaryColor[0];
	cout << "G" << endl;
    cin >> treeSeed.primaryColor[1];
	cout << "B" << endl;
    cin >> treeSeed.primaryColor[2];
	cout << "Secondary Color:(0-255, integers) " << endl;
	cout << "R" << endl;
    cin >> treeSeed.secondaryColor[0];
	cout << "G" << endl;
    cin >> treeSeed.secondaryColor[1];
	cout << "B" << endl;
    cin >> treeSeed.secondaryColor[2];
	cout << "Tertiary Color:(0-255, integers) " << endl;
	cout << "R" << endl;
    cin >> treeSeed.tertiaryColor[0];
	cout << "G" << endl;
    cin >> treeSeed.tertiaryColor[1];
	cout << "B" << endl;
    cin >> treeSeed.tertiaryColor[2];
      
	cout << "Seed Data:" << endl;
	cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << endl;
	cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << endl;
	cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << endl;
	cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance << " Length Variance: " << treeSeed.lengthVariance << endl;
	cout << " Youth: " << treeSeed.youth << " Adult: " << treeSeed.adult << " Leaf Density: " << treeSeed.leafDensity << " Leaf Size: " << treeSeed.leafSize << endl;
	return treeSeed;
}
seed changeSeed(seed& treeSeed)
{
	string choice="y";
	while (choice=="y")
	{
		cout << "What variable would you like to edit?" << endl
		<< "(b)ranchDensity" << endl//The higher the value, the lower branches attach
		<< "(a)ngleVariance" << endl//Variation in angle
		<< "(f)eatureChance" << endl//likelihood of generating features
		<< "(l)engthVariance" << endl//lower value-> shorter branches

		<< "leaf(s)ize" << endl//size of leaf
		<< "leaf(d)ensity" << endl// Amount of leaf coverage per branch.
		<< "(y)outh" << endl// age that branch grows as many branches as possible
		<< "ad(u)lt" << endl// age that branch stops growing more branches and grows features instead
		<< "th(i)ckness" << endl;// aspiring thickness of the trunk

		string toEdit;
		cin >> toEdit;
		if (toEdit=="b")
		{
			float newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.branchDensity=newValue;
			
		}
		if (toEdit=="a")
		{
			float newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.angleVariance=newValue;
			
		}
		if (toEdit=="f")
		{
			float newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.featureChance=newValue;
			
		}
		if (toEdit=="l")
		{
			float newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.lengthVariance=newValue;
			
		}
		if (toEdit=="s")
		{
			float newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.leafSize=newValue;
			
		}
		if (toEdit=="d")
		{
			float newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.leafDensity=newValue;
			
		}
		if (toEdit=="y")
		{
			int newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.youth=newValue;
			
		}
		if (toEdit=="u")
		{
			float newValue;
			cout << "New Value? " << endl;
			cin >> newValue;
			treeSeed.adult=newValue;
		}
		cout << "Edit another variable? (y/n) " << endl;
		cin >> choice;
	}
	return treeSeed;
}
tree spawnTree(int x, int y, int z, seed& treeSeed, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, string plantID)
{     
	   tree newTree;
       newTree.sunlight=0;   //
       newTree.water=0;      //
       newTree.phosphorus=0; //Resources
       newTree.nitrogen=0;   //
       newTree.potassium=0;  //
	   newTree.sunlightcap=100;   //
       newTree.watercap=100;    //
       newTree.phosphoruscap=100; //Resource caps
       newTree.nitrogencap=100;   //
       newTree.potassiumcap=100;  //
       newTree.isAlive=true;
       newTree.reproduced=0;
	   newTree.fire=0;
       newTree.x=x;
       newTree.y=y;
       newTree.z=z;
	   newTree.roots=randInt(10,40);
	   PlantIDAssign(x, y, z, DimInfo, ResourceVector, plantID);
       newTree.age=0;
       newTree.treeSeed=treeSeed;
       newTree.thickness=treeSeed.thickness;
       branch trunk;
       trunk.connection=0;
       trunk.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
       trunk.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
       trunk.length=randFloat(1,40);
       trunk.feature=0;
	   //trunk.width=1
	   trunk.leafCount=randInt(0,10)*newTree.treeSeed.leafDensity;
       trunk.isAlive=1;
       trunk.diameter=1;
	   newTree.sunlightcap+=trunk.leafCount*treeSeed.leafSize*10;
	   newTree.watercap+=trunk.leafCount*treeSeed.leafSize*20;
	   newTree.phosphoruscap+=trunk.length*1.25;
       newTree.nitrogencap+=trunk.length/1.5;
       newTree.potassiumcap+=trunk.length*1.5;
       newTree.branches.push_back(trunk);
	   if (!PlantIDCheck(x, y, z, DimInfo, ResourceVector))
	   {
			newTree.isAlive=0;
	   }
       //newForest.trees.push_back(newTree);
       return newTree;
}
tree growBranch(tree& newTree, vector<VectorStruct>& ResourceVector,DimensionStruct DimInfo)
{
       if(newTree.sunlight>=10 && newTree.water>=20 && newTree.nitrogen>=15 && newTree.potassium>=30 && newTree.phosphorus>=25)
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
			newBranch.leafCount=randInt(0,10)*newTree.treeSeed.leafDensity;
			newBranch.isAlive=1;
			newBranch.diameter = newTree.branches.at(newTree.branches.size()-branchWeighting-1).diameter-newTree.treeSeed.diameterloss;
			if(newBranch.diameter <= 0)
			{
				newBranch.diameter = .01;
			}
			int featureChance=rand()%100;
			if(featureChance/100<newTree.treeSeed.featureChance)
			{
				newBranch.feature=randInt(1,4);
			}
			newTree.branches.push_back(newBranch);
			newTree.roots+=randInt(2,8);
			newTree.sunlightcap+=newBranch.leafCount*newTree.treeSeed.leafSize*10;
			newTree.watercap+=newBranch.leafCount*newTree.treeSeed.leafSize*20;
			newTree.phosphoruscap+=newBranch.length*1.25;
			newTree.nitrogencap+=newBranch.length/1.5;
			newTree.potassiumcap+=newBranch.length*1.5;
			newTree.sunlight=newTree.sunlight-(newBranch.length*.25+newBranch.feature*5);
			newTree.water=newTree.water+(ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "water", (newBranch.length*.5+newBranch.feature*5))-(newBranch.leafCount*(newTree.treeSeed.leafSize)));
			newTree.nitrogen=newTree.nitrogen+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "nitrogen", (newBranch.length*.375+newBranch.feature*10));
			newTree.potassium=newTree.potassium+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "potassium", (newBranch.length*.75));
			newTree.phosphorus=newTree.phosphorus+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "phosphorus", (newBranch.length*.625+newBranch.feature*7));
       }
       /* if(newTree.branches.size() == 20)
       {
              newTree.reproduced = 1;
       } */
 
 
       return newTree;
}
 
tree upkeep(tree& newTree, vector<VectorStruct>& ResourceVector,DimensionStruct DimInfo)
{
       //cout << "Upkeepan";
       int totalLength=newTree.roots;
       for(int i=0; i<newTree.branches.size(); i++)
       {
              totalLength+=newTree.branches.at(i).length;
              if(newTree.sunlight-totalLength*.005<0 || newTree.water-totalLength*.01<0 || newTree.nitrogen-totalLength*.0075<0 || newTree.potassium-totalLength*.015<0 || newTree.phosphorus-totalLength*.0125<0)
              {
              //cout << "test2";
                     for(int j=1; j<newTree.branches.size()-i; j++)
                     {
                           //cout << "killing a branch";
							newTree.branches.at(newTree.branches.size()-1).isAlive=0;
							newTree.sunlightcap-=newTree.branches.at(newTree.branches.size()-1).leafCount*newTree.treeSeed.leafSize*5;
							newTree.watercap-=newTree.branches.at(newTree.branches.size()-1).leafCount*newTree.treeSeed.leafSize*20;
							newTree.phosphoruscap-=newTree.branches.at(newTree.branches.size()-1).length*1.25;
							newTree.nitrogencap-=newTree.branches.at(newTree.branches.size()-1).length/1.5;
							newTree.potassiumcap-=newTree.branches.at(newTree.branches.size()-1).length*1.5;
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "water",-(newTree.branches.at(newTree.branches.size()-1).leafCount*newTree.treeSeed.leafSize*20));
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "nitrogen", -(newTree.branches.at(newTree.branches.size()-1).length/1.5));
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "potassium", -(newTree.branches.at(newTree.branches.size()-1).length*1.5));
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "phosphorus", -(newTree.branches.at(newTree.branches.size()-1).length*1.25));
							newTree.roots-=3;
							//cout << "test";
							newTree.branches.at(newTree.branches.at(newTree.branches.size()-1).connection-1).children.pop_back();
							//cout << "adding it to dead branches";
                            newTree.deadBranches.push_back(newTree.branches.at(newTree.branches.size()-1));
							//cout << "erasing from live branches";
							newTree.branches.pop_back();
							//cout << "Ash Ketchup";
                     }
                    
                     if(newTree.branches.size() == 1) //if the tree runs out of branches, it's dead.
                     {
                           //cout << "tree is kill";
                           newTree.isAlive = false;
                     }
              }
       }
       newTree.sunlight=newTree.sunlight-totalLength*.005;
      
       newTree.water=newTree.water-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "water",-(totalLength*.01));
       newTree.nitrogen=newTree.nitrogen-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "nitrogen", -(totalLength*.0075));
       newTree.potassium=newTree.potassium-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "potassium", -(totalLength*.015));
       newTree.phosphorus=newTree.phosphorus-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "phosphorus", -(totalLength*.0125));
       return newTree;
 
}
 
forest reaper(forest& newForest) //checks forest for trees with isAlive false, and moves them to deadtrees.
{
       for(int f = 0; f < newForest.trees.size(); f++)
       {
              newForest.trees.at(f).age += 1;
              /*
              if(newForest.trees.at(f).thickness < newForest.trees.at(f).treeSeed.thickness) //grow trees
              {
                  newForest.trees.at(f).thickness += (newForest.trees.at(f).treeSeed.thickness) / (newForest.trees.at(f).treeSeed.adult);
              }*/

              if(newForest.trees.at(f).isAlive == false || newForest.trees.at(f).age > newForest.trees.at(f).treeSeed.youth+newForest.trees.at(f).treeSeed.adult)
              {
                     //cout << "Don't fear the reaper";
                     newForest.deadtrees.push_back(newForest.trees.at(f));
                     newForest.trees.erase(newForest.trees.begin()+f);
              }
       }
       return newForest;
}

forest spark(forest& newForest, int x, int y, int z, int power)
{
	fire flame;
	flame.x = x;
	flame.y = y;
	flame.z = z;
	flame.fire = power;
	newForest.fires.push_back(flame);

	return newForest;
}

fire combust(tree& newTree)
{
	fire flame;
	flame.x = newTree.x;
	flame.y = newTree.y;
	flame.z = newTree.z;
	flame.fire = newTree.fire;
	cout << "tree has combusted!" << endl;
	return flame;
}

forest firefight(forest& newForest)
{
	for (int g = 0; g < newForest.trees.size(); g++)
	{
		if (newForest.trees.at(g).fire > 0)
		{
			newForest.trees.at(g).fire -= 10;
			newForest.trees.at(g).water -= 10;
			cout << "tree " << g << " is burning!" << newForest.trees.at(g).fire << " / " << newForest.trees.at(g).water << endl;

			if (newForest.trees.at(g).fire > 0 && newForest.trees.at(g).water <= 0)
			{
				newForest.trees.at(g).isAlive = false; //burn baby burn
				newForest.trees.at(g).fire += (newForest.trees.at(g).potassium + newForest.trees.at(g).nitrogen + newForest.trees.at(g).phosphorus);
				newForest.fires.push_back(combust(newForest.trees.at(g)));
			}
		}
	}

	for (int u = 0; u < newForest.fires.size(); u++)
	{
		newForest.fires.at(u).fire -= 1;

		if (newForest.fires.at(u).fire <= 0)
		{
			newForest.fires.erase(newForest.fires.begin() + u);
		}
		else
		{
			//spread the flames
			for (int k = 0; k < newForest.trees.size(); k++)
			{
				tree newTree = newForest.trees.at(k);
				fire newFire = newForest.fires.at(u);
				cout << "FIRE HAS SPREAD!";
				if (abs(newFire.x - newTree.x) < (newFire.fire / 10) && abs(newFire.y - newTree.y) < (newFire.fire / 10) && abs(newFire.z - newTree.z) < (newFire.fire / 10))
				{
					newForest.trees.at(k).fire = newForest.fires.at(u).fire;
					newForest.fires.at(u).fire = 0;
				}
			}
		}
	}

	return newForest;
}

forest reproduce(forest& newForest, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
       for(int f = 0; f < newForest.trees.size(); f++)
       {
              vector<tree> canReproduce;
              for(int g = 0; g < newForest.trees.at(f).branches.size(); g++)
              {
                     if(newForest.trees.at(f).branches.at(g).feature==3) //&& find(canReproduce.begin(),canReproduce.end(), newForest.trees.at(f))==canReproduce.end()
                     {
                           canReproduce.push_back(newForest.trees.at(f));
                           /* cout << "Tree: " << f << " Branch: "<< g<< " Reproduced" << endl;
                           newForest.trees.at(f).reproduced = 2;
                           tree oldTree = newForest.trees.at(f);
                           seed newSeed = generateSeed();
                           tree newTree = spawnTree(oldTree.x+randFloat(-1,1), oldTree.y, oldTree.z+randFloat(-1,1), newSeed, DimInfo, ResourceVector);
                           newForest.trees.push_back(newTree);*/
                           newForest.trees.at(f).branches.at(g).feature=0;
                     }
                     if(canReproduce.size()>=2)
                     {
                           seed newSeed=goForthAndMultiply(canReproduce.at(0).treeSeed, canReproduce.at(1).treeSeed);
                           canReproduce.erase(canReproduce.begin());
                           canReproduce.erase(canReproduce.begin());
						   int newX=newForest.trees.at(f).x+randInt(-30,30);
						   int newY=newForest.trees.at(f).y+randInt(-30,30);
						   int newZ=newForest.trees.at(f).z;
						   if (newX<0)
						   {
								newX=0;
						   }
						   if (newX>DimInfo.width-1)
						   {
								newX=DimInfo.width-1;
						   }
						   if (newY<0)
						   {
								newY=0;
						   }
						   if (newY>DimInfo.length-1)
						   {
								newY=DimInfo.length-1;
						   }
						   char PlantIDArray[10];
						   sprintf( PlantIDArray, "%d",newForest.trees.size());
						   string newPlantID=PlantIDArray;
                           tree newTree = spawnTree(newX, newY, newZ, newSeed, DimInfo, ResourceVector, newPlantID);
                           newForest.trees.push_back(newTree);
                     }
              }

       }
       return newForest;
}
forest generateTree(forest& newForest, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector,int turns)
{
	 for(int turn=0;turn<turns; turn++)
	 {
		  cout << "Turn " << turn << endl;
		  newForest.trees.at(0).sunlight=newForest.trees.at(0).sunlight+(newForest.trees.at(0).sunlightcap*.1);
		  if(newForest.trees.at(0).sunlight > newForest.trees.at(0).sunlightcap)
		  {
			//cout << "threw out " <<newForest.trees.at(0).sunlightcap-newForest.trees.at(0).sunlight << " sunlight.";
			newForest.trees.at(0).sunlight = newForest.trees.at(0).sunlightcap;
		  }
		  if(WaterGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots)
		  {
			newForest.trees.at(0).water=newForest.trees.at(0).water+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "water", -newForest.trees.at(0).roots);
		  } else {
			newForest.trees.at(0).water=newForest.trees.at(0).water+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "water", -WaterGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }
		  if(newForest.trees.at(0).water > newForest.trees.at(0).watercap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "water", -(newForest.trees.at(0).watercap-newForest.trees.at(0).water)) ;
			newForest.trees.at(0).water = newForest.trees.at(0).watercap;
		  }
		  if(PotassiumGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots)
		  {
			newForest.trees.at(0).potassium=newForest.trees.at(0).potassium+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "potassium", -newForest.trees.at(0).roots);
		  } else {
			newForest.trees.at(0).potassium=newForest.trees.at(0).potassium+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "potassium", -PotassiumGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(0).potassium > newForest.trees.at(0).potassiumcap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "potassium", -(newForest.trees.at(0).potassiumcap-newForest.trees.at(0).potassium));
			newForest.trees.at(0).potassium = newForest.trees.at(0).potassiumcap;
		  }
		  if(PhosphorusGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots)
		  {
			newForest.trees.at(0).phosphorus=newForest.trees.at(0).phosphorus+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "phosphorus", -newForest.trees.at(0).roots);
		  } else {
			newForest.trees.at(0).phosphorus=newForest.trees.at(0).phosphorus+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "phosphorus", -PhosphorusGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(0).phosphorus > newForest.trees.at(0).phosphoruscap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "phosphorus", -(newForest.trees.at(0).phosphoruscap-newForest.trees.at(0).phosphorus));
			newForest.trees.at(0).phosphorus = newForest.trees.at(0).phosphoruscap;
		  }
		  if(NitrogenGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots)
		  {
			newForest.trees.at(0).nitrogen=newForest.trees.at(0).nitrogen+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "nitrogen", -newForest.trees.at(0).roots);
		  } else {
			newForest.trees.at(0).nitrogen=newForest.trees.at(0).nitrogen+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "nitrogen", -NitrogenGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(0).nitrogen > newForest.trees.at(0).nitrogencap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "nitrogen", -(newForest.trees.at(0).nitrogencap-newForest.trees.at(0).nitrogen));
			newForest.trees.at(0).nitrogen = newForest.trees.at(0).nitrogencap;
		  }
		   while(newForest.trees.at(0).sunlight>=10 && newForest.trees.at(0).water>=20 && newForest.trees.at(0).nitrogen>=15 && newForest.trees.at(0).potassium>=30 && newForest.trees.at(0).phosphorus>=25)
		   {
				  newForest.trees.at(0)=growBranch(newForest.trees.at(0), ResourceVector, DimInfo);
				  newForest.trees.at(0)=upkeep(newForest.trees.at(0), ResourceVector, DimInfo);
		   }

		   for(int f=0;f<newForest.trees.at(0).branches.size();f++) //prints out tree data
		   {
				  cout << "Branch #"  << f+1 << ": " ;
				  cout << "Connection Point: " << newForest.trees.at(0).branches.at(f).connection << " ";
				  cout << "X Angle: " << newForest.trees.at(0).branches.at(f).xAngle << " ";
				  cout << "Y Angle: " << newForest.trees.at(0).branches.at(f).yAngle << " ";
				  cout <<  "Length: " << newForest.trees.at(0).branches.at(f).length << " ";
				  cout << "Feature: " << newForest.trees.at(0).branches.at(f).feature << " " << endl;
				  cout << "Children: ";
				  for(int g=0;g<newForest.trees.at(0).branches.at(f).children.size();g++)
				  {
						 cout << newForest.trees.at(0).branches.at(f).children.at(g) <<" ";
				  }
				  cout << endl;
		   }
		   cout << "Sunlight: " << newForest.trees.at(0).sunlight << " Water: " << newForest.trees.at(0).water << " Phosphorus: " << newForest.trees.at(0).phosphorus << " Potassium: " << newForest.trees.at(0).potassium << " Nitrogen: " << newForest.trees.at(0).nitrogen << endl;
		}
		newForest.trees.at(0).isAlive=0;
		return newForest;
}
forest generateForest(forest& newForest, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector, int turn, int turnstogo, int target)
{         
	/*
	turn += 1;
	cout << "Turn: " << turn << endl;
	cout << "Turnstogo: " << turnstogo << endl;*/

	/* feed-=decay;
	if (feed<=0)
	{
		  feed=0;
	} */
	for(int f=0; f<newForest.trees.size(); f++) //feed the trees
	{
		  //cout << f;
		  newForest.trees.at(f).sunlight=newForest.trees.at(f).sunlight+(newForest.trees.at(f).sunlightcap*.1);

		  if(newForest.trees.at(f).sunlight > newForest.trees.at(f).sunlightcap)
		  {
			//cout << "threw out " <<newForest.trees.at(f).sunlightcap-newForest.trees.at(f).sunlight << " sunlight.";
			newForest.trees.at(f).sunlight = newForest.trees.at(f).sunlightcap;
		  }
		  if(WaterGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots)
		  {
			newForest.trees.at(f).water=newForest.trees.at(f).water+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "water", -newForest.trees.at(f).roots);
		  } else {
			newForest.trees.at(f).water=newForest.trees.at(f).water+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "water", -WaterGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector));
		  }
		  if(newForest.trees.at(f).water > newForest.trees.at(f).watercap)
		  {
			ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "water", -(newForest.trees.at(f).watercap-newForest.trees.at(f).water)) ;
			newForest.trees.at(f).water = newForest.trees.at(f).watercap;
		  }
		  if(PotassiumGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots)
		  {
			newForest.trees.at(f).potassium=newForest.trees.at(f).potassium+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "potassium", -newForest.trees.at(f).roots);
		  } else {
			newForest.trees.at(f).potassium=newForest.trees.at(f).potassium+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "potassium", -PotassiumGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(f).potassium > newForest.trees.at(f).potassiumcap)
		  {
			ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "potassium", -(newForest.trees.at(f).potassiumcap-newForest.trees.at(f).potassium));
			newForest.trees.at(f).potassium = newForest.trees.at(f).potassiumcap;
		  }
		  if(PhosphorusGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots)
		  {
			newForest.trees.at(f).phosphorus=newForest.trees.at(f).phosphorus+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "phosphorus", -newForest.trees.at(f).roots);
		  } else {
			newForest.trees.at(f).phosphorus=newForest.trees.at(f).phosphorus+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "phosphorus", -PhosphorusGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(f).phosphorus > newForest.trees.at(f).phosphoruscap)
		  {
			ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "phosphorus", -(newForest.trees.at(f).phosphoruscap-newForest.trees.at(f).phosphorus));
			newForest.trees.at(f).phosphorus = newForest.trees.at(f).phosphoruscap;
		  }
		  if(NitrogenGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots)
		  {
			newForest.trees.at(f).nitrogen=newForest.trees.at(f).nitrogen+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "nitrogen", -newForest.trees.at(f).roots);
		  } else {
			newForest.trees.at(f).nitrogen=newForest.trees.at(f).nitrogen+ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "nitrogen", -NitrogenGrab(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector));
		  }
		  if(newForest.trees.at(f).nitrogen > newForest.trees.at(f).nitrogencap)
		  {
			ResourceChange(newForest.trees.at(f).x, newForest.trees.at(f).y, newForest.trees.at(f).z, DimInfo, ResourceVector, "nitrogen", -(newForest.trees.at(f).nitrogencap-newForest.trees.at(f).nitrogen));
			newForest.trees.at(f).nitrogen = newForest.trees.at(f).nitrogencap;
		  }
		  //let the trees do their tree thing
		  newForest.trees.at(f)=upkeep(newForest.trees.at(f), ResourceVector, DimInfo);
		  while(newForest.trees.at(f).age < newForest.trees.at(f).treeSeed.youth && newForest.trees.at(f).sunlight>=10 && newForest.trees.at(f).water>=20 && newForest.trees.at(f).nitrogen>=15 && newForest.trees.at(f).potassium>=30 && newForest.trees.at(f).phosphorus>=25)
		  {
				 newForest.trees.at(f)=growBranch(newForest.trees.at(f), ResourceVector, DimInfo);
				 
				 //cout << "Tree " << f+1 << " grew a branch."<< endl;
		  }

		  //cout << "Tree " << f+1 << " done."<< endl;
	}
	newForest = reproduce(newForest, DimInfo, ResourceVector);
	newForest = reaper(newForest);
	newForest = firefight(newForest);


	
	cout << "Number of trees: " << newForest.trees.size() << endl;
	/*cout << "Number of dead trees: " << newForest.deadtrees.size() << endl;
	
	for(int f=0; f<newForest.trees.size();f++)
	{
		  cout << "Tree number: " << f+1 << endl;
		  cout << "Number of Branches: " << newForest.trees.at(f).branches.size() << endl;
		  cout << "Number of Dead Branches: " << newForest.trees.at(f).deadBranches.size() << endl;
	}*/
	turnstogo -= 1;
	return newForest;
		  
}

forest userInteraction(int interaction, int target, forest& newForest, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{
	if (interaction==1)
   {
		 cout << "FIRE FIRE FIRE!!!" << endl;
		 cout << "POWER OF FIRE: " << endl;
		 int power;
		 cin >> power;
		 cout << "FIRE: " << endl;
		 cout << "X: " << newForest.trees.at(target-1).x << endl;
		 cout << "Y: " << newForest.trees.at(target-1).y << endl;
		 cout << "Z: " << newForest.trees.at(target-1).z << endl;
		 cout << "POWER: " << power << endl;
		 cout << "CONFIRM? (y/n)" << endl;
		 char choosy;
		 cin >> choosy;
		 if (choosy == 'y')
		 {
			 spark(newForest, newForest.trees.at(target-1).x, newForest.trees.at(target-1).y, newForest.trees.at(target-1).z, power);
			 cout << "Fire has been started." <<endl;
		 }
   }
   if (interaction==2)
   {
		newForest.trees.at(target-1).isAlive=false;
		cout << "Tree " << target << " is dead. Because you killed it. You monster."<< endl;
   }
   if (interaction==3)
   {
		int waterToAdd;
		int phosphorusToAdd;
		int potassiumToAdd;
		int nitrogenToAdd;
		cout << "How much water are you adding?" << endl;
		cin >> waterToAdd;
		cout << "How much phosphorus are you adding?" << endl;
		cin >> phosphorusToAdd;
		cout << "How much potassium are you adding?" << endl;
		cin >> potassiumToAdd;
		cout << "How much nitrogen are you adding?" << endl;
		cin >> nitrogenToAdd;
		newForest.trees.at(target-1).water+=waterToAdd;
		newForest.trees.at(target-1).phosphorus+=phosphorusToAdd;
		newForest.trees.at(target-1).potassium+=potassiumToAdd;
		newForest.trees.at(target-1).nitrogen+=nitrogenToAdd;
		cout << "Tree " << target << " has been fed. Om Nom Nom."<< endl;
	}
	if (interaction==4)
	{
		int clonex;
		int cloney;
		int clonez;
		cout << "Where do you want to put the clone?"<< endl
		<< "X: " << endl;
		cin >> clonex;
		cout << "Y:" << endl;
		cin >> cloney;
		cout << "Z:" << endl;
		cin >> clonez;
		char PlantIDArray[10];
		sprintf( PlantIDArray, "%d",newForest.trees.size());
		string newPlantID=PlantIDArray;
		tree newTree=spawnTree(clonex ,cloney ,clonez , newForest.trees.at(target-1).treeSeed , DimInfo, ResourceVector, newPlantID);
		newForest.trees.push_back(newTree);

	}
	if (interaction==5)
	{
		cout << "WARNING: Weird things happen when you change the seed of a tree mid-growth. They will look even more unnatural than they already do." << endl;
		newForest.trees.at(target-1).treeSeed=changeSeed(newForest.trees.at(target-1).treeSeed);
	}
	if (interaction==99)
	{
		newForest.trees.at(target-1).isAlive=false;
		cout << "Unable to bear the existential dread of living in a simulation, Tree " << target << " killed itself."<< endl;
	}
}