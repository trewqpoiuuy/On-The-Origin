/*
 * CellObject.cpp
 *
 *  Created on: Jan 22, 2014
 *      Author: Theron
 */

#include "CellObject.h"
#include "primitives.h"

namespace Cell {

	CellObject::CellObject() {
		// TODO Auto-generated constructor stub

	}

	void CellObject::draw() {
		for (int i=0; i<cells.size(); i++) {
			Cell* cell = &cells[i];
			drawSphere(cell->x,cell->y,cell->z,cell->size,3,5);
		}
	}

	void CellObject::addcell(Cell cell) {
		cells.push_back(cell);
	}
}
