/*
 * voxelobject.cpp
 *
 *  Created on: Dec 17, 2013
 *      Author: theron
 */

#include "VoxelObject.h"
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "utils.h"

#include <assert.h>

namespace Voxel {

	const int V_CSG_UNION=0;
	const int V_CSG_SUBTRACT=1;
	const int V_CSG_INTERSECTION=2;
	const int V_CSG_XOR=3;

	/*int VoxelObject::vdaddr(int x, int y, int z) {
		return x+size*y+size*size*z;
	}*/

	VoxelObject::VoxelObject() {
		init();
	}

	void VoxelObject::init() {
		defVoxel = Voxel{0};
		printf("World min value is %d and max is %d",worldminvalue,worldmaxvalue);
	}

	int sc(int x, int y, int z, int size) {
		return x+y*size+z*size*size;
	}

	int scwc(int x, int y, int z, int c, int wco, int wc) {
		return sc(x/c+wco,y/c+wco,z/c+wco,wc);
	}

	Chunk* VoxelObject::getChunkAtCoord(int x, int y, int z) {
		return chunkpointers[scwc(x,y,z,chunksize,worldchunkoffset,worldchunksize)];
	}

	Chunk* VoxelObject::getCreateChunkAtCoord(int x, int y, int z) {
		printf("chunkpointers[%d]\n",scwc(x,y,z,chunksize,worldchunkoffset,worldchunksize));
		Chunk** chunkpp = &chunkpointers[scwc(x,y,z,chunksize,worldchunkoffset,worldchunksize)];
		printf("pointer: %p\n",chunkpp);
		if (!*chunkpp) {
			chunks.push_back(Chunk{x/chunksize*chunksize,y/chunksize*chunksize,z/chunksize*chunksize});
			*chunkpp = &chunks.back();
			printf("Chunk being created for voxel at %d,%d,%d (Chunk %d,%d,%d)\n",x,y,z,x/chunksize+worldchunkoffset,y/chunksize+worldchunkoffset,z/chunksize+worldchunkoffset);
		}
		assert (*chunkpp);
		return *chunkpp;
	}

	Voxel* VoxelObject::getVoxelAtCoord(int x, int y, int z) {
		return &getChunkAtCoord(x, y, z)->storage[sc(x%chunksize,y%chunksize,z%chunksize,chunksize)];
	}

	Voxel* VoxelObject::getCreateVoxelAtCoord(int x, int y, int z) {
		return &getCreateChunkAtCoord(x, y, z)->storage[sc(x%chunksize,y%chunksize,z%chunksize,chunksize)];
	}

	Voxel* VoxelObject::editVoxelAt(int x, int y, int z) {
		getCreateChunkAtCoord(x,y,z)->needsupdate=true;
		return getCreateVoxelAtCoord(x,y,z);
	}

	bool VoxelObject::dropSeed(float* x, float* y, float* z) {
		// Returns false if seed is being placed inside existing soil
		// Otherwise, adjusts y downwards until seed is sitting on top of a voxel
		if (/*this->get(*x, *y, *z)*/false) {
			return false;
		}
		else {
			/*while (*y>0 && !this->get(*x, *y, *z)) {
				*y-=1;
			}
			*y=ceil(*y);*/
			return true;
		}
	}

	/*void VoxelObject::fillChunkAt(int x, int y, int z, bool state) {

	}*/

	void VoxelObject::fillRect(int x1, int y1, int z1, int x2, int y2, int z2, bool state) {
		/*for (int i=0; i<chunks.size(); i++) {
			Chunk* chunkp = &chunks[i];
			if (chunkp->xpos>x1 and chunkp->ypos>y1 and chunkp->zpos>z1 and chunkp->xpos+chunksize<x2 and chunkp->ypos+chunksize<y2 and chunkp->zpos+chunksize<z2) {
				fillChunkAt(x,y,z);

			}
		}*/
		for (int z=z1; z<z2; z++) {
			for (int y=y1; y<y2; y++) {
				for (int x=x1; x<x2; x++) {
					getCreateVoxelAtCoord(x,y,z)->solid=state;
				}
			}
		}
	}

	void VoxelObject::setSolid(int x, int y, int z, bool state) {
		//d_solid[vdaddr(x,y,z)]=state; // Replace with chunk-based code
		getCreateVoxelAtCoord(x,y,z)->solid = state;
	}

	bool VoxelObject::getSolid(int x, int y, int z) {
		//return d_solid[vdaddr(x,y,z)]; // Replace with chunk-based code
		Voxel* voxelp = getVoxelAtCoord(x,y,z);
		if (voxelp) {
			return voxelp->solid;
		}
		else {
			return defVoxel.solid;
		}
	}

