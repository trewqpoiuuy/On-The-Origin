/*
 * VertexArrayUtils.cpp
 *
 *  Created on: Feb 25, 2014
 *      Author: Theron
 */

#include "VertexArrayUtils.h"
#include <stdio.h>
#include <algorithm>
#include <SDL2/SDL_mutex.h>


namespace VertexArrayUtils {

void drawData(Data* data) {
	while (data->lockdata) { // Thread safety
		//pass
	}
	/*bool trippy=true;
	if (trippy) {
		for (unsigned int i=0; i<data->verts.size(); i++) {
			data->verts[i] += TreeGen::randFloat(-1,1);
		}
	}*/
	glColorMaterial(GL_FRONT, GL_DIFFUSE);  // Thanks Nathan!
	glEnable(GL_COLOR_MATERIAL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &data->verts[0]);
    glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), &data->normals[0]);
    glColorPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &data->colors[0]);
    //glTexCoordPointer(2, GL_FLOAT, 0, &texcoords.front());
    glDrawArrays(GL_QUADS, 0*sizeof(GLfloat), data->verts.size()/12*sizeof(GLfloat));
    //printf("Drawing %d verts from voxel object\n",verts.size()/3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void testDraw() {
	glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex3f(-10.0f, 0.0f, 10.0f);
	glVertex3f(-10.0f, 0.0f, -10.0f);
	glVertex3f(10.0f, 0.0f, -10.0f);
	glVertex3f(10.0f, 0.0f, 10.0f);
	glEnd();
}

void setOffset(Data* data, GLfloat x, GLfloat y, GLfloat z) {
	data->xoffset_temp = x;
	data->yoffset_temp = y;
	data->zoffset_temp = z;
}

void setColor(Data* data, GLfloat r, GLfloat g, GLfloat b) {
	data->r_temp = r;
	data->g_temp = g;
	data->b_temp = b;
}
void setNormal(Data* data, GLfloat x, GLfloat y, GLfloat z) {
	data->normalx_temp = x;
	data->normaly_temp = y;
	data->normalz_temp = z;
}
void addVertex(Data* data, GLfloat x, GLfloat y, GLfloat z) {
	data->verts_temp.push_back(x+data->xoffset_temp);
	data->verts_temp.push_back(y+data->yoffset_temp);
	data->verts_temp.push_back(z+data->zoffset_temp);
	data->normals_temp.push_back(data->normalx_temp);
	data->normals_temp.push_back(data->normaly_temp);
	data->normals_temp.push_back(data->normalz_temp);
	data->colors_temp.push_back(data->r_temp);
	data->colors_temp.push_back(data->g_temp);
	data->colors_temp.push_back(data->b_temp);
}
void finishData(Data* data) {
	printf("data verts_temp size is %d\n",data->verts_temp.size());
	data->lockdata=true;
	data->verts = data->verts_temp;
	data->verts_temp.clear();
	data->normals = data->normals_temp;
	data->lockdata=false;
	data->normals_temp.clear();
	data->colors = data->colors_temp;
	data->colors_temp.clear();
	data->xoffset_temp=0;
	data->yoffset_temp=0;
	data->zoffset_temp=0;
	//printf("verts.size()=%d\n",data->verts.size());
	//printf("verts_temp.size()=%d\n",data->verts_temp.size());
}

}
