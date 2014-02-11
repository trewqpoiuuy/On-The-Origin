/*
 * World.h
 *
 *  Created on: Feb 10, 2014
 *      Author: Theron
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>

namespace World {

class Entity {
	float x;
	float y;
	float z;
};

struct Voxel {
	//TODO: Solid, color, resources
};

struct Chunk {
	std::vector<Voxel> voxeldata;
	std::vector<Entity> entities;
};

class World {
	int ncwx;  // Number of chunks in world in x direction
	int ncwy;  // Number of chunks in world in y direction
	int ncwz;  // Number of chunks in world in z direction
	int nvcx;  // Number of voxels in chunk in x direction
	int nvcy;  // Number of voxels in chunk in y direction
	int nvcz;  // Number of voxels in chunk in z direction

	std::vector<Chunk> chunkdata;
	std::vector<Chunk*> chunkpdata;
	// All chunk data and functions are internal.  Only Entities and Voxel Data should be interacted with externally.
	void createChunkAtCC(int cx, int cy, int cz);  // Add new chunk at given Chunk Coordinates (CC)
	void getChunkPAtCC(int cx, int cy, int cz);
	Chunk* getCreateChunkPAtCC(int cx, int cy, int cz);  // Try to get chunk at CC, create it if not exists
	int VCtoCC(float d, int nvcd);
public:
	World();
	World(int nvcd, int ncwd);  // In EACH direction:  Number of voxels per chunk, number of chunks in world
	Voxel defaultData;
	Voxel* getDataPAt(float x, float y, float z);  // Returns pointer to Voxel data at coords (x,y,z)
	Voxel retrieveDataAt(float x, float y, float z);  // Returns a copy of the Voxel at coords, or a copy of default if no chunk exists
	std::vector<Entity*> getEntityPsInSphere(float x, float y, float z, float r);  // Returns an array of pointers to entities within radius
	std::vector<Entity*> getEntityPsInRange(float x1, float y1, float z1, float x2, float y2, float z2);  // Returns array of pointers to entities within given range of xyz values
	void applyFuncInRange(void (*func)(float x, float y, float z), float x1, float y1, float z1, float x2, float y2, float z2);
};

} /* namespace World */

#endif /* WORLD_H_ */
