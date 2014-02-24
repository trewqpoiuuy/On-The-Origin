/*
 * utils.cpp
 *
 *  Created on: Jan 23, 2014
 *      Author: Theron
 */

#include "utils.h"
#include <math.h>

float randf() {
	return (float)rand()/RAND_MAX;
}

float pythag(float x, float y, float z) {
	return sqrt(pow(x,2)+pow(y,2)+pow(z,2));
}

void normalize3p(float* x, float* y, float* z) {
	float m = pythag(*x,*y,*z);
	if (m!=0) {
		*x/=m;
		*y/=m;
		*z/=m;
	}
}

int sc(int x, int y, int z, int size) {
	return x+y*size+z*size*size;
}

int sc3w(int x, int y, int z, int sizex, int sizey, int sizez) {
	return x+y*sizex+z*sizey*sizex;
}
