//#include "stdafx.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../Resources/VectorIntLib.h" //delete 'Resources/' if broken
#include "../TreeGen/Tree generation.h"
#include "../TreeRender/TreeRender.h"
#include "../Engine/Engine.h"

using namespace std;
using namespace veclib;

int main(int argc, char **argv)
{
       srand(time(NULL));
       int uselessThing=rand(); //First value of rand is the seed, so this gets rid of it
       int depth = 10;
       int length = 200;
       int width = 200;
       int TopsoilDepth = 3;
       DimensionStruct DimInfo;
       DimInfo.length = length;
       DimInfo.width = width;
       DimInfo.depth = depth;
       DimInfo.TopsoilDepth = TopsoilDepth;
       vector<VectorStruct> ResourceVector=initializeResources(DimInfo);
       cout << "Water Test: " << WaterGrab(0,0,0,DimInfo, ResourceVector) << endl;
       forest newForest; //all trees

       int startingtrees;
	   cout << "How many trees to start?" << endl;
	   cin >> startingtrees;
       int originpointx = 100;
       int originpointy = 100;
       while(startingtrees > 0)
       {
              cout << "Spawning origin tree. " << endl;
              seed treeSeed=generateSeed();
              tree newTree=spawnTree(originpointx,originpointy,0,treeSeed, DimInfo, ResourceVector);
              newForest.trees.push_back(newTree);

              startingtrees -= 1;
              originpointx += randInt(-10,10);
              if(originpointx > length)
              {
                     originpointx = length;
              }
			  if(originpointx < 0)
              {
                     originpointx = 0;
              }
              originpointy += randInt(-10,10);
              if(originpointy > width)
              {
                     originpointy = width;
              }
              if(originpointy < 0)
              {
                     originpointy = 0;
              }
       }


       char choicy;
       cout << "(m)icro (single tree) or m(a)cro (landscape) testing?" << endl;
       cin >> choicy;

       if (choicy == 'm')
       {
              while(newForest.trees.at(0).isAlive==1)
              {
                generateTree(newForest, DimInfo, ResourceVector, 100);
              }

              //calculates XYZ coordinates for each branch
              //newTree = CalcXYZ(newTree);

              //renders the tree in a GLUT window
              //render(argc, argv, newTree);

              ///// End 3-d render code /////
       }
       else //test multiple trees
       {
              /* int feed;
              cout << "Feed:" <<endl;
              cin >> feed;
              int decay;
              cout << "Decay:" <<endl;
              cin >> decay; */
              while (newForest.trees.size() > 0) // number of alive trees
              {
            	  int turn = 0;
            	      int turnstogo = 0;
            	  	int target = 0;
            	  	cout << "Analyse a tree? (negative # to not analyse anything)" << endl;
            	  	cin >> target;
            	  	if (target>0 && target<=newForest.trees.size())
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
            	  	   int interaction=0;
            	  	   cout << "What would you like to do to this tree?" << endl
            	  	   << "0: Leaf it alone" << endl
            	  	   << "1: Kill it with fire" << endl
            	  	   << "2: Kill it with herbicide" << endl
            	  	   << "3: Fertilize and water it" << endl
            	  	   << "4: Clone it" << endl
            	  	   << "5: Change the seed" << endl
            	  	   << "99: Give it sentience" << endl;
            	  	   cin >> interaction;
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
            	  			tree newTree=spawnTree(clonex,cloney,clonez,newForest.trees.at(target-1).treeSeed, DimInfo, ResourceVector);
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

            	  	 cout << "Turn: " << turn << endl;
            	  	 cout << "Number of turns to continue?" << endl;
            	  	 cin >> turnstogo;



            	  	 if(turnstogo == -1)
            	  	 {
            	  		   newForest.trees.clear();
            	  	 }
            	  	 while(turnstogo > 0)
            	  	 {
            	  		newForest=generateForest(newForest,DimInfo, ResourceVector, turn, turnstogo, target);


            	  		cout << "(S)top Generation and Render?" << endl;
						char abba;
						cin >> abba;
						if (abba == 's' || abba == 'S')
						{
							//calculates XYZ coordinates for each branch
							for (int i = 0; i < newForest.trees.size();)
							{
								newForest.trees.at(i) = CalcXYZ(newForest.trees.at(i));

								i++;
							}


							passForest(newForest);

							//renders the tree in a window
							int treeupdate = 0;

							Engine::Settings engine;
							Engine::Camera_settings camera;

							Engine::setup(&engine, &camera);
								Engine::setDrawFunc(&renderScene);
								engine.run = true;
								while (engine.run) {

									if (treeupdate > 1000) //Tree generation and updating
									{
										newForest=generateForest(newForest,DimInfo, ResourceVector, turn, turnstogo, target);
										for (int i = 0; i < newForest.trees.size();)
										{
											newForest.trees.at(i) = CalcXYZ(newForest.trees.at(i));

											i++;
										}

										passForest(newForest);
										treeupdate = 0;
									}

									Engine::updateMouseMode();
									Engine::update();
									Engine::display();

									treeupdate++;
								}

							///// End 3-d render code /////
						}

            	  	 }
			  }
       }
       cin >> uselessThing;



}
