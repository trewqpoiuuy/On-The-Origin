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
              //cout << newTree.isAlive;
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
    	   	  cout << "Hello" << endl;
              while (newForest.trees.size() > 0) // number of alive trees
              {
				newForest=generateForest(newForest,DimInfo, ResourceVector);
			  }
			  cout << "ITS DEAD!";

       }
       cin >> uselessThing;



}
