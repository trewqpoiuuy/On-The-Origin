/*
 * main.cpp
 *
 *  Created on: Dec 14, 2013
 *      Author: theron
 */

// Testing, 2014-10-17 10:12

const double PI=3.14159265358979323846;

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <SDL_ttf.h>
//#include <GL/gl.h>
#include <GL/glu.h>
//#include "GL/glew.h"
//#include "GL/glut.h"
//////////                    Internal includes:
#include "entity.h"
#include "voxelobject.h"
#include "Plant.h"

#include "primitives.h"


int argc;
char** argv;

//#include "entity.h"

int w_width;
int w_height;
//float w_aspect;

bool fullscreen=false;
bool render360mode=false;  // Whether to render in panorama mode (experimental, also redundant to GL's cubemap functionality)
bool riftmode=false;
float eyespread=1;
int r360_p_width=460;      // View texture resolution
int r360_p_spacing=20;

bool run;
bool capturemouse=true;
bool showterrain=true;
int maxfps=120;

float rot1 = 0;
float rot2 = 0;

float camera_x=0;
float camera_y=0;
float camera_z=-10;
float camera_yaw=0;
float camera_pitch=0;
float camera_fb_v=0;
float camera_fb_speed=2;
float camera_lr_v=0;
float camera_lr_speed=2;
float camera_ud_v=0;
float camera_ud_speed=2;
#ifdef _WIN32
float sensitivity=200;
#else
float sensitivity=1000;
#endif

float camera_x_last=camera_x;
float camera_y_last=camera_y;
float camera_z_last=camera_z;

struct vec3 {
	float x;
	float y;
	float z;
};

VoxelObject voxel_test(256);
Plant plant_test;
std::vector<vec3> seeds;

SDL_Window *sdlWindow;
SDL_Renderer *sdlRenderer;
SDL_GLContext glContext;

void setupGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0, 0, 0, 0);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void setupSDL() {
	SDL_Init( SDL_INIT_EVERYTHING );
	int mode = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
	if (fullscreen) {
		mode = (mode | SDL_WINDOW_FULLSCREEN_DESKTOP); // TODO: Make this toggleable during program run
	}
	sdlWindow = SDL_CreateWindow("On The Origin - Game Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w_width, w_height, mode);
	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	glContext = SDL_GL_CreateContext(sdlWindow);
}

void setupGraphics() {
	setupSDL();
	setupGL();
}

void updateMouseMode() {
    if (capturemouse) { SDL_SetRelativeMouseMode(SDL_TRUE); }
    else { SDL_SetRelativeMouseMode(SDL_FALSE); }
}

SDL_Event event;
void update() {
    while (SDL_PollEvent(&event)) // Loop until all events are handled
    {
        switch (event.type) {
        case (SDL_QUIT):
        	run=false;
        	break;
        case (SDL_KEYDOWN):
			//Uint8 *keystate = SDL_GetKeyState(NULL);    // SDL 1.2
			//keystate = SDL_GetKeyboardState(NULL);
			switch (event.key.keysym.sym) { // Key SCANcode
			case (SDLK_ESCAPE):
		        capturemouse=!capturemouse;
				updateMouseMode();
		        break;
			/*case (SDL_SCANCODE_P):
				plant_test.simulate();
				printf("simulating plant_test\n");
				break;*/
			}
        	break;
        case (SDL_MOUSEMOTION):
        	if (capturemouse) {
        		camera_yaw+=(float)(event.motion.xrel)/sensitivity;
        		camera_pitch+=(float)(-event.motion.yrel)/sensitivity;
        	}
        	break;
        case (SDL_WINDOWEVENT):
        	switch (event.window.event) {
        	case (SDL_WINDOWEVENT_RESIZED):
				SDL_GetWindowSize(sdlWindow, &w_width, &w_height);
				break;
        	}
        	break;
    	}
    }



    //Uint8 *keystate = SDL_GetKeyState(NULL);    // SDL 1.2
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

	camera_fb_v*=0.1;
	camera_lr_v*=0.1;

	if (keystate[SDL_SCANCODE_W]) {
		camera_fb_v+=camera_fb_speed;
	}
	if (keystate[SDL_SCANCODE_S]) {
		camera_fb_v-=camera_fb_speed;
	}
	if (keystate[SDL_SCANCODE_A]) {
		camera_lr_v+=camera_lr_speed;
	}
	if (keystate[SDL_SCANCODE_D]) {
		camera_lr_v-=camera_lr_speed;
	}
	if (keystate[SDL_SCANCODE_SPACE]) {
		camera_y+=camera_ud_speed;
	}
	if (keystate[SDL_SCANCODE_LSHIFT]) {
		camera_y-=camera_ud_speed;
	}
	if (keystate[SDL_SCANCODE_R]) {
		//voxel_test.updateMesh();
	}
	if (keystate[SDL_SCANCODE_I]) {
		camera_pitch+=.05;
	}
	if (keystate[SDL_SCANCODE_K]) {
		camera_pitch-=.05;
	}
	if (keystate[SDL_SCANCODE_L]) {
		camera_yaw+=.05;
	}
	if (keystate[SDL_SCANCODE_J]) {
		camera_yaw-=.05;
	}
	if (keystate[SDL_SCANCODE_P]) {
		//plant_test.simulate();
		printf("simulating plant_test\n");
	}
	if (keystate[SDL_SCANCODE_T]) {
		eyespread-=0.05;
	}
	if (keystate[SDL_SCANCODE_Y]) {
		eyespread+=0.05;
	}
	for (int n=0; n<9; n++) {
		if (keystate[SDL_SCANCODE_1+n]) {
			float speed=pow(2,n-2);
			camera_fb_speed=speed;
			camera_lr_speed=speed;
			camera_ud_speed=speed;
		}
	}

	camera_x+= -sin(camera_yaw)*camera_fb_v + cos(camera_yaw)*camera_lr_v;
	camera_z+= cos(camera_yaw)*camera_fb_v + sin(camera_yaw)*camera_lr_v;

	//camera_x+=camera_fb_v;
	//camera_z+=camera_rl_v;
	camera_yaw=fmod(camera_yaw,360);
	if (camera_pitch>=PI/2) {
		camera_pitch=PI/2-0.0001;
	}
	if (camera_pitch<=-PI/2) {
		camera_pitch=-PI/2+0.0001;
	}
	//rot1=fmod(rot1,360.0);
	//rot2=fmod(rot2,360.0);
	//printf("%d\n",counter);
	//counter+=1;

	/*if (camera_x!=camera_x_last || camera_y!=camera_y_last || camera_z!=camera_z_last) {
		voxelExtractVerts(voxel, vertexbufferdata, normalbufferdata, &nbufferdata);

		glBufferData(GL_ARRAY_BUFFER, nbufferdata * 3 * sizeof(float), vertexbufferdata, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, nbufferdata * 3 * sizeof(float), normalbufferdata, GL_STATIC_DRAW);
	}*/

	camera_x_last=camera_x;
	camera_y_last=camera_y;
	camera_z_last=camera_z;
}

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
	for (int i=0; i<seeds.size(); i++) {
		vec3 seed = seeds[i];
		drawSphere(seed.x,seed.y,seed.z,0.1,3,5);
	}
	/*for (int i=0; i<1000*2; i++) {
		//float x=((float)(rand()%10000))/10000*256-128;
		//float y=((float)(rand()%10000))/10000*256-128;
		//float z=((float)(rand()%10000))/10000*256-128;
		int x=rand()%256-128;
		int y=rand()%256-128;
		int z=rand()%256-128;
		drawSphere(x,y,z,1,3,5);
	}*/
	//ent_test.draw();
	//drawSphere(0,0,0,100,3,5);
	//plant_test.draw();
	if (showterrain) {
		glPushMatrix();
		glTranslatef(-128,-128,-128);
		voxel_test.draw();
		glPopMatrix();
	}
    /*glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
    //glNormalPointer(GL_FLOAT, 0, &normals[0]);
    glColorPointer(3, GL_FLOAT, 0, &colors[0]);
    //glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
    glDrawArrays(GL_QUADS, 0, verts.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);*/
}

