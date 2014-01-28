/*
 * Plant.cpp
 *
 *  Created on: Jan 8, 2014
 *      Author: theron
 */

#include "Plant.h"
#include "primitives.h"

Plant::Plant() {
	Node node;
	nodes.push_back(node);
	root=&nodes.back();
	root->xrel=0;
	root->yrel=0;
	root->zrel=0;
}

Plant::Node* Plant::addnode(Node* pnode, float xrel, float yrel, float zrel) {

	Node node;
	nodes.push_back(node);
	Node* nodep=&nodes.back();
	nodep->xrel = xrel;
	nodep->yrel = yrel;
	nodep->zrel = zrel;
	pnode->cnodes.push_back(pnode);

	return pnode;
}

//Plant::delnode(Node* node) {
	// How to handle orphaned branches?
//}

void Plant::draw() {
	drawnodeR(&nodes[0],0,0,0);
}

void Plant::drawnodeR(Node* node, float xoff, float yoff, float zoff) {

	drawSphere(xoff+node->xrel, yoff+node->yrel, zoff+node->zrel,0.5,3,5);
	for (int i=0; i<node->cnodes.size(); i++) {
		drawnodeR(node->cnodes[i], xoff+node->xrel, yoff+node->yrel, zoff+node->zrel);
	}
}
