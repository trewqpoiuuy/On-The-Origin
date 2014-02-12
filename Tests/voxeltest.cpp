/*
 * voxeltest.cpp
 *
 *  Created on: Feb 11, 2014
 *      Author: Theron
 */




#include "../World/World.h"
#include <stdio.h>

World::World testworld;
int main() {
	testworld.editDataPAt(0,0,0)->solid = true;
	testworld.update();
	printf("Hello world!\n");
}
