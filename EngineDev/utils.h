/*
 * utils.h
 *
 *  Created on: Jan 22, 2014
 *      Author: Theron
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <iostream>
#include <stdio.h>

struct vec3 {
	float x;
	float y;
	float z;
};

float randf();

float pythag(float x, float y, float z);

void normalize3p(float* x, float* y, float* z);

float squaref(float x);

#endif /* UTILS_H_ */
