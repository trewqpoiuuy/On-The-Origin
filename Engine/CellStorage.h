/*
 * CellStorage.h
 *
 *  Created on: Jan 27, 2014
 *      Author: Theron
 */

#ifndef CELLSTORAGE_H_
#define CELLSTORAGE_H_

#include <vector>
#include "Cell.h"

namespace Cell {

	struct level0chunk {
		std::vector<Cell> cells;
	};

	struct level1chunk {
		std::vector<level0chunk> chunks;
	};

	std::vector<Cell*> getL0chunkCells(float xref, float yref, float zref);
	std::vector<Cell*> getL0chunkCellsRadius(float xref, float yref, float zref, float radius);
}



#endif /* CELLSTORAGE_H_ */
