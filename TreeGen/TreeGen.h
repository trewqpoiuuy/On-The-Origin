#include <iostream>
#include <vector>
#include <tuple>
#include <string>
#include <cstdlib>
#include <ctime>
#include <tuple>
#define PI 3.14159265
#define treeMax 3000
#define rootsOn 0

using namespace veclib;

namespace TreeGen {

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

std::tuple<int,int,int> sphericalToCartesian(float xAngle,float yAngle,int length, int originX, int originY, int originZ)
{
	int x=(length*cos(xAngle*(PI/180))*sin(yAngle*(PI/180)))+originX;
	int y=(length*sin(xAngle*(PI/180))*sin(yAngle*(PI/180)))+originY;
	int z=(length*cos(yAngle*(PI/180)))+originZ;
	tuple<int,int,int> coords (x,y,z);
	return coords;
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
       std::vector<int> children; //Branches connected to this one
 
       //3d stuff
       std::vector<float> xyzPos;
       std::vector<float> pRot;
};
#if rootsOn==1
struct root
{
       int connection; //What the branch connects to
       float xAngle; //
       float yAngle; //Polar coordinates relative to connection
       float length; //
       float diameter; // how thick the branch is compared to the original size. 1 = trunk, .1 = twig
	   //float width;
 
 
       bool isAlive;
 
       //3d stuff
       std::vector<float> xyzPos;
       std::vector<float> pRot;
};
#endif
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
       unsigned int sunlight;   //
       unsigned int water;      //
       unsigned int phosphorus; // Current Resources
       unsigned int nitrogen;   //
       unsigned int potassium;  //
       int sunlightcap;  //
       int watercap;     //
       int phosphoruscap; // Max capacity of resource that the tree can hold
       int nitrogencap;  // currently unused
       int potassiumcap; //
       int age;
       int x;
       int y;
       int z;
       bool isAlive;
       int reproduced; // 0 = nil 1 = ready 2 = done
	   #if rootsOn==0
	   int roots;
	   #endif

       float thickness; // thickness of the tree
	   #if rootsOn==1
       std::vector<std::tuple<int,int,int>> rootsIn;
	   std::vector<root> roots;
	   #endif
	   std::vector<branch> branches;
       std::vector<branch> deadBranches;
       seed treeSeed;
 
       //3d stuff (placeholder for determing color)
       float maxvals[3];
};
 
struct forest
{
       int treenum; //useless right now
       std::vector<tree> trees;
       std::vector<tree> deadtrees;
	   std::vector<fire> fires;
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
      
