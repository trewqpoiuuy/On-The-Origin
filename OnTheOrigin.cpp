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

bool lighting=false;

void toggleLighting() {
	lighting = !lighting;
	if (lighting) {
		printf("Toggleing lighting ON\n");
	}
	else {
		printf("Toggleing lighting OFF\n");
	}
}

void renderScene() {
	if (lighting) {
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
	}
	float light0pos[4] = {0,70-128,0,1};
	float light0color[4] = {.6,.45,.4,0};
	float light1pos[4] = {5,20,7,0};
	float light1color[4] = {0.2,0.2,0.5};

	/*for (int i=0; i<3; i++) {
		light0color[i]*=.5;
		light1color[i]*=.5;
	}*/
	//float light1spec[4] = {1,1,1,0};

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0color);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light1spec);

	// Draw ground
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, 0.0f, 10.0f);
	glVertex3f(-10.0f, 0.0f, -10.0f);
	glVertex3f(10.0f, 0.0f, -10.0f);
	glVertex3f(10.0f, 0.0f, 10.0f);
	glEnd();

	VertexArrayUtils::drawData(&drawdata);
}

int main(int argc, char **argv) {
	Engine::Settings engine;
	Engine::Camera_settings camera;

	Engine::setup(&engine, &camera);
	Engine::setDrawFunc(&renderScene);
	Engine::addKeyDownBinding(SDLK_F6,&toggleLighting);
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
