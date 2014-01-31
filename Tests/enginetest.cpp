/*
 * main.cpp
 *
 *  Created on: Jan 18, 2014
 *      Author: Theron
 */
// Example usage of Engine.h

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include "../Engine/Engine.h"
#include "../Engine/primitives.h"
#include "../Engine/VoxelObject.h"
#include "../Engine/Cell.h"
#include "../Engine/CellObject.h"
#include "../Engine/utils.h"

const double PI=3.14159265358979323846;

Engine::Settings engine;
Engine::Camera_settings camera;

bool showterrain=true;

VoxelObject voxel_test(256);
Cell::CellObject cell_test;
void renderscene() {
    //                                          Test Lights
	//float light0pos[4] = {25,50,0,0};
	//float light0color[4] = {1,.2,.6,0};
	//float light1pos[4] = {-25,50,0,0};
	//float light1color[4] = {.2,1,.6,0};
	//float light1pos[4] = {1,5,2,0};
	//float light1color[4] = {.5,.5,.7,0};
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

	drawSphere(0,0,0,1,20,20);
	if (showterrain) {
		glPushMatrix();
		glTranslatef(-128,-128,-128);
		voxel_test.draw();
		glPopMatrix();
	}
	cell_test.draw();
}

void carvevoxels1() {
	voxel_test.fill(1);

	float cave_c_x=128;
	//float cave_c_y=256-120;
	float cave_c_y=512/2;
	float cave_c_z=128;
	float cave_r=512/2-50;

	//voxel_test.voxelSphere(cave_c_x,cave_c_y,cave_c_z,cave_r+30,1);
	//voxel_test.voxelSphere(cave_c_x,cave_c_y,cave_c_z,cave_r-0,0);
	voxel_test.voxelSphere(cave_c_x,cave_c_y,cave_c_z,cave_r,0);
	for (int i=0; i<1000*4; i++) {
		float a=((float)(rand()%10000))*PI*2/10000;
		float b=((float)(rand()%10000))*PI*2/10000-PI;
		voxel_test.voxelSphere(cave_c_x+cave_r*cos(a)*cos(b),cave_c_y+cave_r*sin(b),cave_c_z+cave_r*sin(a)*cos(b),rand()%20,0);
		printf("%f,%f\n",a,b);
	}
}

void carvevoxels2() {
	voxel_test.voxelSphere(128., 128., 128., 36., 1);
	for (int z=-2; z<=2; z++) {
		for (int y=-2; y<=2; y++) {
			for (int x=-2; x<=2; x++) {
				voxel_test.voxelSphere( 128+28*x, 128+28*y, 128+28*z, 16., 0);
				printf("Added sphere %d %d %d\n",x,y,z);
			}
		}
	}
}

void addrandomcells() {
	for (int i=0; i<500; i++) {
		float range=.512;
		float x=(pow(randf()*2-1,9)*range);
		float y=(pow(randf()*2-1,9)*range);
		float z=(pow(randf()*2-1,9)*range);
		cell_test.addcell(Cell::Cell{&cell_test.cells,x,y,z,1});
	}
}

int main(int argc, char *argv[]) {

	carvevoxels1();
	//carvevoxels2();
	voxel_test.updateMesh();

	addrandomcells();

	//engine.fullscreen = true;
	//engine.riftmode = true;
	Engine::setup(&engine, &camera);
	Engine::setDrawFunc(&renderscene);
	engine.run = true;
	while (engine.run) {
		for (int i=0; i<cell_test.cells.size(); i++) {
			cell_test.cells[i].physics();
		}
		Engine::updateMouseMode();
		Engine::update();
		Engine::display();
	}
	return 0;
}