       //std::cout << "Seed Data:" << "\n";
       //std::cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << "\n";
       //std::cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << "\n";
       //std::cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << "\n";
       //std::cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance << " Length Variance: " << treeSeed.lengthVariance << "\n";
       return treeSeed;
}
seed userDefinedSeed()
{
	seed treeSeed;
	std::cout << "Please enter values for: " << "\n";
    /*
        ~See Tree generation.h for the significance of these values~
    */
	std::cout << "Branch Density:(0-2, decimals allowed) " << "\n";
    std::cin >> treeSeed.branchDensity;
	std::cout << "Angle Variance:(0-90, decimals allowed)" << "\n";
    std::cin >> treeSeed.angleVariance;
	std::cout << "Feature Chance:(0-1, decimals allowed) " << "\n";
    std::cin >> treeSeed.featureChance;
	std::cout << "Length Variance:(0-2, decimals allowed) " << "\n";
    std::cin >> treeSeed.lengthVariance;
	std::cout << "Leaf Density:(0-2, decimals allowed) " << "\n";
	std::cin >> treeSeed.leafDensity;
	
	std::cout << "Leaf Size:(0-1, decimals allowed) " << "\n";
    std::cin >> treeSeed.leafSize;
	std::cout << "Age at which the tree stops growing:(integer)" << "\n";
    std::cin >> treeSeed.youth;
	std::cout << "Years after reaching maturity that the tree will live:" << "\n";
    std::cin >> treeSeed.adult;
	std::cout << "Branch Thickness:(1-100, aspiring width of the trunk) " << "\n";
	std::cin >> treeSeed.thickness;
 
	std::cout << "Primary Color:(0-255, integers) " << "\n";
	std::cout << "R" << "\n";
    std::cin >> treeSeed.primaryColor[0];
	std::cout << "G" << "\n";
    std::cin >> treeSeed.primaryColor[1];
	std::cout << "B" << "\n";
    std::cin >> treeSeed.primaryColor[2];
	std::cout << "Secondary Color:(0-255, integers) " << "\n";
	std::cout << "R" << "\n";
    std::cin >> treeSeed.secondaryColor[0];
	std::cout << "G" << "\n";
    std::cin >> treeSeed.secondaryColor[1];
	std::cout << "B" << "\n";
    std::cin >> treeSeed.secondaryColor[2];
	std::cout << "Tertiary Color:(0-255, integers) " << "\n";
	std::cout << "R" << "\n";
    std::cin >> treeSeed.tertiaryColor[0];
	std::cout << "G" << "\n";
    std::cin >> treeSeed.tertiaryColor[1];
	std::cout << "B" << "\n";
    std::cin >> treeSeed.tertiaryColor[2];
      
	std::cout << "Seed Data:" << "\n";
	std::cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << "\n";
	std::cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << "\n";
	std::cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << "\n";
	std::cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance << " Length Variance: " << treeSeed.lengthVariance << "\n";
	std::cout << " Youth: " << treeSeed.youth << " Adult: " << treeSeed.adult << " Leaf Density: " << treeSeed.leafDensity << " Leaf Size: " << treeSeed.leafSize << "\n";
	return treeSeed;
}
seed changeSeed(seed& treeSeed)
{
	string choice="y";
	while (choice=="y")
	{
		std::cout << "What variable would you like to edit?" << "\n"
		<< "(b)ranchDensity" << "\n"//The higher the value, the lower branches attach
		<< "(a)ngleVariance" << "\n"//Variation in angle
		<< "(f)eatureChance" << "\n"//likelihood of generating features
		<< "(l)engthVariance" << "\n"//lower value-> shorter branches

		<< "leaf(s)ize" << "\n"//size of leaf
		<< "leaf(d)ensity" << "\n"// Amount of leaf coverage per branch.
		<< "(y)outh" << "\n"// age that branch grows as many branches as possible
		<< "ad(u)lt" << "\n"// age that branch stops growing more branches and grows features instead
		<< "th(i)ckness" << "\n";// aspiring thickness of the trunk

		string toEdit;
		std::cin >> toEdit;
		if (toEdit=="b")
		{
			float newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.branchDensity=newValue;
			
		}
		if (toEdit=="a")
		{
			float newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.angleVariance=newValue;
			
		}
		if (toEdit=="f")
		{
			float newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.featureChance=newValue;
			
		}
		if (toEdit=="l")
		{
			float newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.lengthVariance=newValue;
			
		}
		if (toEdit=="s")
		{
			float newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.leafSize=newValue;
			
		}
		if (toEdit=="d")
		{
			float newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.leafDensity=newValue;
			
		}
		if (toEdit=="y")
		{
			int newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.youth=newValue;
			
		}
		if (toEdit=="u")
		{
			float newValue;
			std::cout << "New Value? " << "\n";
			std::cin >> newValue;
			treeSeed.adult=newValue;
		}
		std::cout << "Edit another variable? (y/n) " << "\n";
		std::cin >> choice;
	}
	return treeSeed;
}
#if rootsOn==1
tree growRoot(tree& newTree, std::vector<VectorStruct>& ResourceVector,DimensionStruct DimInfo)
{
	root newRoot;
	newRoot.connection=0;
	newRoot.xAngle=randFloat(-newTree.treeSeed.angleVariance,newTree.treeSeed.angleVariance);
	newRoot.yAngle=180-randFloat(-newTree.treeSeed.angleVariance,newTree.treeSeed.angleVariance);
	newRoot.length=randFloat(1,20)*newTree.treeSeed.lengthVariance;
	tuple<int,int,int> cartCoords=sphericalToCartesian(newRoot.xAngle,newRoot.yAngle,newRoot.length, newTree.x, newTree.y, newTree.z);
	int x=get<0>(cartCoords);
	int y=get<1>(cartCoords);
	int z=get<2>(cartCoords);
	if (get<0>(cartCoords)<0)
	{
		tuple<int,int,int> temp1 (0,y,z);
		cartCoords=temp1;
	}
	if (get<0>(cartCoords)>DimInfo.width-1)
	{
		tuple<int,int,int> temp2 (DimInfo.width-1,y,z);
		cartCoords= temp2;
	}
	x=get<0>(cartCoords);
	if (get<1>(cartCoords)<0)
	{
		tuple<int,int,int> temp3 (x,0,z);
		cartCoords=temp3;
	}
	if (get<1>(cartCoords)>DimInfo.length-1)
	{
		tuple<int,int,int> temp4 (x,DimInfo.length-1,z);
		cartCoords=temp4;
	}
	if(newTree.rootsIn.size()==0)
	{
		//std::cout << "root";
		newTree.rootsIn.push_back(cartCoords);
	} else {
		for(int q=0;q<newTree.rootsIn.size();q++)
		{
			if(newTree.rootsIn.at(q)==cartCoords)
			{
				break;
			}
			if(q==newTree.rootsIn.size())
			{
				//std::cout << "root";
				newTree.rootsIn.push_back(cartCoords);
			}
		}
	}
	newRoot.isAlive=1;
	newRoot.diameter = randFloat(0.01, 0.5);
	newTree.roots.push_back(newRoot);
    return newTree;
}
#endif
tree spawnTree(int x, int y, int z, seed& treeSeed, DimensionStruct DimInfo, std::vector<VectorStruct>& ResourceVector, string plantID)
{      
	   tree newTree;
       newTree.sunlight=0;   //
       newTree.water=0;      //
       newTree.phosphorus=0; //Resources
       newTree.nitrogen=0;   //
       newTree.potassium=0;  //
	   newTree.sunlightcap=200;   //
       newTree.watercap=200;    //
       newTree.phosphoruscap=200; //Resource caps
       newTree.nitrogencap=200;   //
       newTree.potassiumcap=200;  //
       newTree.isAlive=true;
       newTree.reproduced=0;
	   newTree.fire=0;
       newTree.x=x;
       newTree.y=y;
       newTree.z=z;
	   int newRoots=randInt(5,20);
	   #if rootsOn==1
	   for(int q=0; q<newRoots; q++)
	   {
			newTree=growRoot(newTree, ResourceVector, DimInfo);
	   }
	   #endif
	   #if rootsOn==0
	   newTree.roots=newRoots;
	   #endif
	   PlantIDAssign(x, y, z, DimInfo, ResourceVector, plantID);
       newTree.age=0;
       newTree.treeSeed=treeSeed;
       newTree.thickness=treeSeed.thickness;
       branch trunk;
       trunk.connection=0;
       trunk.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
       trunk.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
       trunk.length=randFloat(10,40);
       trunk.feature=0;
	   //trunk.width=1
	   trunk.leafCount=randInt(0,10)*newTree.treeSeed.leafDensity;
       trunk.isAlive=1;
       trunk.diameter=1;
	   newTree.sunlightcap+=trunk.leafCount*treeSeed.leafSize*75;
	   newTree.watercap+=trunk.leafCount*treeSeed.leafSize*125;
	   newTree.phosphoruscap+=trunk.length*10;
       newTree.nitrogencap+=trunk.length/7.5;
       newTree.potassiumcap+=trunk.length*7.5;
       newTree.branches.push_back(trunk);
	   if (!PlantIDCheck(x, y, z, DimInfo, ResourceVector))
	   {
			newTree.isAlive=0;
	   }
       //newForest.trees.push_back(newTree);
       return newTree;
}
tree growBranch(tree& newTree, std::vector<VectorStruct>& ResourceVector,DimensionStruct DimInfo)
{
       if(newTree.sunlight>=10 && newTree.water>=20 && newTree.nitrogen>=20 && newTree.potassium>=30 && newTree.phosphorus>=30)
       {
			branch newBranch;
			int branchWeighting=randInt(0,newTree.branches.size())*newTree.treeSeed.branchDensity; //Weights connection points
			if(branchWeighting>=(int)newTree.branches.size())
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
			int newRoots=randInt(2,8);
			#if rootsOn==1
			for(int q=0; q<newRoots; q++)
		    {
				newTree=growRoot(newTree, ResourceVector, DimInfo);
		    }
			#endif
			#if rootsOn==0
			newTree.roots+=newRoots;
			#endif
			//newTree=growRoot(newTree, ResourceVector, DimInfo);
			newTree.sunlightcap+=newBranch.leafCount*newTree.treeSeed.leafSize*50;
			newTree.watercap+=newBranch.leafCount*newTree.treeSeed.leafSize*100;
			newTree.phosphoruscap+=newBranch.length*10;
			newTree.nitrogencap+=newBranch.length/7.5;
			newTree.potassiumcap+=newBranch.length*7.5;
			newTree.sunlight=newTree.sunlight-(newBranch.length*.25+newBranch.feature*3);
			newTree.water=newTree.water+(ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "water", (newBranch.length*.5+newBranch.feature*5))-(newBranch.leafCount*(newTree.treeSeed.leafSize)));
			newTree.nitrogen=newTree.nitrogen+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "nitrogen", (newBranch.length*.375+newBranch.feature*10));
			newTree.potassium=newTree.potassium+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "potassium", (newBranch.length*.75)+newBranch.feature*6);
			newTree.phosphorus=newTree.phosphorus+ResourceChange(newTree.x, newTree.y, newTree.z, DimInfo, ResourceVector, "phosphorus", (newBranch.length*.625+newBranch.feature*7));
       }
       /* if(newTree.branches.size() == 20)
       {
              newTree.reproduced = 1;
       } */
 
 
       return newTree;
}

 
tree upkeep(tree& newTree, std::vector<VectorStruct>& ResourceVector,DimensionStruct DimInfo)
{
       //std::cout << "Upkeepan";
	   #if rootsOn==1
       int totalLength=newTree.roots.size();
	   #else
	   int totalLength=newTree.roots;
	   #endif
       for(unsigned int i=0; i<newTree.branches.size(); i++)
       {
			 
              totalLength+=newTree.branches.at(i).length;
			  
              if(newTree.sunlight-totalLength*.05<0 || newTree.water-totalLength*.1<0 || newTree.nitrogen-totalLength*.075<0 || newTree.potassium-totalLength*.15<0 || newTree.phosphorus-totalLength*.125<0)
              {
              //std::cout << "test2";
                     for(unsigned int j=1; j<newTree.branches.size()-i; j++)
                     {
                           //std::cout << "killing a branch";
							newTree.branches.at(newTree.branches.size()-1).isAlive=0;
							newTree.sunlightcap-=newTree.branches.at(newTree.branches.size()-1).leafCount*newTree.treeSeed.leafSize*50;
							newTree.watercap-=newTree.branches.at(newTree.branches.size()-1).leafCount*newTree.treeSeed.leafSize*100;
							newTree.phosphoruscap-=newTree.branches.at(newTree.branches.size()-1).length*10;
							newTree.nitrogencap-=newTree.branches.at(newTree.branches.size()-1).length/7.5;
							newTree.potassiumcap-=newTree.branches.at(newTree.branches.size()-1).length*7.5;
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "water",-(newTree.branches.at(newTree.branches.size()-1).leafCount*newTree.treeSeed.leafSize*20));
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "nitrogen", -(newTree.branches.at(newTree.branches.size()-1).length/1.5));
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "potassium", -(newTree.branches.at(newTree.branches.size()-1).length*1.5));
							ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "phosphorus", -(newTree.branches.at(newTree.branches.size()-1).length*1.25));
							#if rootsOn==1
							newTree.roots.pop_back();
							newTree.roots.pop_back();
							newTree.roots.pop_back();
							#else
							newTree.roots-=3;
							#endif
							//std::cout << "test";
							newTree.branches.at(newTree.branches.at(newTree.branches.size()-1).connection-1).children.pop_back();
							//std::cout << "adding it to dead branches";
                            newTree.deadBranches.push_back(newTree.branches.at(newTree.branches.size()-1));
							//std::cout << "erasing from live branches";
							newTree.branches.pop_back();
							//std::cout << "Ash Ketchup";
                     }
                    
                     if(newTree.branches.size() == 1) //if the tree runs out of branches, it's dead.
                     {
                           //std::cout << "tree is kill";
                           newTree.isAlive = false;
                     }
              }
       }
       newTree.sunlight=newTree.sunlight-totalLength*.05;
      
       newTree.water=newTree.water-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "water",-(totalLength*.1));
       newTree.nitrogen=newTree.nitrogen-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "nitrogen", -(totalLength*.075));
       newTree.potassium=newTree.potassium-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "potassium", -(totalLength*.15));
       newTree.phosphorus=newTree.phosphorus-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "phosphorus", -(totalLength*.125));
       return newTree;
 
}
 
