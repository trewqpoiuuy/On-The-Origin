/*
 * primitives.cpp
 *
 *  Created on: Dec 15, 2013
 *      Author: theron
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
//#define _USE_MATH_DEFINES  // Screw it.  Just define by hand...
const double M_PI=3.14159265358979323846;
//const double M_PI_2=6.28318530717958647692;
const double M_PI_2=1.5707963267948966;
#include <cmath>

void drawSphere(float x, float y, float z, float radius, unsigned int rings, unsigned int sectors) {   // Function adapted from http://stackoverflow.com/a/5989676
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

    float const R = 1./(float)(rings-1);
            float const S = 1./(float)(sectors-1);
            int r, s;

            vertices.resize(rings * sectors * 3);
            normals.resize(rings * sectors * 3);
            texcoords.resize(rings * sectors * 2);
            std::vector<GLfloat>::iterator v = vertices.begin();
            std::vector<GLfloat>::iterator n = normals.begin();
            std::vector<GLfloat>::iterator t = texcoords.begin();
            for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                    float const y = sin( -M_PI_2 + M_PI * r * R );
                    float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                    float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

                    *t++ = s*S;
                    *t++ = r*R;

                    *v++ = x * radius;
                    *v++ = y * radius;
                    *v++ = z * radius;

                    *n++ = x;
                    *n++ = y;
                    *n++ = z;
            }

            indices.resize(rings * sectors * 4);
            std::vector<GLushort>::iterator i = indices.begin();
            for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
                    *i++ = r * sectors + s;
                    *i++ = r * sectors + (s+1);
                    *i++ = (r+1) * sectors + (s+1);
                    *i++ = (r+1) * sectors + s;
            }

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(x,y,z);

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

            glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
            glNormalPointer(GL_FLOAT, 0, &normals[0]);
            glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
            glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
            glPopMatrix();
}