void display() {
	// An actual example of proper working VBOs http://stackoverflow.com/a/7947134
	if (render360mode) { // Set up and render 6 different views plus normal view
		int w=r360_p_width;
		int s=r360_p_width+r360_p_spacing;
		float glnear=1;
		float glfar=512;
		glViewport(0, 0, w_width, w_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Up
		glViewport(s*0, s*1, w, w);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90,1,glnear,glfar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_x, camera_y, camera_z,
						 0+camera_x, 1+camera_y, 0+camera_z,
						 1., 0., 0.);
		renderscene();
		// Down
		glViewport(s*1, s*1, w, w);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90,1,glnear,glfar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_x, camera_y, camera_z,
						 0+camera_x, -1+camera_y, 0+camera_z,
						 1., 0., 0.);
		renderscene();
		// Left (+x)
		glViewport(s*0, s*0, w, w);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90,1,glnear,glfar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_x, camera_y, camera_z,
						 1+camera_x, 0+camera_y, 0+camera_z,
						 0., 1., 0.);
		renderscene();
		// Forward (+z)
		glViewport(s*1, s*0, w, w);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90,1,glnear,glfar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_x, camera_y, camera_z,
						 0+camera_x, 0+camera_y, 1+camera_z,
						 0., 1., 0.);
		renderscene();
		// Right (-x)
		glViewport(s*2, s*0, w, w);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90,1,glnear,glfar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_x, camera_y, camera_z,
						 -1+camera_x, 0+camera_y, 0+camera_z,
						 0., 1., 0.);
		renderscene();
		// Behind (-z)
		glViewport(s*3, s*0, w, w);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(90,1,glnear,glfar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_x, camera_y, camera_z,
						 0+camera_x, 0+camera_y, -1+camera_z,
						 0., 1., 0.);
		renderscene();

		// Normal camera view
		glViewport(s*2, s*1, w*2, w);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,2,glnear,glfar);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camera_x, camera_y, camera_z,
						 -sin(camera_yaw)*cos(camera_pitch)+camera_x, sin(camera_pitch)+camera_y, cos(camera_yaw)*cos(camera_pitch)+camera_z,
						 0., 1., 0.);
		renderscene();

		SDL_GL_SwapWindow(sdlWindow);
	}
	else if (riftmode) { // Split-eye.  Not rift compatible!
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, w_width/2, w_height); // Left
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(float)w_width/(float)w_height,1.,512.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 0,
				0,0,1,
				0,1,0);
		glTranslatef(-eyespread,0,0);
		glRotatef(camera_pitch*180/PI,1,0,0);
		glRotatef(camera_yaw*180/PI,0,1,0);
		glTranslatef(-camera_x, -camera_y, -camera_z);
		renderscene();

		glViewport(w_width/2, 0, w_width/2, w_height); // Right
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(float)w_width/(float)w_height,1.,512.);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 0,
				0,0,1,
				0,1,0);
		glTranslatef(eyespread,0,0);
		glRotatef(camera_pitch*180/PI,1,0,0);
		glRotatef(camera_yaw*180/PI,0,1,0);
		glTranslatef(-camera_x, -camera_y, -camera_z);
		renderscene();
		SDL_GL_SwapWindow(sdlWindow);

	}
	else { // Normal fullscreen viewport
		glViewport(0, 0, w_width, w_height);
		glClearColor(.0, .0, .0, .0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60,(float)w_width/(float)w_height,1.,512.);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		/*gluLookAt(0., 0., 0.,
			 -sin(camera_yaw)*cos(camera_pitch), sin(camera_pitch), cos(camera_yaw)*cos(camera_pitch),
			 0., 1., 0.);*/
		//glTranslatef(-camera_x, -camera_y, -camera_z);
		gluLookAt(camera_x, camera_y, camera_z,
				 -sin(camera_yaw)*cos(camera_pitch)+camera_x, sin(camera_pitch)+camera_y, cos(camera_yaw)*cos(camera_pitch)+camera_z,
				 0., 1., 0.);
		// TODO: Consider changing the above gluLookAt to be similar to that used for riftmode -
		//   Will make using GL to render a HUD or GUI much easier!  (Eye-space coordinates before transforms)

		renderscene(); // Draw OpenGL polygons

		//SDL_GL_SwapBuffers();    // SDL 1.2
		SDL_GL_SwapWindow(sdlWindow);
		// We seriously need a framerate regulator of some sort.  Render loop seems to run at unlimited speed (no VSync)
	}
}

