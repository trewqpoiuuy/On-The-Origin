/*
 * entity.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: theron
 */

#include "entity.h"
#include <GL/gl.h>

Entity::Entity() {

}

void Entity::draw() {
    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
    //glNormalPointer(GL_FLOAT, 0, &normals[0]);
    glColorPointer(3, GL_FLOAT, 0, &colors[0]);
    //glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
    glDrawArrays(GL_QUADS, 0, verts.size()/12*sizeof(GLfloat));
    glDisableClientState(GL_VERTEX_ARRAY);
    //glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Entity::setvectors(std::vector<GLfloat> s_verts, std::vector<GLfloat> s_normals, std::vector<GLfloat> s_colors, std::vector<GLfloat> s_texcoords) {
	verts=s_verts;
	normals=s_normals;
	colors=s_colors;
	texcoords=s_texcoords;
}

/*entity::~entity() {
	// Figure out what this does later...
}*/

