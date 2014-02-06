//#include "stdafx.h"
 
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "VectorIntLib.h" //delete 'Resources/' if broken
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
       vector<VectorStruct> ResourceVector=initializeResources(DimInfo);
       forest newForest; //all trees
 
       int startingtrees;
	   cout << "How many trees to start?" << endl;
	   cin >> startingtrees;
       int originpointx = 45;
       int originpointy = 45;
       while(startingtrees > 0)
       {
              cout << "Spawning origin tree. " << endl;
              seed treeSeed=generateSeed();
              tree newTree=spawnTree(originpointx,originpointy,0,treeSeed, DimInfo, ResourceVector);
              newForest.trees.push_back(newTree);
 
              startingtrees -= 1;
              originpointx += randInt(-5,5);
              if(originpointx > length)
              {
                     originpointx = length;
              }
			  if(originpointx < 0)
              {
                     originpointx = 0;
              }
              originpointy += randInt(-5,5);
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
                     int sunlightAdded;
                     int waterAdded;
                     int potassiumAdded;
                     int phosphorusAdded;
                     int nitrogenAdded;
 
                           cout << "sunlight this turn?" << endl;
                           cin >> sunlightAdded;
                            newForest.trees.at(0).sunlight=newForest.trees.at(0).sunlight+sunlightAdded;
                           cout << "water this turn?" << endl;
                           cin >> waterAdded;
                           newForest.trees.at(0).water=newForest.trees.at(0).water-ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "water", waterAdded);
                           cout << "potassium this turn?" << endl;                       //Prompts are temporary, will be replaced by game engine commands
                           cin >> potassiumAdded;
                           newForest.trees.at(0).potassium=newForest.trees.at(0).potassium-ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "potassium", potassiumAdded);
                           cout << "phosphorus this turn?" << endl;
                           cin >> phosphorusAdded;
                           newForest.trees.at(0).phosphorus=newForest.trees.at(0).phosphorus-ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "phosphorus", phosphorusAdded);
                           cout << "nitrogen this turn?" << endl;
                           cin >> nitrogenAdded;
                           newForest.trees.at(0).nitrogen=newForest.trees.at(0).nitrogen-ResourceChange(newForest.trees.at(0).x, newForest.trees.at(0).y, newForest.trees.at(0).z, DimInfo, ResourceVector, "nitrogen", nitrogenAdded);
                           //while(newForest.trees.at(0).sunlight>=10 && newForest.trees.at(0).water>=20 && newForest.trees.at(0).nitrogen>=15 && newForest.trees.at(0).potassium>=30 && newTree.phosphorus>=25)
                           while(newForest.trees.at(0).sunlight>=10 && newForest.trees.at(0).water>=20 && newForest.trees.at(0).nitrogen>=15 && newForest.trees.at(0).potassium>=30 && newForest.trees.at(0).phosphorus>=25)
                           {
                                  newForest.trees.at(0)=growBranch(newForest.trees.at(0), ResourceVector, DimInfo);
                           }
                           newForest.trees.at(0)=upkeep(newForest.trees.at(0), ResourceVector, DimInfo);
 
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
 
                           ///// Begin 3-d render code /////
                           //exits generation
                           cout << "Kill? 0 for yes, 1 for no" << endl;
                           cin >> newForest.trees.at(0).isAlive;
              }
 
              //calculates XYZ coordinates for each branch
              //newTree = CalcXYZ(newTree);
 
              //renders the tree in a GLUT window
              //render(argc, argv, newTree);
 
              ///// End 3-d render code /////
       }
       else //test multiple trees
       {
              int feed;
              cout << "Feed:" <<endl;
              cin >> feed;
              int decay;
              cout << "Decay:" <<endl;
              cin >> decay;
              while (newForest.trees.size() > 0) // number of alive trees
              {
				generateForest(newForest,DimInfo, ResourceVector, feed,decay);
			  }
       }
       cin >> uselessThing;
 
 
 
}