forest reaper(forest& newForest) //checks forest for trees with isAlive false, and moves them to deadtrees.
{
       for(unsigned int f = 0; f < newForest.trees.size(); f++)
       {
             // newForest.trees.at(f).age += 1;
              /*
              if(newForest.trees.at(f).thickness < newForest.trees.at(f).treeSeed.thickness) //grow trees
              {
                  newForest.trees.at(f).thickness += (newForest.trees.at(f).treeSeed.thickness) / (newForest.trees.at(f).treeSeed.adult);
              }*/
			  if(f>treeMax)
			  {
					newForest.trees.at(f).isAlive = false;
			  }
              if(newForest.trees.at(f).isAlive == false || newForest.trees.at(f).age > newForest.trees.at(f).treeSeed.youth+newForest.trees.at(f).treeSeed.adult)
              {
                     //std::cout << "Don't fear the reaper";
                     newForest.deadtrees.push_back(newForest.trees.at(f));
                     newForest.trees.erase(newForest.trees.begin()+f);
              }
       }
	   newForest.deadtrees.clear();
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
	std::cout << "tree has combusted!" << "\n";
	return flame;
}

forest firefight(forest& newForest)
{
	for (unsigned int g = 0; g < newForest.trees.size(); g++)
	{
		if (newForest.trees.at(g).fire > 0)
		{
			newForest.trees.at(g).fire -= 10;
			newForest.trees.at(g).water -= 10;
			std::cout << "tree " << g << " is burning!" << newForest.trees.at(g).fire << " / " << newForest.trees.at(g).water << "\n";

			if (newForest.trees.at(g).fire > 0 && newForest.trees.at(g).water <= 0)
			{
				newForest.trees.at(g).isAlive = false; //burn baby burn
				newForest.trees.at(g).fire += (newForest.trees.at(g).potassium + newForest.trees.at(g).nitrogen + newForest.trees.at(g).phosphorus);
				newForest.fires.push_back(combust(newForest.trees.at(g)));
			}
		}
	}

	for (unsigned int u = 0; u < newForest.fires.size(); u++)
	{
		newForest.fires.at(u).fire -= 1;

		if (newForest.fires.at(u).fire <= 0)
		{
			newForest.fires.erase(newForest.fires.begin() + u);
		}
		else
		{
			//spread the flames
			for (unsigned int k = 0; k < newForest.trees.size(); k++)
			{
				tree newTree = newForest.trees.at(k);
				fire newFire = newForest.fires.at(u);
				std::cout << "FIRE HAS SPREAD!";
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

forest reproduce(forest& newForest, DimensionStruct DimInfo, std::vector<VectorStruct>& ResourceVector)
{
    
	for(unsigned int f = 0; f < newForest.trees.size(); f++)
   {
		  if (newForest.trees.size()>treeMax)
		  {
			break;
		  }
		  std::vector<tree> canReproduce;
		  for(unsigned int g = 0; g < newForest.trees.at(f).branches.size(); g++)
		  {
				 if(newForest.trees.at(f).branches.at(g).feature==3) //&& find(canReproduce.begin(),canReproduce.end(), newForest.trees.at(f))==canReproduce.end()
				 {
					   canReproduce.push_back(newForest.trees.at(f));
					   /* std::cout << "Tree: " << f << " Branch: "<< g<< " Reproduced" << "\n";
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

forest generateTree(forest& newForest, DimensionStruct DimInfo, std::vector<VectorStruct>& ResourceVector,int turns)
{
	#if rootsOn==1
	 for(int turn=0;turn<turns; turn++)
	 {
		  std::cout << "Turn " << turn << "\n";
		  newForest.trees.at(0).sunlight=newForest.trees.at(0).sunlight+(newForest.trees.at(0).sunlightcap*.1);
		  if(newForest.trees.at(0).sunlight > newForest.trees.at(0).sunlightcap)
		  {
			//std::cout << "threw out " <<newForest.trees.at(0).sunlightcap-newForest.trees.at(0).sunlight << " sunlight.";
			newForest.trees.at(0).sunlight = newForest.trees.at(0).sunlightcap;
		  }
		  if(WaterGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots.size())
		  {
			newForest.trees.at(0).water=newForest.trees.at(0).water+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "water", -newForest.trees.at(0).roots.size());
		  } else {
			newForest.trees.at(0).water=newForest.trees.at(0).water+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "water", -WaterGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }
		  if(newForest.trees.at(0).water > newForest.trees.at(0).watercap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "water", -(newForest.trees.at(0).watercap-newForest.trees.at(0).water)) ;
			newForest.trees.at(0).water = newForest.trees.at(0).watercap;
		  }
		  if(PotassiumGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots.size())
		  {
			newForest.trees.at(0).potassium=newForest.trees.at(0).potassium+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "potassium", -newForest.trees.at(0).roots.size());
		  } else {
			newForest.trees.at(0).potassium=newForest.trees.at(0).potassium+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "potassium", -PotassiumGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(0).potassium > newForest.trees.at(0).potassiumcap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "potassium", -(newForest.trees.at(0).potassiumcap-newForest.trees.at(0).potassium));
			newForest.trees.at(0).potassium = newForest.trees.at(0).potassiumcap;
		  }
		  if(PhosphorusGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots.size())
		  {
			newForest.trees.at(0).phosphorus=newForest.trees.at(0).phosphorus+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "phosphorus", -newForest.trees.at(0).roots.size());
		  } else {
			newForest.trees.at(0).phosphorus=newForest.trees.at(0).phosphorus+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "phosphorus", -PhosphorusGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(0).phosphorus > newForest.trees.at(0).phosphoruscap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "phosphorus", -(newForest.trees.at(0).phosphoruscap-newForest.trees.at(0).phosphorus));
			newForest.trees.at(0).phosphorus = newForest.trees.at(0).phosphoruscap;
		  }
		  if(NitrogenGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector)> newForest.trees.at(0).roots.size())
		  {
			newForest.trees.at(0).nitrogen=newForest.trees.at(0).nitrogen+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "nitrogen", -newForest.trees.at(0).roots.size());
		  } else {
			newForest.trees.at(0).nitrogen=newForest.trees.at(0).nitrogen+ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "nitrogen", -NitrogenGrab(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector));
		  }

		  if(newForest.trees.at(0).nitrogen > newForest.trees.at(0).nitrogencap)
		  {
			ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "nitrogen", -(newForest.trees.at(0).nitrogencap-newForest.trees.at(0).nitrogen));
			newForest.trees.at(0).nitrogen = newForest.trees.at(0).nitrogencap;
		  }
		  #else
		  for(int turn=0;turn<turns; turn++)
	 {
		  std::cout << "Turn " << turn << "\n";
		  newForest.trees.at(0).sunlight=newForest.trees.at(0).sunlight+(newForest.trees.at(0).sunlightcap*.1);
		  if(newForest.trees.at(0).sunlight > newForest.trees.at(0).sunlightcap)
		  {
			//std::cout << "threw out " <<newForest.trees.at(0).sunlightcap-newForest.trees.at(0).sunlight << " sunlight.";
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
		  #endif
		   while(newForest.trees.at(0).sunlight>=10 && newForest.trees.at(0).water>=20 && newForest.trees.at(0).nitrogen>=15 && newForest.trees.at(0).potassium>=30 && newForest.trees.at(0).phosphorus>=25)
		   {
				  newForest.trees.at(0)=growBranch(newForest.trees.at(0), ResourceVector, DimInfo);
				  newForest.trees.at(0)=upkeep(newForest.trees.at(0), ResourceVector, DimInfo);
		   }

		   for(int f=0;f<newForest.trees.at(0).branches.size();f++) //prints out tree data
		   {
				  std::cout << "Branch #"  << f+1 << ": " ;
				  std::cout << "Connection Point: " << newForest.trees.at(0).branches.at(f).connection << " ";
				  std::cout << "X Angle: " << newForest.trees.at(0).branches.at(f).xAngle << " ";
				  std::cout << "Y Angle: " << newForest.trees.at(0).branches.at(f).yAngle << " ";
				  std::cout <<  "Length: " << newForest.trees.at(0).branches.at(f).length << " ";
				  std::cout << "Feature: " << newForest.trees.at(0).branches.at(f).feature << " " << "\n";
				  std::cout << "Children: ";
				  for(unsigned int g=0;g<newForest.trees.at(0).branches.at(f).children.size();g++)
				  {
						 std::cout << newForest.trees.at(0).branches.at(f).children.at(g) <<" ";
				  }
				  std::cout << "\n";
		   }
		   std::cout << "Sunlight: " << newForest.trees.at(0).sunlight << " Water: " << newForest.trees.at(0).water << " Phosphorus: " << newForest.trees.at(0).phosphorus << " Potassium: " << newForest.trees.at(0).potassium << " Nitrogen: " << newForest.trees.at(0).nitrogen << "\n";
		}
		newForest.trees.at(0).isAlive=0;
		return newForest;
}
forest generateForest(forest& newForest, DimensionStruct DimInfo, std::vector<VectorStruct>& ResourceVector, int turn, int turnstogo, int target)
{         
	/*
	turn += 1;
	std::cout << "Turn: " << turn << "\n";
	std::cout << "Turnstogo: " << turnstogo << "\n";*/

	/* feed-=decay;
	if (feed<=0)
	{
		  feed=0;
	} */
	std::cout << "start";
	int rootRate=1;
	for(unsigned int f=0; f<newForest.trees.size(); f++) //feed the trees
	{
		  //std::cout << f;
		  newForest.trees.at(f).sunlight=newForest.trees.at(f).sunlight+(newForest.trees.at(f).sunlightcap*.1);

		  if(newForest.trees.at(f).sunlight > newForest.trees.at(f).sunlightcap*.1)
		  {
			//std::cout << "threw out " <<newForest.trees.at(f).sunlightcap-newForest.trees.at(f).sunlight << " sunlight.";
			newForest.trees.at(f).sunlight = newForest.trees.at(f).sunlightcap*.1;
		  }
		#if rootsOn==1
		for(unsigned int v=0; v<newForest.trees.at(f).rootsIn.size(); v++)
		{
			
			//std::cout << v;
			//std::cout << get<0>(newForest.trees.at(f).rootsIn.at(v)) << " " << get<1>(newForest.trees.at(f).rootsIn.at(v)) << " " <<get<2>(newForest.trees.at(f).rootsIn.at(v));
			if(WaterGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector)> newForest.trees.at(f).roots.size()*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).water=newForest.trees.at(f).water+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "water", -newForest.trees.at(f).roots.size()*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).water=newForest.trees.at(f).water+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "water", -WaterGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector));
			}
			if(newForest.trees.at(f).water > newForest.trees.at(f).watercap)
			{
			ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "water", -(newForest.trees.at(f).watercap-newForest.trees.at(f).water)) ;
			newForest.trees.at(f).water = newForest.trees.at(f).watercap;
			}
			if(PotassiumGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector)> newForest.trees.at(f).roots.size()*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).potassium=newForest.trees.at(f).potassium+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "potassium", -newForest.trees.at(f).roots.size()*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).potassium=newForest.trees.at(f).potassium+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "potassium", -PotassiumGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector));
			}

			if(newForest.trees.at(f).potassium > newForest.trees.at(f).potassiumcap)
			{
			ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "potassium", -(newForest.trees.at(f).potassiumcap-newForest.trees.at(f).potassium));
			newForest.trees.at(f).potassium = newForest.trees.at(f).potassiumcap;
			}
			if(PhosphorusGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector)> newForest.trees.at(f).roots.size()*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).phosphorus=newForest.trees.at(f).phosphorus+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "phosphorus", -newForest.trees.at(f).roots.size()*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).phosphorus=newForest.trees.at(f).phosphorus+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "phosphorus", -PhosphorusGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector));
			}

			if(newForest.trees.at(f).phosphorus > newForest.trees.at(f).phosphoruscap)
			{
			ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "phosphorus", -(newForest.trees.at(f).phosphoruscap-newForest.trees.at(f).phosphorus));
			newForest.trees.at(f).phosphorus = newForest.trees.at(f).phosphoruscap;
			}
			if(NitrogenGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector)> newForest.trees.at(f).roots.size()/newForest.trees.at(f).rootsIn.size()*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).nitrogen=newForest.trees.at(f).nitrogen+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "nitrogen", -newForest.trees.at(f).roots.size()*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).nitrogen=newForest.trees.at(f).nitrogen+ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "nitrogen", -NitrogenGrab(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector));
			}
			if(newForest.trees.at(f).nitrogen > newForest.trees.at(f).nitrogencap)
			{
			ResourceChange(get<0>(newForest.trees.at(f).rootsIn.at(v)), get<1>(newForest.trees.at(f).rootsIn.at(v)), get<2>(newForest.trees.at(f).rootsIn.at(v)), DimInfo, ResourceVector, "nitrogen", -(newForest.trees.at(f).nitrogencap-newForest.trees.at(f).nitrogen));
			newForest.trees.at(f).nitrogen = newForest.trees.at(f).nitrogencap;
			}
		}
			#else
			if(WaterGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).water=newForest.trees.at(f).water+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "water", -newForest.trees.at(f).roots*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).water=newForest.trees.at(f).water+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "water", -WaterGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector));
			}
			if(newForest.trees.at(f).water > newForest.trees.at(f).watercap)
			{
			ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "water", -(newForest.trees.at(f).watercap-newForest.trees.at(f).water)) ;
			newForest.trees.at(f).water = newForest.trees.at(f).watercap;
			}
			if(PotassiumGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).potassium=newForest.trees.at(f).potassium+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "potassium", -newForest.trees.at(f).roots*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).potassium=newForest.trees.at(f).potassium+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "potassium", -PotassiumGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector));
			}

			if(newForest.trees.at(f).potassium > newForest.trees.at(f).potassiumcap)
			{
			ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "potassium", -(newForest.trees.at(f).potassiumcap-newForest.trees.at(f).potassium));
			newForest.trees.at(f).potassium = newForest.trees.at(f).potassiumcap;
			}
			if(PhosphorusGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).phosphorus=newForest.trees.at(f).phosphorus+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "phosphorus", -newForest.trees.at(f).roots*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).phosphorus=newForest.trees.at(f).phosphorus+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "phosphorus", -PhosphorusGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector));
			}

			if(newForest.trees.at(f).phosphorus > newForest.trees.at(f).phosphoruscap)
			{
			ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "phosphorus", -(newForest.trees.at(f).phosphoruscap-newForest.trees.at(f).phosphorus));
			newForest.trees.at(f).phosphorus = newForest.trees.at(f).phosphoruscap;
			}
			if(NitrogenGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector)> newForest.trees.at(f).roots*rootRate)//newForest.trees.at(f).rootsIn.size())
			{
			newForest.trees.at(f).nitrogen=newForest.trees.at(f).nitrogen+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "nitrogen", -newForest.trees.at(f).roots*rootRate);//newForest.trees.at(f).rootsIn.size());
			} else {
			newForest.trees.at(f).nitrogen=newForest.trees.at(f).nitrogen+ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "nitrogen", -NitrogenGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector));
			}
			if(newForest.trees.at(f).nitrogen > newForest.trees.at(f).nitrogencap)
			{
			ResourceChange(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector, "nitrogen", -(newForest.trees.at(f).nitrogencap-newForest.trees.at(f).nitrogen));
			newForest.trees.at(f).nitrogen = newForest.trees.at(f).nitrogencap;
			}
			#endif
			//std::cout <<NitrogenGrab(newForest.trees.at(f).x,newForest.trees.at(f).y,newForest.trees.at(f).z, DimInfo, ResourceVector);
		  //let the trees do their tree thing
		  //std::cout << "Number of trees: " << newForest.trees.size() << "\n";
		  newForest.trees.at(f)=upkeep(newForest.trees.at(f), ResourceVector, DimInfo);
		  //std::cout << "Number of trees: " << newForest.trees.size() << "\n";
		  //std::cout << "Sunlight: " << newForest.trees.at(f).sunlight << " Water: " << newForest.trees.at(f).water << " Phosphorus: " << newForest.trees.at(f).phosphorus << " Potassium: " << newForest.trees.at(f).potassium << " Nitrogen: " << newForest.trees.at(f).nitrogen << "\n";
		  while(newForest.trees.at(f).age < newForest.trees.at(f).treeSeed.youth && newForest.trees.at(f).sunlight>=10 && newForest.trees.at(f).water>=20 && newForest.trees.at(f).nitrogen>=20 && newForest.trees.at(f).potassium>=30 && newForest.trees.at(f).phosphorus>=30)
		  {
				 newForest.trees.at(f)=growBranch(newForest.trees.at(f), ResourceVector, DimInfo);
				 
				 //std::cout << "Tree " << f+1 << " grew a branch."<< "\n";
		  }
		
		  //std::cout << "Tree " << f+1 << " has "<< newForest.trees.at(f).roots<<" roots." << "\n";
	}
	//std::cout << "reproduce";
	newForest = reproduce(newForest, DimInfo, ResourceVector);
	//std::cout << "reap";
	newForest = reaper(newForest);
	//std::cout << "fire";
	newForest = firefight(newForest);


	
	std::cout << "Number of trees: " << newForest.trees.size() << "\n";
	/* std::cout << "Number of dead trees: " << newForest.deadtrees.size() << "\n";
	
	for(int f=0; f<newForest.trees.size();f++)
	{
		  std::cout << "Tree number: " << f+1 << "\n";
		  std::cout << "Number of Branches: " << newForest.trees.at(f).branches.size() << "\n";
		  std::cout << "Number of Dead Branches: " << newForest.trees.at(f).deadBranches.size() << "\n";
	} */
	turnstogo -= 1;
	//std::cin >> turnstogo;
	return newForest;
		  
}

