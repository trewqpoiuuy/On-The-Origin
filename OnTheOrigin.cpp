/*
 * OnTheOrigin.cpp
 *
 *  Created on: Mar 1, 2014
 *      Author: Theron
 */

// This will now be our main source for running the actual program.  All other main()s will remain in Tests, as tests and only tests.


#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Resources/VectorIntLib.h"
#include "TreeGen/TreeGen.h"
#include "TreeRender/TreeRender.h"
#include "Engine/Engine.h"
#include "Engine/VertexArrayUtils.h"

VertexArrayUtils::Data drawdata;
DimensionStruct DimInfo;
vector<VectorStruct> ResourceVector;
TreeGen::forest forest; //all trees

void createForest() {
    srand(time(NULL));
    rand(); //First value of rand is the seed, so this gets rid of it
    int depth = 45;
    int length = 1000;
    int width = 1000;
    int TopsoilDepth = 1;

    DimInfo.length = length;
    DimInfo.width = width;
    DimInfo.depth = depth;
    DimInfo.TopsoilDepth = TopsoilDepth;
    ResourceVector=initializeResources(DimInfo);
    cout << "Water Test: " << WaterGrab(0,0,0,DimInfo, ResourceVector) << endl;


    int startingtrees;
	   cout << "How many trees to start?" << endl;
	   cin >> startingtrees;
    int originpointx = 500;
    int originpointy = 500;
    while(startingtrees > 0)
    {
           cout << "Spawning origin tree. " << endl;
           TreeGen::seed treeSeed=TreeGen::generateSeed();
			  char PlantIDArray[10];
			  sprintf( PlantIDArray, "%d",forest.trees.size());
			  string newPlantID=PlantIDArray;
			  int treex=originpointx + TreeGen::randInt(-499,499);
           if(treex > length-1)
           {
                  treex = length-1;
           }
			  if(treex < 0)
           {
                  treex = 0;
           }
           int treey = originpointy+  TreeGen::randInt(-499,499);
           if(treey > width-1)
           {
                  treey = width-1;
           }
           if(treey < 0)
           {
                  treey = 0;
           }
           TreeGen::tree newTree=spawnTree(treex,treey,depth,treeSeed, DimInfo, ResourceVector, newPlantID);
           forest.trees.push_back(newTree);

           startingtrees -= 1;

    }
}

void renderScene() {
	glDisable(GL_LIGHTING);
	VertexArrayUtils::drawData(&drawdata);
}

int main(int argc, char **argv) {
	Engine::Settings engine;
	Engine::Camera_settings camera;

	Engine::setup(&engine, &camera);
	Engine::setDrawFunc(&renderScene);
	engine.run = true;
#define threaded
#ifdef threaded
	Engine::startUpdateLoop();
#endif
#ifndef threaded
	Engine::setupSDL();
	Engine::setupGL();
#endif
	createForest();
	while (engine.run) {
		forest=TreeGen::generateForest(forest,DimInfo, ResourceVector, 0, 0, 0);
		for (unsigned int i = 0; i < forest.trees.size();)
		{
			forest.trees.at(i) = TreeRender::CalcXYZ(forest.trees.at(i));
			i++;
		}
#ifdef threaded
		SDL_mutexP(Engine::renderLock);
#endif
		TreeRender::drawForest(&forest,&drawdata,0,0,10000.);
		VertexArrayUtils::finishData(&drawdata);
#ifdef threaded
		SDL_mutexV(Engine::renderLock);
#endif
#ifndef threaded
		VertexArrayUtils::drawData(&drawdata);
		Engine::updateMouseMode();
		Engine::update();
		Engine::display();
#endif
	}
#ifdef threaded
	Engine::finishUpdateLoop();
#endif
	return 0;
}
