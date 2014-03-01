//#include "stdafx.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "../Resources/VectorIntLib.h" //delete 'Resources/' if broken
#include "../TreeGen/Tree generation.h"
#include "../TreeRender/TreeRender.h"
#include "../Engine/Engine.h"

//using namespace std;
//using namespace veclib;

int main(int argc, char **argv)
{
       srand(time(NULL));
       int uselessThing=rand(); //First value of rand is the seed, so this gets rid of it
       int depth = 45;
       int length = 1000;
       int width = 1000;
       int TopsoilDepth = 1;
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
       int originpointx = 500;
       int originpointy = 500;
       while(startingtrees > 0)
       {
              cout << "Spawning origin tree. " << endl;
              seed treeSeed=generateSeed();
			  char PlantIDArray[10];
			  sprintf( PlantIDArray, "%d",newForest.trees.size());
			  string newPlantID=PlantIDArray;
			  int treex=originpointx + randInt(-499,499);
              if(treex > length-1)
              {
                     treex = length-1;
              }
			  if(treex < 0)
              {
                     treex = 0;
              }
              int treey = originpointy+  randInt(-499,499);
              if(treey > width-1)
              {
                     treey = width-1;
              }
              if(treey < 0)
              {
                     treey = 0;
              }
              tree newTree=spawnTree(treex,treey,0,treeSeed, DimInfo, ResourceVector, newPlantID);
              newForest.trees.push_back(newTree);

              startingtrees -= 1;

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
            	  	   userInteraction(interaction, target, newForest, DimInfo, ResourceVector);
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
//#define threaded
#ifdef threaded
							Engine::startUpdateLoop();
#endif
#ifndef threaded
							Engine::setupSDL();
							Engine::setupGL();
#endif
							while (engine.run) {

								if (treeupdate > 1000 || true) //Tree generation and updating
								{
									newForest=generateForest(newForest,DimInfo, ResourceVector, turn, turnstogo, target);
									for (int i = 0; i < newForest.trees.size();)
									{
										newForest.trees.at(i) = CalcXYZ(newForest.trees.at(i));

										i++;
									}

									passForest(newForest);  // Copying all data is potential bottleneck.  Consider using pointers
									drawForest();
#ifdef threaded
									SDL_mutexP(Engine::renderLock);
#endif
									VertexArrayUtils::finishData(&drawdata);
#ifdef threaded
									SDL_mutexV(Engine::renderLock);
#endif
									treeupdate = 0;
								}
#ifndef threaded
								VertexArrayUtils::drawData(&drawdata);
								Engine::updateMouseMode();
								Engine::update();
								Engine::display();
#endif

								treeupdate++;
							}
#ifdef threaded
							Engine::finishUpdateLoop();
#endif
							///// End 3-d render code /////
						}

            	  	 }
			  }
       }
       cin >> uselessThing;



}
