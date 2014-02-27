/*
 * VertexArrayUtils.h
 *
 *  Created on: Feb 25, 2014
 *      Author: Theron
 */

#ifndef VERTEXARRAYUTILS_H_
#define VERTEXARRAYUTILS_H_

#include <vector>
#include <GL/gl.h>

namespace VertexArrayUtils {

struct Data {
	std::vector<GLfloat> verts;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> verts_temp;
	std::vector<GLfloat> normals_temp;
	std::vector<GLfloat> colors_temp;
	GLfloat r_temp;
	GLfloat g_temp;
	GLfloat b_temp;
	GLfloat normalx_temp;
	GLfloat normaly_temp;
	GLfloat normalz_temp;
	GLfloat xoffset_temp;
	GLfloat yoffset_temp;
	GLfloat zoffset_temp;
	bool lockdata=false;
};

void setOffset(Data* data, GLfloat x, GLfloat y, GLfloat z);
void setColor(Data* data, GLfloat r, GLfloat g, GLfloat b);
void setNormal(Data* data, GLfloat x, GLfloat y, GLfloat z);
void addVertex(Data* data, GLfloat x, GLfloat y, GLfloat z);
void finishData(Data* data);
void drawData(Data* data);
void testDraw();

}

#endif /* VERTEXARRAYUTILS_H_ */