int main(int argc, char *argv[]) {

	if (render360mode) {
		w_width=r360_p_width*4+r360_p_spacing*3;
		w_height=r360_p_width*2+r360_p_spacing;
	}
	else {
		//int w_width=1920;
		//int w_height=1080;
		w_width=800;    // Default (starting) window size
		w_height=600;
		//w_aspect=(float)w_width/(float)w_height;
	}

	if (showterrain) {
		/*voxel_test.fill(1);

		float cave_c_x=128;
		//float cave_c_y=256-120;
		float cave_c_y=512/2;
		float cave_c_z=128;
		float cave_r=512/2-50;

		//voxel_test.voxelSphere(cave_c_x,cave_c_y,cave_c_z,cave_r+30,1);
		voxel_test.voxelSphere(cave_c_x,cave_c_y,cave_c_z,cave_r-0,0);
		for (int i=0; i<1000*2; i++) {
			float a=((float)(rand()%10000))*PI*2/10000;
			float b=((float)(rand()%10000))*PI*2/10000-PI;
			voxel_test.voxelSphere(cave_c_x+cave_r*cos(a)*cos(b),cave_c_y+cave_r*sin(b),cave_c_z+cave_r*sin(a)*cos(b),rand()%20,0);
			printf("%f,%f\n",a,b);
		}*/
		voxel_test.set(127,127,127,1);
	}

	/*voxel_test.voxelSphere(128., 128., 128., 36., 1);
    for (int z=-2; z<=2; z++) {
    	for (int y=-2; y<=2; y++) {
    		for (int x=-2; x<=2; x++) {
    			voxel_test.voxelSphere( 128+28*x, 128+28*y, 128+28*z, 16., 0);
    			printf("Added sphere %d %d %d\n",x,y,z);
    		}
    	}
    }*/

	/*//voxel_test.fill(1);
	//int z=127;
	for (int z=4; z<250; z++) {
		//int y=127;
		for (int y=4; y<250; y++) {
			//int x=127;
			for (int x=4; x<250; x++) {
				voxel_test.set(x,y,z,1);
				for (int i=1; i<4; i++) {
					int p=pow(3,i+1);
					int a=1*pow(3,i);
					int b=2*pow(3,i);
					if ( ( x%p>a && x%p<b ) || ( y%p>a && y%p<b ) || ( z%p>a && z%p<b )) {
						voxel_test.set(x,y,z,0);
					}
					//printf("p=%d a=%d b=%d\n",p,a,b);
				}
			}
		}
		printf("processing z layer %d\n",z);
	}*/


	if (showterrain) {
		voxel_test.updateMesh();
		for (int i=0; i<1000; i++) {
			float x=((float)(rand()%10000))*256;
			float y=((float)(rand()%10000))*256;
			float z=((float)(rand()%10000))*256;
			//voxel_test.dropSeed(&x, &y, &z);
			/*vec3 seed;
			seed.x=x;
			seed.y=y;
			seed.z=z;*/
			//seeds.push_back(seed);
		}
	}

	setupGraphics();

	/*Plant::Resources plant_test_resources;
	Plant::Data plant_test_data;
	plant_test_resources.biomass = 1000;
	plant_test_data.size = 5;
	plant_test_data.nbranches = 2;
	plant_test_data.bsratio = 1.1;

	plant_test.addnode(plant_test_resources, plant_test_data, 0, -50, 0, NULL);*/

	//plant_test.addnode(plant_test.root,0,0,0);

	run=true;
	updateMouseMode();
	while (run) {
		//run=false;
		update();
		//SDL_PollEvent(&event);
		display();
	}
	//render();
	printf("Exiting!\n");
	return 0;
}
