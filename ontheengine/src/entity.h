/*
 * entity.h
 *
 *  Created on: Dec 17, 2013
 *      Author: theron
 */

#include <vector>
#include <GL/gl.h>

#ifndef ENTITY_H_
#define ENTITY_H_

class Entity {
	std::vector<GLfloat> verts;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> texcoords;
public:
	Entity();
	void draw();
	void setvectors(std::vector<GLfloat> s_verts, std::vector<GLfloat> s_normals, std::vector<GLfloat> s_colors, std::vector<GLfloat> s_texcoords);
	//virtual ~entity();
};

#endif /* ENTITY_H_ */
