/*
 * VoxelWorld.h
 *
 *  Created on: Feb 6, 2014
 *      Author: Theron
 */

#ifndef VOXELWORLD_H_
#define VOXELWORLD_H_

#include <vector>

namespace Voxel {

struct Voxel {
	bool solid;
	float color_r;
	float color_g;
	float color_b;
	// Resources resources;
};

struct Chunk {
	std::vector<Voxel> storage;

};

class VoxelWorld {
public:
	VoxelWorld();
};

} /* namespace Voxel */

#endif /* VOXELWORLD_H_ */
