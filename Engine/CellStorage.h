/*
 * CellStorage.h
 *
 *  Created on: Jan 27, 2014
 *      Author: Theron
 */

#ifndef CELLSTORAGE_H_
#define CELLSTORAGE_H_

#include <vector>

namespace Cell {

	struct level0chunk {
		std::vector<> cells;
	};

	struct level1chunk {
		std::vector<level0chunk> chunks;
	};
}



#endif /* CELLSTORAGE_H_ */
