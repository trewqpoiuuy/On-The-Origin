/*
 * Cell.cpp
 *
 *  Created on: Jan 29, 2014
 *      Author: Theron
 */

#include "Cell.h"
#include "utils.h"
#include <vector>

namespace Cell {
	Cell::Cell(std::vector<Cell>* _container, float _x, float _y, float _z, float _size) {
		x = _x;
		y = _y;
		z = _z;
		size = _size;
		container = _container;
	}
	std::vector<Cell*> Cell::getCellsNearby(float radius) {
	}
	std::vector<Cell*> Cell::getCellsTouching() {
		std::vector<Cell*> returncells;
		for (int i=0; i<container->size(); i++) {
			Cell* cellp = &(*container)[i];
			if (pythag(cellp->x-x,cellp->y-y,cellp->z-z) < cellp->size/2 + size/2) {
				returncells.push_back(cellp);
			}
		}
		return returncells;
	}
	void Cell::move() {

	}
	void Cell::destroy() {

	}
	void Cell::physics() {
		float rate=0.1;
		std::vector<Cell*> cells = getCellsTouching();
		float shiftx = 0;
		float shifty = 0;
		float shiftz = 0;
		for (int i=0; i<cells.size(); i++) {
			Cell* cellp = cells[i];
			float xoff = cellp->x - x;
			float yoff = cellp->y - y;
			float zoff = cellp->z - z;
			normalize3p(&xoff,&yoff,&zoff);
			shiftx -= xoff;
			shifty -= yoff;
			shiftz -= zoff;
		}
		normalize3p(&shiftx,&shifty,&shiftz);
		shiftx*=rate; shifty*=rate; shiftz*=rate;
		x += shiftx;
		y += shifty;
		z += shiftz;
	}
}
