/*
 * Cell.h
 *
 *  Created on: Jan 29, 2014
 *      Author: Theron
 */

#ifndef CELL_H_
#define CELL_H_

#include <vector>

namespace Cell {

	class Cell {
	public:
		float x;
		float y;
		float z;
		float size;
		std::vector<Cell>* container;
		// Resources resources;
	public:
		Cell(std::vector<Cell>* container, float x, float y, float z, float size);
		std::vector<Cell*> getCellsNearby(float radius);
		std::vector<Cell*> getCellsTouching();
		void move();
		void destroy();
		void bio();
		void physics();
	};

	//std::vector<Cell> world;

}

#endif /* CELL_H_ */
