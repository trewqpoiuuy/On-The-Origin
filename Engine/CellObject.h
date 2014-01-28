/*
 * CellObject.h
 *
 *  Created on: Jan 22, 2014
 *      Author: Theron
 */

#ifndef CELLOBJECT_H_
#define CELLOBJECT_H_

#include "utils.h"
#include <vector>

struct Cell {
	float x;
	float y;
	float z;
	float size;
	// TODO: Resources resources;
};

class CellObject {
	std::vector<Cell> cells;
public:
	CellObject();
	void draw();
	void addcell(Cell cell);
};

#endif /* CELLOBJECT_H_ */