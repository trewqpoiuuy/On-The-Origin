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
#include "../TreeRender/TreeRender.h"

const double PI=3.14159265358979323846;

Engine::Settings engine;
Engine::Camera_settings camera;


int main(int argc, char *argv[]) {

	Engine::setup(&engine, &camera);
	Engine::setDrawFunc(&drawTree);
	engine.run = true;
	while (engine.run) {
		Engine::updateMouseMode();
		Engine::update();
		Engine::display();
	}
	return 0;
}
