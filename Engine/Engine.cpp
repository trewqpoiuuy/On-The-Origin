/*
 * Engine.cpp
 *
 *  Created on: Jan 18, 2014
 *      Author: Theron
 */

#include "Engine.h"
#include <vector>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>

const double PI=3.14159265358979323846;

namespace Engine {

	Settings *engine;
	Camera_settings *camera;
	SDL_Window *sdlWindow;
	//SDL_Renderer *sdlRenderer;
	SDL_GLContext glContext;

	std::vector<keyBind> keyDownBindings;
	std::vector<keyBind> keyPressedBindings;

	SDL_Thread *updateLoopThread;
	SDL_Thread *renderLoopThread;

	int framecounter=0;
	int updatelastframe=0;

	void (*engineDrawScene)();

	SDL_mutex *renderLock = NULL;

	float renderMin=1.0;
	float renderMax=2048.0;

	GLuint consoleFontTexture;

	void loadConsoleFontTexture() {
		glEnable( GL_TEXTURE_2D );
		SDL_Surface *Image = SDL_LoadBMP("Resource/font6x8_rgb.bmp");  //TODO NEED RELATIVE PATH
	    if (Image==NULL) {
	        printf("Error: %s\n",SDL_GetError());
	        return;
	    }
		glGenTextures( 1, &consoleFontTexture);
		glBindTexture( GL_TEXTURE_2D, consoleFontTexture);
		//glTexImage2D( GL_TEXTURE_2D, 0, 3, Image->w, Image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, Image->pixels);
		printf("Width x Height: %dx%d\n",Image->w,Image->h);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Image->w,Image->h, 0, GL_RGB,GL_UNSIGNED_BYTE,Image->pixels);
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		SDL_FreeSurface(Image);
		glDisable( GL_TEXTURE_2D );
	}

	void setupSDL() {
		SDL_Init( SDL_INIT_EVERYTHING );
		int mode = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
		if (engine->fullscreen) {
			mode = (mode | SDL_WINDOW_FULLSCREEN_DESKTOP); // TODO: Make this toggleable during program run
		}
		sdlWindow = SDL_CreateWindow(engine->windowname, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, engine->w_width, engine->w_height, mode);
		//sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	}

	void setupGL() {
		glContext = SDL_GL_CreateContext(sdlWindow);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.4f, 0.7f, 1.0f, 0.0f); // Sky color (glClear)
		glFrontFace(GL_CW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		loadConsoleFontTexture();
	}

	int setupSDL_thread(void *p) {
		setupSDL();
		return 0;
	}

	void updateMouseMode() {
		if (engine->capturemouse) { SDL_SetRelativeMouseMode(SDL_TRUE); }
		else { SDL_SetRelativeMouseMode(SDL_FALSE); }
	}

	bool setup(Settings* enginearg, Camera_settings* cameraarg) {
		engine=enginearg;
		camera=cameraarg;
		if (engine->render360mode) {
			engine->w_width=engine->r360_p_width*4+engine->r360_p_spacing*3;
			engine->w_height=engine->r360_p_width*2+engine->r360_p_spacing;
		}


		//setupSDL();
		/*updateLoopThread = SDL_CreateThread(setupSDL_thread, "updatethread", (void *)NULL);
		int r;
		SDL_WaitThread(updateLoopThread, &r);*/

		//setupGL();

		return true; // TODO: Consider adding Error detection, use this return for success
	}

	void setDrawFunc(void (*drawFunc)()) {   // Passing a function as a pointer
		engineDrawScene = drawFunc;
	}

	void addKeyDownBinding(int keyid, void (*funcp)()) {
		keyDownBindings.push_back(keyBind{keyid,funcp});
	}
	void addKeyPressedBinding(int keyid, void (*funcp)()) {
		keyPressedBindings.push_back(keyBind{keyid,funcp});
	}

	void draw2Dgl() {  //TODO text drawing function
		float chrW=10;
		float chrH=16;
		std::string text="\n Hello world";
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, consoleFontTexture );
		glColor3f(0,0,1);
		int cursorX=0;
		int cursorY=0;
		for (unsigned int i=0; i<text.size(); i++) {
			int asciin=text[i];
			int texaddrX = asciin%16;
			int texaddrY = asciin/16;
			glBegin(GL_QUADS);
			glTexCoord2f((texaddrX)/16.,(texaddrY)/16.);  glVertex2f(chrW*(cursorX),chrH*(cursorY));
			glTexCoord2f((texaddrX+1)/16.,(texaddrY)/16.);  glVertex2f(chrW*(cursorX+1),chrH*(cursorY));
			glTexCoord2f((texaddrX+1)/16.,(texaddrY+1)/16.);  glVertex2f(chrW*(cursorX+1),chrH*(cursorY+1));
			glTexCoord2f((texaddrX)/16.,(texaddrY+1)/16.);  glVertex2f(chrW*(cursorX),chrH*(cursorY+1));
			glEnd();
			if (asciin=='\n' /*ASCII for '\n' */) {
				cursorX=0;
				cursorY+=1;
			}
			else {
				cursorX+=1;
			}
		}

		/*		glBegin(GL_QUADS);
		glTexCoord2f(0,0);  glVertex2f(10,10);
		glTexCoord2f(1,0);  glVertex2f(200,10);
		glTexCoord2f(1,1);  glVertex2f(200,200);
		glTexCoord2f(0,1);  glVertex2f(10,200);
		glEnd();*/

		glDisable( GL_TEXTURE_2D );
	}

	void display() {
		// An actual example of proper working VBOs http://stackoverflow.com/a/7947134
		if (engine->render360mode) { // Set up and render 6 different views plus normal view
			int w=engine->r360_p_width;
			int s=engine->r360_p_width+engine->r360_p_spacing;
			float glnear=1;
			float glfar=512;
			glViewport(0, 0, engine->w_width, engine->w_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Up
			glViewport(s*0, s*1, w, w);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90,1,glnear,glfar);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(camera->x, camera->y, camera->z,
							 0+camera->x, 1+camera->y, 0+camera->z,
							 1., 0., 0.);
			engineDrawScene();
			// Down
			glViewport(s*1, s*1, w, w);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90,1,glnear,glfar);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(camera->x, camera->y, camera->z,
							 0+camera->x, -1+camera->y, 0+camera->z,
							 1., 0., 0.);
			engineDrawScene();
			// Left (+x)
			glViewport(s*0, s*0, w, w);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90,1,glnear,glfar);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(camera->x, camera->y, camera->z,
							 1+camera->x, 0+camera->y, 0+camera->z,
							 0., 1., 0.);
			engineDrawScene();
			// Forward (+z)
			glViewport(s*1, s*0, w, w);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90,1,glnear,glfar);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(camera->x, camera->y, camera->z,
							 0+camera->x, 0+camera->y, 1+camera->z,
							 0., 1., 0.);
			engineDrawScene();
			// Right (-x)
			glViewport(s*2, s*0, w, w);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90,1,glnear,glfar);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(camera->x, camera->y, camera->z,
							 -1+camera->x, 0+camera->y, 0+camera->z,
							 0., 1., 0.);
			engineDrawScene();
			// Behind (-z)
			glViewport(s*3, s*0, w, w);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90,1,glnear,glfar);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(camera->x, camera->y, camera->z,
							 0+camera->x, 0+camera->y, -1+camera->z,
							 0., 1., 0.);
			engineDrawScene();

			// Normal camera view
			glViewport(s*2, s*1, w*2, w);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60,2,glnear,glfar);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(camera->x, camera->y, camera->z,
							 -sin(camera->yaw)*cos(camera->pitch)+camera->x, sin(camera->pitch)+camera->y, cos(camera->yaw)*cos(camera->pitch)+camera->z,
							 0., 1., 0.);
			engineDrawScene();

			SDL_GL_SwapWindow(sdlWindow);
		}
		else if (engine->riftmode) { // Split-eye.  Almost rift compatible!
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, engine->w_width/2, engine->w_height); // Left
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glTranslatef(camera->riftoffset,0,0);
			gluPerspective(60,(float)engine->w_width/(float)engine->w_height/2,renderMin,renderMax);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0, 0, 0,
					0,0,1,
					0,1,0);
			glTranslatef(-camera->eyespread,0,0);
			glRotatef(camera->pitch*180/PI,1,0,0);
			glRotatef(camera->yaw*180/PI,0,1,0);
			glTranslatef(-camera->x, -camera->y, -camera->z);
			engineDrawScene();

			glViewport(engine->w_width/2, 0, engine->w_width/2, engine->w_height); // Right
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glTranslatef(-camera->riftoffset,0,0);
			gluPerspective(60,(float)engine->w_width/(float)engine->w_height/2,renderMin,renderMax);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(0, 0, 0,
					0,0,1,
					0,1,0);
			glTranslatef(camera->eyespread,0,0);
			glRotatef(camera->pitch*180/PI,1,0,0);
			glRotatef(camera->yaw*180/PI,0,1,0);
			glTranslatef(-camera->x, -camera->y, -camera->z);
			engineDrawScene();
			SDL_GL_SwapWindow(sdlWindow);

		}
		else { // Normal fullscreen viewport
			glViewport(0, 0, engine->w_width, engine->w_height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(60,(float)engine->w_width/(float)engine->w_height,renderMin,renderMax);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			/*gluLookAt(0., 0., 0.,
				 -sin(camera->yaw)*cos(camera->pitch), sin(camera->pitch), cos(camera->yaw)*cos(camera->pitch),
				 0., 1., 0.);*/
			//glTranslatef(-camera->x, -camera->y, -camera->z);
			gluLookAt(camera->x, camera->y, camera->z,
					 -sin(camera->yaw)*cos(camera->pitch)+camera->x, sin(camera->pitch)+camera->y, cos(camera->yaw)*cos(camera->pitch)+camera->z,
					 0., 1., 0.);
			// TODO: Consider changing the above gluLookAt to be similar to that used for riftmode -
			//   Will make using GL to render a HUD or GUI much easier!  (Eye-space coordinates before transforms)

			engineDrawScene(); // Draw OpenGL polygons

			glClear(GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glOrtho(0,800,600,0,-1,1);
			glDisable(GL_LIGHTING);
			//draw2Dgl();

			//SDL_GL_SwapBuffers();    // SDL 1.2
			SDL_GL_SwapWindow(sdlWindow);
			// We seriously need a framerate regulator of some sort.  Render loop seems to run at unlimited speed (no VSync)
		}
	}

	void update() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) // Loop until all events for this cycle are handled
		{
			switch (event.type) {
			case (SDL_QUIT):
				engine->run=false;
				break;
			case (SDL_KEYDOWN):
				//Uint8 *keystate = SDL_GetKeyState(NULL);    // SDL 1.2
				//keystate = SDL_GetKeyboardState(NULL);
				switch (event.key.keysym.sym) { // Key SCANcode
				case (SDLK_ESCAPE):
					engine->capturemouse=!engine->capturemouse;
					updateMouseMode();
					break;
				/*case (SDL_SCANCODE_P):
					plant_test.simulate();
					printf("simulating plant_test\n");
					break;*/
				case (SDLK_0):
					camera->x=0;
					camera->y=0;
					camera->z=0;
					break;
				case (SDLK_EQUALS || SDLK_PLUS):
					camera->lr_speed*=1.01;
					camera->fb_speed*=1.01;
					camera->ud_speed*=1.01;
					break;
				case (SDLK_MINUS):
					camera->lr_speed/=1.01;
					camera->fb_speed/=1.01;
					camera->ud_speed/=1.01;
					break;
				case (SDLK_r):
					engine->riftmode = !engine->riftmode;
					break;
				case (SDLK_F11):
					engine->fullscreen = !engine->fullscreen;
					//SDL_SetWindowFullscreen(sdlWindow, engine->fullscreen);
					SDL_DestroyWindow(sdlWindow);
					setupSDL();
					setupGL();
					break;
				}
				for (int i=0; i<keyDownBindings.size(); i++) {
					if (event.key.keysym.sym==keyDownBindings[i].keyid) {
						keyDownBindings[i].funcp();
					}
				}
				for (int i=0; i<keyPressedBindings.size(); i++) {
					if (event.key.keysym.sym==keyPressedBindings[i].keyid) {
						keyPressedBindings[i].funcp();
					}
				}
				break;
			case (SDL_MOUSEMOTION):
				if (engine->capturemouse) {
					camera->yaw+=(float)(event.motion.xrel)/camera->sensitivity;
					camera->pitch+=(float)(-event.motion.yrel)/camera->sensitivity;
				}
				break;
			case (SDL_WINDOWEVENT):
				switch (event.window.event) {
				case (SDL_WINDOWEVENT_RESIZED):
					SDL_GetWindowSize(sdlWindow, &engine->w_width, &engine->w_height);
					break;
				}
				break;
			}
		}



		//Uint8 *keystate = SDL_GetKeyState(NULL);    // SDL 1.2
		const Uint8 *keystate = SDL_GetKeyboardState(NULL);

		camera->fb_v*=0.1;
		camera->lr_v*=0.1;

		if (keystate[SDL_SCANCODE_W]) {
			camera->fb_v+=camera->fb_speed;
		}
		if (keystate[SDL_SCANCODE_S]) {
			camera->fb_v-=camera->fb_speed;
		}
		if (keystate[SDL_SCANCODE_A]) {
			camera->lr_v+=camera->lr_speed;
		}
		if (keystate[SDL_SCANCODE_D]) {
			camera->lr_v-=camera->lr_speed;
		}
		if (keystate[SDL_SCANCODE_SPACE]) {
			camera->y+=camera->ud_speed;
		}
		if (keystate[SDL_SCANCODE_LSHIFT]) {
			camera->y-=camera->ud_speed;
		}
		if (keystate[SDL_SCANCODE_R]) {
			//voxel_test.updateMesh();
		}
		if (keystate[SDL_SCANCODE_I]) {
			camera->pitch+=.05;
		}
		if (keystate[SDL_SCANCODE_K]) {
			camera->pitch-=.05;
		}
		if (keystate[SDL_SCANCODE_L]) {
			camera->yaw+=.05;
		}
		if (keystate[SDL_SCANCODE_J]) {
			camera->yaw-=.05;
		}
		if (keystate[SDL_SCANCODE_P]) {
			//plant_test.simulate();
			printf("simulating plant_test\n");
		}
		if (keystate[SDL_SCANCODE_T]) {
			camera->eyespread-=0.05;
		}
		if (keystate[SDL_SCANCODE_Y]) {
			camera->eyespread+=0.05;
		}
		for (int n=0; n<9; n++) {
			if (keystate[SDL_SCANCODE_1+n]) {
				float speed=pow(2,n+camera->speed2powexpoffset);
				camera->fb_speed=speed;
				camera->lr_speed=speed;
				camera->ud_speed=speed;
			}
		}

		camera->x+= -sin(camera->yaw)*camera->fb_v + cos(camera->yaw)*camera->lr_v;
		camera->z+= cos(camera->yaw)*camera->fb_v + sin(camera->yaw)*camera->lr_v;

		//camera->x+=camera->fb_v;
		//camera->z+=camera->rl_v;
		camera->yaw=fmod(camera->yaw,360);
		if (camera->pitch>=PI/2) {
			camera->pitch=PI/2-0.0001;
		}
		if (camera->pitch<=-PI/2) {
			camera->pitch=-PI/2+0.0001;
		}
		//rot1=fmod(rot1,360.0);
		//rot2=fmod(rot2,360.0);
		//printf("%d\n",counter);
		//counter+=1;

		/*if (camera->x!=camera->x_last || camera->y!=camera->y_last || camera->z!=camera->z_last) {
			voxelExtractVerts(voxel, vertexbufferdata, normalbufferdata, &nbufferdata);

			glBufferData(GL_ARRAY_BUFFER, nbufferdata * 3 * sizeof(float), vertexbufferdata, GL_STATIC_DRAW);
			glBufferData(GL_ARRAY_BUFFER, nbufferdata * 3 * sizeof(float), normalbufferdata, GL_STATIC_DRAW);
		}*/
		//printf("Camera at (%f,%f,%f)\n",camera->x,camera->y,camera->z);
	}

	int updateLoop(void *p) {
		setupSDL();
		setupGL();
		renderLock = SDL_CreateMutex();
		while (engine->run) {
			SDL_mutexP(renderLock);
			Engine::updateMouseMode();
			Engine::update();
			Engine::display();
			SDL_mutexV(renderLock);
			framecounter+=1;
			int ups=60; // Updates per second
			SDL_Delay(1000/ups);
			//printf("Update\n");
			//int framessinceupdate = framecounter-updatelastframe;
			//float fps = framessinceupdate*ups;
			//printf("FPS: %f\n",fps);
			updatelastframe = framecounter;
		}
		return 0;
	}
	int renderLoop(void *p) {
		//setupGL();
		//SDL_Init( SDL_INIT_VIDEO );
		while (engine->run) {
			//Engine::display();
			framecounter+=1;
			//printf("Frame %d\n",framecounter);
		}
		return 0;
	}

	void startUpdateLoop() {
		updateLoopThread = SDL_CreateThread(updateLoop, "updatethread", (void *)NULL);
		//updateLoopThread = SDL_CreateThread(updateLoop, NULL);
	    if ( updateLoopThread == NULL ) {
	        fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
	        return;
	    }
	}
	void startRenderLoop() {
		renderLoopThread = SDL_CreateThread(renderLoop, "renderthread", (void *)NULL);
	}
	void finishUpdateLoop() {
		int r;
		SDL_WaitThread(updateLoopThread, &r);
	}
	void finishRenderLoop() {
		int r;
		SDL_WaitThread(renderLoopThread, &r);
	}
}
