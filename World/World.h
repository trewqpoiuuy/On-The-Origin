/*
 * World.h
 *
 *  Created on: Feb 10, 2014
 *      Author: Theron
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <deque>
#include <GL/gl.h>

namespace World {

class Entity {
	float x;
	float y;
	float z;
};

struct VData {
	bool solid;
	float xnormalc;
	float ynormalc;  // This normal represents only one corner, that which is towards the negative direction
	float znormalc;  // (-0.5 voxel lengths from center in each direction)
	//TODO: Solid, color, resources
};

struct DrawData {
	std::vector<GLfloat> verts;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> texcoords;
};

struct Chunk {
	bool update=false;
	int cx;
	int cy;
	int cz;
	std::vector<VData> voxeldata;
	std::vector<Entity> entities;
	DrawData drawdata;
};

struct WorldSettings {
	int ncwx;  // Number of chunks in world in x direction
	int ncwy;  // Number of chunks in world in y direction
	int ncwz;  // Number of chunks in world in z direction
	int nvcx;  // Number of voxels in chunk in x direction
	int nvcy;  // Number of voxels in chunk in y direction
	int nvcz;  // Number of voxels in chunk in z direction
	int ocwx;  // Offset of chunks in x direction (Can have this many chunks in negative direction, remainder are in positive direction)
	int ocwy;  // Offset of chunks in y direction
	int ocwz;  // Offset of chunks in z direction
	float voxelsize;
	VData defaultdata;
	int normal_smoothing;
};

class World {



	std::deque<Chunk> chunkstorage;
	std::vector<Chunk*> chunkpdata;
	// All chunk data and functions are internal.  Only Entities and Voxel Data should be interacted with externally.
	void createChunkAtCC(int cx, int cy, int cz);  // Add new chunk at given Chunk Coordinates (CC)
	Chunk* getChunkPAtCC(int cx, int cy, int cz);  // Try to get pointer to chunk at CC, return null pointer if doesn't exist
	Chunk* getCreateChunkPAtCC(int cx, int cy, int cz);  // Try to get pointer to chunk at CC, create chunk if doesn't exist
	Chunk* getChunkPAtVC(int x, int y, int z);
	Chunk* getCreateChunkPAtVC(int x, int y, int z);  // Same, but use VC as source, find containing chunk
	VData* getVDataPAtVC(int x, int y, int z);
	VData getVDataDefAtVC(int x, int y, int z);  // Return a copy of the VData at VC, return copy of default if chunk doesn't exist
												 // Only use for temporary calculations, e.g. graphics.  Do not use for world-editing.
	VData* getCreateVDataPAtVC(int x, int y, int z);  // Try to get pointer to voxel at VC, create the chunk for voxel if doesn't exist
	int VCtoCC(int d, int nvcd);
	int VCrelC(int d, int nvcd);
	int scCW(int cx, int cy, int cz);
	int scVC(int vx, int vy, int vz);
	void generateNormalsInChunk(Chunk* chunkp);
	void generateVertsInChunk(Chunk* chunkp);
	void drawChunk(Chunk* chunkp);
	void drawChunkDebug(Chunk* chunkp);
	void init();
public:
	WorldSettings settings;
	World();
	World(WorldSettings _settings);  // In EACH direction:  Number of voxels per chunk, number of chunks in world
	VData defaultData;
	VData* editDataPAt(float x, float y, float z);  // Returns pointer to Voxel data at coords (x,y,z)
	VData retrieveDataAt(float x, float y, float z);  // Returns a copy of the Voxel at coords, or a copy of default if no chunk exists
	std::vector<Entity*> getEntityPsInSphere(float x, float y, float z, float r);  // Returns an array of pointers to entities within radius
	std::vector<Entity*> getEntityPsInRange(float x1, float y1, float z1, float x2, float y2, float z2);  // Returns array of pointers to entities within given range of xyz values
	void applyFuncInRange(void (*func)(float x, float y, float z, VData* datap), float x1, float y1, float z1, float x2, float y2, float z2);
	void voxelGeomSphere(float xc, float yc, float zc, float r, bool solid);
	void update();
	void draw(int mode);
	void drawDebugChunkAtCC(int x, int y, int z);
};

} /* namespace World */

#endif /* WORLD_H_ */
