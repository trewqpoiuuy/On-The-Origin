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
	   //float width;
 
       int leafCount; // number of leaves
       int wood; // wood stores phosphorus and nitrogen. Will change later. 0-100
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
 
       seed newSeed;
       newSeed = seed1;
       newSeed.primaryColor[0]= absoluteIntMutation(seed1.primaryColor[0],seed2.primaryColor[0], 1);
       newSeed.primaryColor[1]= absoluteIntMutation(seed1.primaryColor[1],seed2.primaryColor[1], 1);
       newSeed.primaryColor[2]= absoluteIntMutation(seed1.primaryColor[2],seed2.primaryColor[2], 1);
       newSeed.secondaryColor[0]= absoluteIntMutation(seed1.secondaryColor[0],seed2.secondaryColor[0], 1);
       newSeed.secondaryColor[1]= absoluteIntMutation(seed1.secondaryColor[1],seed2.secondaryColor[1], 1);
       newSeed.secondaryColor[2]= absoluteIntMutation(seed1.secondaryColor[2],seed2.secondaryColor[2], 1);
       newSeed.tertiaryColor[0]= absoluteIntMutation(seed1.tertiaryColor[0],seed2.tertiaryColor[0], 1);
       newSeed.tertiaryColor[1]= absoluteIntMutation(seed1.tertiaryColor[1],seed2.tertiaryColor[1], 1);
       newSeed.tertiaryColor[2]= absoluteIntMutation(seed1.tertiaryColor[2],seed2.tertiaryColor[2], 1);
       newSeed.branchDensity= absoluteFloatMutation(seed1.branchDensity,seed2.branchDensity, .01);
	   newSeed.leafDensity= absoluteFloatMutation(seed1.leafDensity,seed2.leafDensity, .01);
	   newSeed.leafSize= absoluteFloatMutation(seed1.leafSize,seed2.leafSize, .01);
       newSeed.angleVariance= absoluteFloatMutation(seed1.angleVariance,seed2.angleVariance, .01);
       newSeed.lengthVariance= absoluteFloatMutation(seed1.lengthVariance,seed2.lengthVariance, .01);
       newSeed.featureChance= absoluteFloatMutation(seed1.featureChance,seed2.featureChance, .01);
       newSeed.youth=absoluteIntMutation(seed1.youth,seed2.youth, 1);
       newSeed.adult=absoluteIntMutation(seed1.adult,seed2.adult, 1);
 
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
tree spawnTree(int x, int y, int z, seed& treeSeed, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
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
	   newTree.roots=randInt(10,40);
       newTree.z=z;
       newTree.age=0;
       newTree.treeSeed=treeSeed;
       branch trunk;
       trunk.connection=0;
       trunk.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
       trunk.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
       trunk.length=randFloat(1,40);
       trunk.feature=0;
	   //trunk.width=1
	   trunk.leafCount=randInt(0,10)*newTree.treeSeed.leafDensity;
       trunk.isAlive=1;
	   newTree.sunlightcap+=trunk.leafCount*treeSeed.leafSize*10;
	   newTree.watercap+=trunk.leafCount*treeSeed.leafSize*20;
	   newTree.phosphoruscap+=trunk.length*1.25;
       newTree.nitrogencap+=trunk.length/1.5;
       newTree.potassiumcap+=trunk.length*1.5;
       newTree.branches.push_back(trunk);
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
              if(newTree.sunlight-totalLength*.0025<0 || newTree.water-totalLength*.005<0 || newTree.nitrogen-totalLength*.00375<0 || newTree.potassium-totalLength*.0075<0 || newTree.phosphorus-totalLength*.00625<0)
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
       newTree.sunlight=newTree.sunlight-totalLength*.0025;
      
       newTree.water=newTree.water-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "water",-(totalLength*.005));
       newTree.nitrogen=newTree.nitrogen-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "nitrogen", -(totalLength*.00375));
       newTree.potassium=newTree.potassium-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "potassium", -(totalLength*.0075));
       newTree.phosphorus=newTree.phosphorus-ResourceChange(newTree.x, newTree.y, newTree.z,  DimInfo, ResourceVector, "phosphorus", -(totalLength*.00625));
       return newTree;
 
}
 
 
 