forest userInteraction(int interaction, int target, forest& newForest, DimensionStruct DimInfo, std::vector<VectorStruct>& ResourceVector)
{
	if (interaction==1)
   {
		 std::cout << "FIRE FIRE FIRE!!!" << "\n";
		 std::cout << "POWER OF FIRE: " << "\n";
		 int power;
		 std::cin >> power;
		 std::cout << "FIRE: " << "\n";
		 std::cout << "X: " << newForest.trees.at(target-1).x << "\n";
		 std::cout << "Y: " << newForest.trees.at(target-1).y << "\n";
		 std::cout << "Z: " << newForest.trees.at(target-1).z << "\n";
		 std::cout << "POWER: " << power << "\n";
		 std::cout << "CONFIRM? (y/n)" << "\n";
		 char choosy;
		 std::cin >> choosy;
		 if (choosy == 'y')
		 {
			 spark(newForest, newForest.trees.at(target-1).x, newForest.trees.at(target-1).y, newForest.trees.at(target-1).z, power);
			 std::cout << "Fire has been started." <<"\n";
		 }
   }
   if (interaction==2)
   {
		newForest.trees.at(target-1).isAlive=false;
		std::cout << "Tree " << target << " is dead. Because you killed it. You monster."<< "\n";
   }
   if (interaction==3)
   {
		int waterToAdd;
		int phosphorusToAdd;
		int potassiumToAdd;
		int nitrogenToAdd;
		std::cout << "How much water are you adding?" << "\n";
		std::cin >> waterToAdd;
		std::cout << "How much phosphorus are you adding?" << "\n";
		std::cin >> phosphorusToAdd;
		std::cout << "How much potassium are you adding?" << "\n";
		std::cin >> potassiumToAdd;
		std::cout << "How much nitrogen are you adding?" << "\n";
		std::cin >> nitrogenToAdd;
		newForest.trees.at(target-1).water+=waterToAdd;
		newForest.trees.at(target-1).phosphorus+=phosphorusToAdd;
		newForest.trees.at(target-1).potassium+=potassiumToAdd;
		newForest.trees.at(target-1).nitrogen+=nitrogenToAdd;
		std::cout << "Tree " << target << " has been fed. Om Nom Nom."<< "\n";
	}
	if (interaction==4)
	{
		int clonex;
		int cloney;
		int clonez;
		std::cout << "Where do you want to put the clone?"<< "\n"
		<< "X: " << "\n";
		std::cin >> clonex;
		std::cout << "Y:" << "\n";
		std::cin >> cloney;
		std::cout << "Z:" << "\n";
		std::cin >> clonez;
		char PlantIDArray[10];
		sprintf( PlantIDArray, "%d",newForest.trees.size());
		std::string newPlantID=PlantIDArray;
		tree newTree=spawnTree(clonex ,cloney ,clonez , newForest.trees.at(target-1).treeSeed , DimInfo, ResourceVector, newPlantID);
		newForest.trees.push_back(newTree);

	}
	if (interaction==5)
	{
		std::cout << "WARNING: Weird things happen when you change the seed of a tree mid-growth. They will look even more unnatural than they already do." << "\n";
		newForest.trees.at(target-1).treeSeed=changeSeed(newForest.trees.at(target-1).treeSeed);
	}
	if (interaction==99)
	{
		newForest.trees.at(target-1).isAlive=false;
		std::cout << "Unable to bear the existential dread of living in a simulation, Tree " << target << " killed itself."<< "\n";
	}
}

}
