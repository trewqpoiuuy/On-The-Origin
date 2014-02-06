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

namespace Voxel {

	const int chunksize = 8;
	const int chunkveclength = chunksize*chunksize*chunksize;

	const int worldchunksize = 32;
	const int worldchunkoffset = worldchunksize/2;
	const int worldchunkveclength = worldchunksize*worldchunksize*worldchunksize;

	const int worldminvalue = -chunksize*worldchunkoffset+chunksize/2;
	const int worldmaxvalue = chunksize*worldchunkoffset-chunksize/2;

	struct Voxel {
		bool solid;
		float color_r;
		float color_y;
		float color_z;
		float density;  // Physics?
		// Resources resources;   // Will's resources Todo
	};

	struct Chunk {
		int xpos;
		int ypos;
		int zpos;
		bool needsupdate;
		Voxel storage[chunkveclength];
		std::vector<GLfloat> verts;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> colors;
		std::vector<GLfloat> texcoords;
	};

	class VoxelObject {

		std::vector<Chunk> chunks;
		Chunk* chunkpointers[worldchunkveclength];
		Chunk* getChunkAtCoord(int x, int y, int z);
		Chunk* getCreateChunkAtCoord(int x, int y, int z);
		Voxel* getVoxelAtCoord(int x, int y, int z);\
		Voxel* getCreateVoxelAtCoord(int x, int y, int z);
		Voxel defVoxel;
		void init();
		void fillChunkAt(int x,int y,int z);
	public:
		VoxelObject();
		void draw();
		int vdaddr(int x, int y, int z);
		void fillRect(int x1, int y1, int z1, int x2, int y2, int z2, bool state);
		void setSolid(int, int, int, bool);
		bool getSolid(int, int, int);
		void voxelSphere(float centerx, float centery, float centerz, float radius, bool state);
		void voxelSphereNoise(float centerx, float centery, float centerz, float radius, bool state);
		void updateMesh();
		bool dropSeed(float* startx, float* starty, float* startz);
		Voxel* editVoxelAt(int x, int y, int z);
	};
}
#endif /* VOXELOBJECT_H_ */
