/*
 * Engine.h
 *
 *  Created on: Jan 17, 2014
 *      Author: Theron
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>

// Rewriting as a library (namespace Engine)
// Moving code over from main.cpp
namespace Engine {    // This is the proper way to set and use namespaces for libraries.  It's like this in the libstdc++ (std::) itself.
	struct Settings {
		int w_width=800;
		int w_height=600;

		bool fullscreen=false;
		bool render360mode=false;  // Whether to render in panorama mode (experimental, also redundant to GL's cubemap functionality)
		bool riftmode=false;
		int r360_p_width=460;      // Cube mode texture resolution
		int r360_p_spacing=20;

		bool run;
		bool capturemouse=true;
		bool showterrain=true;
		int maxfps=120;          // Unused!  TODO: Needs to be implemented

		const char* windowname = "OnTheOrigin";
	};

	struct Camera_settings {
		float x=0;
		float y=0;
		float z=-10;
		float yaw=0;
		float pitch=0;
		float fb_v=0;
		float fb_speed=2;
		float lr_v=0;
		float lr_speed=2;
		float ud_v=0;
		float ud_speed=2;   // Defaults, defaults, defaults ...
		float eyespread=1;  // For riftmode
		#ifdef _WIN32
		float sensitivity=200;  // Sensitivity needs to be set to less (more sensitive) on Windows...
		#else
		float sensitivity=1000; // And higher on Linux (Mint) ???
		#endif
	};

	bool setup(Settings* enginearg, Camera_settings* cameraarg);
	void updateMouseMode();
	void setDrawFunc(void (*drawFunc)());
	void display();
	void update();
}

#endif /* ENGINE_H_ */