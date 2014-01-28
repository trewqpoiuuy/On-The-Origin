/*
 * Plant.h
 *
 *  Created on: Jan 8, 2014
 *      Author: theron
 */

#ifndef PLANT_H_
#define PLANT_H_

#include <vector>

class Plant {
public:
	Plant();
	struct Node {
		std::vector<Node*> cnodes;
		float xrel;
		float yrel;
		float zrel;
	};
	std::vector<Node> nodes;

	void draw();
	void drawnodeR(Node* node, float, float, float);
	Node* addnode(Node* pnode, float xrel, float yrel, float zrel);
	Node* root;
};

#endif /* PLANT_H_ */