	void VoxelObject::voxelSphere(float centerx, float centery, float centerz, float radius, bool state) {
		for (int z=std::max((int)(centerz-radius),worldminvalue); z<std::min((int)(centerz+radius),worldmaxvalue); z++) {
			for (int y=std::max((int)(centery-radius),worldminvalue); y<std::min((int)(centery+radius),worldmaxvalue); y++) {
				for (int x=std::max((int)(centerx-radius),worldminvalue); x<std::min((int)(centerx+radius),worldmaxvalue); x++) {
					if ((x-centerx)*(x-centerx)+(y-centery)*(y-centery)+(z-centerz)*(z-centerz)<radius*radius) {  // This is ugly.  Use std::pow()?
						//d_solid[vdaddr(x,y,z)]=state;
						editVoxelAt(x,y,z)->solid=state;
					}
				}
			}
		}
	}

	void VoxelObject::draw() {
		for (int i=0; i<chunks.size(); i++) {
			Chunk* chunk = &chunks[i];
			//glPushMatrix();
			//glTranslatef(chunk->xpos,chunk->ypos,chunk->zpos);
			glEnableClientState(GL_VERTEX_ARRAY);
			glEnableClientState(GL_NORMAL_ARRAY);
			//glEnableClientState(GL_COLOR_ARRAY);
			//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &chunk->verts[0]);
			glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), &chunk->normals[0]);
			//glColorPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &colors.front());
			//glTexCoordPointer(2, GL_FLOAT, 0, &texcoords.front());
			glDrawArrays(GL_QUADS, 0*sizeof(GLfloat), chunk->verts.size()/12*sizeof(GLfloat));
			//printf("Drawing %d verts from voxel object\n",verts.size()/3);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			//glDisableClientState(GL_COLOR_ARRAY);
			//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			//glPopMatrix();
		}
	}

	void VoxelObject::updateMesh() {

		/*int xd=1;
		int yd=1;
		int zd=1;

		if (sin(player_yaw)<0) { xd=-1; }    // Double-negatives for consistency
		if (-sin(player_pitch)<0) { yd=-1; }    // var equals sign of expression
		if (-cos(player_yaw)<0) { zd=-1; }


		int xs=size*(xd<0);
		int xe=size*(xd>0);
		int ys=size*(yd<0);
		int ye=size*(yd>0);
		int zs=size*(zd<0);
		int ze=size*(zd>0);*/

		/*for (int i=0; i<nbufferdatatotal*nb; i++) {
			vertexbufferdata[i]=0;
			normalbufferdata[i]=0;
			colorbufferdata[i]=0;
		}*/

		for (int i=0; i<chunks.size(); i++) {
			Chunk* chunkp = &chunks[i];
			if (chunkp->needsupdate) {

				chunkp->verts.clear();
				chunkp->normals.clear();
				chunkp->colors.clear();
				chunkp->texcoords.clear();

				/*for (int z=zs+zd; zd*z<zd*(ze-zd); z+=zd) {
					for (int y=ys+yd; yd*y<yd*(ye-yd); y+=yd) {
						for (int x=xs+xd; xd*x<xd*(xe-xd); x+=xd) {*/ // Leave optimization crap alone
				for (int cz=0; cz<chunksize; cz++) {
					int z = cz+chunkp->zpos;
					for (int cy=0; cy<chunksize; cy++) {
						int y = cy+chunkp->ypos;
						for (int cx=0; cx<chunksize; cx++) {
							int x = cx+chunkp->xpos;
							if (getVoxelAtCoord(x,y,z)->solid==1) {
								/*float normalx=0;
								float normaly=0;
								float normalz=0;
								for (int oz=-1; oz<=1; oz++) {
									for (int oy=0; oy<=1; oy++) {
										for (int ox=-1; ox<=1; ox++) {
											normalx-=(ox*(d_solid[vdaddr(x+ox,y+oy,z+oz)]));
											normaly-=(oy*(d_solid[vdaddr(x+ox,y+oy,z+oz)]));
											normalz-=(oz*(d_solid[vdaddr(x+ox,y+oy,z+oz)]));

										}
									}
								}
								float m=sqrt(normalx*normalx+normaly*normaly+normalz*normalz);
								if (m!=0) {
									normalx = normalx/m;
									normaly = normaly/m;
									normalz = normalz/m;
								}*/

								float r = 1.0;
								float g = 0.0;
								float b = 0.0;

								//////// C++ Generated by voxelcuberule.py ////////
								if (getVoxelAtCoord(0,0,-1)->solid==0) {
									float normalx=0;
									float normaly=0;
									float normalz=0;
									for (int oz=-2; oz<=2; oz++) {
										for (int oy=-2; oy<=2; oy++) {
											for (int ox=-2; ox<=2; ox++) {
												normalx-=ox*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normaly-=oy*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normalz-=oz*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
											}
										}
									}
									normalz+=-2.000000;
									float m=sqrt(normalx*normalx+normaly*normaly+normalz*normalz);
									if (m!=0) {
										normalx = normalx/m;
										normaly = normaly/m;
										normalz = normalz/m;
									}
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z-0.5);
								}
								if (getVoxelAtCoord(0,0,1)->solid==0) {
									float normalx=0;
									float normaly=0;
									float normalz=0;
									for (int oz=-2; oz<=2; oz++) {
										for (int oy=-2; oy<=2; oy++) {
											for (int ox=-2; ox<=2; ox++) {
												normalx-=ox*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normaly-=oy*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normalz-=oz*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
											}
										}
									}
									normalz+=2.000000;
									float m=sqrt(normalx*normalx+normaly*normaly+normalz*normalz);
									if (m!=0) {
										normalx = normalx/m;
										normaly = normaly/m;
										normalz = normalz/m;
									}
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z+0.5);
								}
								if (getVoxelAtCoord(0,-1,0)->solid==0) {
									float normalx=0;
									float normaly=0;
									float normalz=0;
									for (int oz=-2; oz<=2; oz++) {
										for (int oy=-2; oy<=2; oy++) {
											for (int ox=-2; ox<=2; ox++) {
												normalx-=ox*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normaly-=oy*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normalz-=oz*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
											}
										}
									}
									normaly+=-2.000000;
									float m=sqrt(normalx*normalx+normaly*normaly+normalz*normalz);
									if (m!=0) {
										normalx = normalx/m;
										normaly = normaly/m;
										normalz = normalz/m;
									}
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z+0.5);
								}
								if (getVoxelAtCoord(0,1,0)->solid==0) {
									float normalx=0;
									float normaly=0;
									float normalz=0;
									for (int oz=-2; oz<=2; oz++) {
										for (int oy=-2; oy<=2; oy++) {
											for (int ox=-2; ox<=2; ox++) {
												normalx-=ox*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normaly-=oy*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normalz-=oz*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
											}
										}
									}
									normaly+=2.000000;
									float m=sqrt(normalx*normalx+normaly*normaly+normalz*normalz);
									if (m!=0) {
										normalx = normalx/m;
										normaly = normaly/m;
										normalz = normalz/m;
									}
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z-0.5);
								}
								if (getVoxelAtCoord(-1,0,0)->solid==0) {
									float normalx=0;
									float normaly=0;
									float normalz=0;
									for (int oz=-2; oz<=2; oz++) {
										for (int oy=-2; oy<=2; oy++) {
											for (int ox=-2; ox<=2; ox++) {
												normalx-=ox*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normaly-=oy*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normalz-=oz*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
											}
										}
									}
									normalx+=-2.000000;
									float m=sqrt(normalx*normalx+normaly*normaly+normalz*normalz);
									if (m!=0) {
										normalx = normalx/m;
										normaly = normaly/m;
										normalz = normalz/m;
									}
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x-0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z+0.5);
								}
								if (getVoxelAtCoord(1,0,0)->solid==0) {
									float normalx=0;
									float normaly=0;
									float normalz=0;
									for (int oz=-2; oz<=2; oz++) {
										for (int oy=-2; oy<=2; oy++) {
											for (int ox=-2; ox<=2; ox++) {
												normalx-=ox*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normaly-=oy*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
												normalz-=oz*(getVoxelAtCoord(x+ox,y+oy,z+oz)->solid);
											}
										}
									}
									normalx+=2.000000;
									float m=sqrt(normalx*normalx+normaly*normaly+normalz*normalz);
									if (m!=0) {
										normalx = normalx/m;
										normaly = normaly/m;
										normalz = normalz/m;
									}
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z+0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y+0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z-0.5);
									chunkp->colors.push_back(r); chunkp->colors.push_back(g); chunkp->colors.push_back(b);
									chunkp->normals.push_back(normalx); chunkp->normals.push_back(normaly); chunkp->normals.push_back(normalz);
									chunkp->verts.push_back(x+0.5); chunkp->verts.push_back(y-0.5); chunkp->verts.push_back(z+0.5);
								}
								////////       End of generated code       ////////

							}
						}
					}
					printf("Extracting voxels line %d of %d\n",z,chunksize);
				}
				printf("Extracted %d vertices from voxel\n",chunkp->verts.size());
			}
		}
	}
}
