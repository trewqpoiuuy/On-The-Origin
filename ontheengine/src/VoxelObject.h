/*
 * voxelobject.h
 *
 *  Created on: Dec 17, 2013
 *      Author: theron
 */

#ifndef VOXELOBJECT_H_
#define VOXELOBJECT_H_

#include <vector>
#include <GL/gl.h>

class VoxelObject {
	std::vector<bool> d_solid;
	std::vector<int> d_colors;
	int size;
	int offset;

	std::vector<GLfloat> verts;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> texcoords;
public:
	VoxelObject();
	VoxelObject(int);
	void draw();
	int vdaddr(int x, int y, int z);
	void fill(bool state);
	void set(int, int, int, bool);
	bool get(int, int, int);
	void voxelSphere(float centerx, float centery, float centerz, float radius, bool state);
	void voxelSphereNoise(float centerx, float centery, float centerz, float radius, bool state);
	void updateMesh();
	bool dropSeed(float* startx, float* starty, float* startz);
private:
	void init();
};

#endif /* VOXELOBJECT_H_ */