forest reaper(forest& newForest) //checks forest for trees with isAlive false, and moves them to deadtrees.
{
       for(int f = 0; f < newForest.trees.size(); f++)
       {
              newForest.trees.at(f).age += 1;
              if(newForest.trees.at(f).isAlive == false || newForest.trees.at(f).age > newForest.trees.at(f).treeSeed.youth+newForest.trees.at(f).treeSeed.adult)
              {
                     cout << "Don't fear the reaper";
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
                           tree newTree = spawnTree(newForest.trees.at(f).x+randInt(-3,3), newForest.trees.at(f).y+randInt(-3,3), newForest.trees.at(f).z, newSeed, DimInfo, ResourceVector);
                           newForest.trees.push_back(newTree);
                     }
              }

       }
       return newForest;
}
forest generateForest(forest& newForest, DimensionStruct DimInfo, vector<VectorStruct>& ResourceVector)
{         
	int turn = 0;
    int turnstogo = 0;
	int target = 0;
	cout << "Analyse a tree? (negative # to not analyse anything)" << endl;
	cin >> target;
	 if (target>0 && target<=newForest.trees.size() && target != 1337)
	 {
		   seed treeSeed = newForest.trees.at(target-1).treeSeed;
		   cout << "Seed Data:" << endl;
		   cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << endl;
		   cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << endl;
		   cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << endl;
		   cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance << " Length Variance: " << treeSeed.lengthVariance << endl;
		   cout << "Age: " << newForest.trees.at(target-1).age << " Youth: " << treeSeed.youth << " Adult: " << treeSeed.adult << " Leaf Density: " << treeSeed.leafDensity << " Leaf Size: " << treeSeed.leafSize << endl;
		   cout << "sunlight: " << newForest.trees.at(target-1).sunlight <<" water: " << newForest.trees.at(target-1).water <<" potassium: " << newForest.trees.at(target-1).potassium <<" phosphorus: " << newForest.trees.at(target-1).phosphorus <<" nitrogen: " << newForest.trees.at(target-1).nitrogen<< endl;
		   cout << "sunlight cap: " << newForest.trees.at(target-1).sunlightcap <<" water cap: " << newForest.trees.at(target-1).watercap <<" potassium cap: " << newForest.trees.at(target-1).potassiumcap <<" phosphorus cap: " << newForest.trees.at(target-1).phosphoruscap <<" nitrogen cap: " << newForest.trees.at(target-1).nitrogencap<< endl;
	}

	 if (target == 1337)
	 {
		 cout << "FIRE FIRE FIRE!!!" << endl;
		 cout << "X COORDINATE: " << endl;
		 int xx;
		 cin >> xx;
		 cout << "Y COORDINATE: " << endl;
		 int yy;
		 cin >> yy;
		 cout << "Z COORDINATE: " << endl;
		 int zz;
		 cin >> zz;
		 cout << "POWER OF FIRE: " << endl;
		 int power;
		 cin >> power;
		 cout << "FIRE: " << endl;
		 cout << "X: " << xx << endl;
		 cout << "Y: " << yy << endl;
		 cout << "Z: " << zz << endl;
		 cout << "POWER: " << power << endl;
		 cout << "CONFIRM? (y/n)" << endl;
		 char choosy;
		 cin >> choosy;
		 if (choosy == 'y')
		 {
			 spark(newForest, xx, yy, zz, power);
			 cout << "Fire has been started.";
		 }

	 }

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
				  while(newForest.trees.at(f).age < newForest.trees.at(f).treeSeed.youth && newForest.trees.at(f).sunlight>=10 && newForest.trees.at(f).water>=20 && newForest.trees.at(f).nitrogen>=15 && newForest.trees.at(f).potassium>=30 && newForest.trees.at(f).phosphorus>=25)
				  {
						 newForest.trees.at(f)=growBranch(newForest.trees.at(f), ResourceVector, DimInfo);
						 newForest.trees.at(f)=upkeep(newForest.trees.at(f), ResourceVector, DimInfo);
						 //cout << "Tree " << f+1 << " grew a branch."<< endl;
				  }
				 
				  //cout << "Tree " << f+1 << " done."<< endl;
		   }
		   newForest = reproduce(newForest, DimInfo, ResourceVector);
		   newForest = reaper(newForest);
		   newForest = firefight(newForest);



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
	return newForest;
		  
}
