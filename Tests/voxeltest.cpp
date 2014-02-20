/*
 * voxeltest.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: Theron
 */

#include "../World/World.h"
#include "../Engine/Engine.h"
#include "../Engine/primitives.h"
#include <stdio.h>

const double PI=3.14159265358979323846;

Engine::Settings engine;
Engine::Camera_settings camera;

World::World testworld;

int drawdebugmode=0;
bool normalsdebugmode=false;

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

	if (normalsdebugmode) {
		glDisable(GL_LIGHTING);
	}
	glColor3f(1.,0.,0.);
	drawSphere(0,0,0,1,20,20);
	testworld.draw(drawdebugmode);
	//testworld.drawDebugChunkAtCC(0,0,0);
}

void carvevoxels1() {
	//voxel_test.fill(1);

	float cave_c_x=128;
	//float cave_c_y=256-120;
	float cave_c_y=512/2;
	float cave_c_z=128;
	float cave_r=512/2-50;

	testworld.voxelGeomSphere(cave_c_x,cave_c_y,cave_c_z,cave_r+30,1);
	//testworld.voxelGeomSphere(cave_c_x,cave_c_y,cave_c_z,cave_r-0,0);
	testworld.voxelGeomSphere(cave_c_x,cave_c_y,cave_c_z,cave_r,0);
	for (int i=0; i<1000*4; i++) {
		float a=((float)(rand()%10000))*PI*2/10000;
		float b=((float)(rand()%10000))*PI*2/10000-PI;
		testworld.voxelGeomSphere(cave_c_x+cave_r*cos(a)*cos(b),cave_c_y+cave_r*sin(b),cave_c_z+cave_r*sin(a)*cos(b),rand()%20,0);
		printf("%f,%f\n",a,b);
	}
}

void carvevoxels2() {
	testworld.voxelGeomSphere(128., 128., 128., 36., 1);
	for (int z=-2; z<=2; z++) {
		for (int y=-2; y<=2; y++) {
			for (int x=-2; x<=2; x++) {
				testworld.voxelGeomSphere( 128+28*x, 128+28*y, 128+28*z, 16., 0);
				printf("Added sphere %d %d %d\n",x,y,z);
			}
		}
	}
}

/*void addrandomcells() {
	for (int i=0; i<500; i++) {
		float range=.512;
		float x=(pow(randf()*2-1,9)*range);
		float y=(pow(randf()*2-1,9)*range);
		float z=(pow(randf()*2-1,9)*range);
		cell_test.addcell(Cell::Cell{&cell_test.cells,x,y,z,1});
	}
}*/

void setDebugMode0() {
	drawdebugmode=0;
}
void setDebugMode1() {
	drawdebugmode=1;
}

void toggleNormalsDebug() {
	normalsdebugmode = !normalsdebugmode;
}

int main(int argc, char *argv[]) {
	/*for (int i=-8; i<8; i++) {
		testworld.editDataPAt(0,i,0)->solid = true;
	}
	testworld.editDataPAt(0,2,0)->solid = true;
	testworld.editDataPAt(2,2,0)->solid = true;
	testworld.editDataPAt(2,0,0)->solid = true;
	testworld.editDataPAt(0,4,0)->solid = true;*/
	//carvevoxels2();
	testworld.voxelGeomSphere(0,0,0,30,true);
	printf("Hello world!\n");
	Engine::setup(&engine, &camera);
	Engine::setDrawFunc(&renderscene);
	Engine::addKeyDownBinding(SDLK_y,&setDebugMode0);
	Engine::addKeyDownBinding(SDLK_u,&setDebugMode1);

	engine.run = true;
	while (engine.run) {
		/*for (int i=0; i<cell_test.cells.size(); i++) {
			cell_test.cells[i].physics();
		}*/
		testworld.update();
		Engine::updateMouseMode();
		Engine::update();
		Engine::display();
	}
	return 0;
}
