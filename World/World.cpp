/*
 * World.cpp
 *
 *  Created on: Feb 10, 2014
 *      Author: Theron
 */

#include "World.h"

#include <assert.h>
#include "../Misc/utils.h"
#include "../Engine/primitives.h"
#include <math.h>

const double PI=3.14159265358979323846;

namespace World {

World::World() {
	settings.ncwx = 512;
	settings.ocwx = 256;
	settings.ncwy = 32;
	settings.ocwy = 16;
	settings.ncwz = 512;
	settings.ocwz = 256;
	settings.nvcx = 8;
	settings.nvcy = 8;
	settings.nvcz = 8;
	settings.voxelsize = 1;
	settings.defaultdata = VData{false,0,0,0};  // Default VData for when chunks don't exist or aren't loaded.  E.g.: Not solid, and all resources zero
	settings.normal_smoothing = 3;
}

void World::init() {
	chunkpdata.resize(settings.ncwx*settings.ncwy*settings.ncwz, NULL);
	printf("chunkpdata size is %d\n",chunkpdata.size());
}

int sc(int x, int y, int z, int xw, int yw, int zw) {
	return x+y*xw+z*xw*yw;
}

int World::scCW(int cx, int cy, int cz) {
	return (cx+settings.ocwx)+(cy+settings.ocwy)*settings.ncwx+(cz+settings.ocwz)*settings.ncwx*settings.ncwy;
}

int World::scVC(int vx, int vy, int vz) {
	return vx+vy*settings.nvcx+vz*settings.nvcx*settings.nvcy;
}

int World::VCtoCC(int d, int nvcd) {
	return d/nvcd-((d<0 && (d%nvcd)!=0) ? 1:0);  // Adjust for dumb division  (You can tell I really don't like c++'s integer math...)
}

int World::VCrelC(int d, int nvcd) {
	return (d%nvcd+nvcd)%nvcd;  // Adjust for dumb modulo
}

int World::getMinVC_x() {
	return settings.ocwx*settings.nvcx;
}
int World::getMinVC_y() {
	return settings.ocwy*settings.nvcy;
}
int World::getMinVC_z() {
	return settings.ocwz*settings.nvcz;
}
int World::getMaxVC_x() {
	return (settings.ocwx+settings.ncwx)*settings.nvcx-1;
}
int World::getMaxVC_y() {
	return (settings.ocwy+settings.ncwy)*settings.nvcy-1;
}
int World::getMaxVC_z() {
	return (settings.ocwz+settings.ncwz)*settings.nvcz-1;
}
float World::getMin_x() {
	return (settings.ocwx*settings.nvcx)*settings.voxelsize;
}
float World::getMin_y() {
	return (settings.ocwy*settings.nvcy)*settings.voxelsize;
}
float World::getMin_z() {
	return (settings.ocwz*settings.nvcz)*settings.voxelsize;
}
float World::getMax_x() {
	return ((settings.ocwx+settings.ncwx)*settings.nvcx-1)*settings.voxelsize;
}
float World::getMax_y() {
	return ((settings.ocwy+settings.ncwy)*settings.nvcy-1)*settings.voxelsize;
}
float World::getMax_z() {
	return ((settings.ocwz+settings.ncwz)*settings.nvcz-1)*settings.voxelsize;
}

void World::createChunkAtCC(int cx, int cy, int cz) {
	Chunk chunk;
	int index = chunkstorage.size();
	chunkstorage.push_back(chunk);
	Chunk* newchunkp = &chunkstorage.back();
	newchunkp->voxeldata.resize(settings.nvcx*settings.nvcy*settings.nvcz);
	newchunkp->cx = cx;
	newchunkp->cy = cy;
	newchunkp->cz = cz;
	chunkpdata.at(scCW(cx,cy,cz)) = newchunkp;  // Return to pointers!
	// chunkindex.at(scCW(cx,cy,cz)) = index;
}

Chunk* World::getChunkPAtCC(int cx, int cy, int cz) {
	return chunkpdata.at(scCW(cx,cy,cz));
}

Chunk* World::getCreateChunkPAtCC(int cx, int cy, int cz) {
	int p = scCW(cx,cy,cz);
	int n=0;
	Chunk* chunkp = chunkpdata.at(scCW(cx,cy,cz));  // FIXME Breaks here

	if (!chunkp) {
		createChunkAtCC(cx, cy, cz);
	}
	return chunkpdata.at(scCW(cx,cy,cz));
}

Chunk* World::getChunkPAtVC(int x, int y, int z) {
	return getChunkPAtCC(VCtoCC(x,settings.nvcx),VCtoCC(y,settings.nvcy),VCtoCC(z,settings.nvcz));
}

Chunk* World::getCreateChunkPAtVC(int x, int y, int z) {
	return getCreateChunkPAtCC(VCtoCC(x,settings.nvcx),VCtoCC(y,settings.nvcy),VCtoCC(z,settings.nvcz));
}

VData* World::getVDataPAtVC(int x, int y, int z) {
	Chunk* chunkp = getChunkPAtVC(x,y,z);
	if (chunkp) {
		return &chunkp->voxeldata.at(scVC(VCrelC(x,settings.nvcx),VCrelC(y,settings.nvcy),VCrelC(z,settings.nvcz)));
	}
	else {
		return NULL;
	}
}

VData World::getVDataDefAtVC(int x, int y, int z) {
	VData* voxelp = getVDataPAtVC(x,y,z);
	if (voxelp) {
		return *voxelp;
	}
	else {
		return settings.defaultdata;
	}
}

VData* World::getCreateVDataPAtVC(int x, int y, int z) {
	Chunk* chunkp = getCreateChunkPAtVC(x,y,z);
	int xrel = VCrelC(x,settings.nvcx);
	int yrel = VCrelC(y,settings.nvcy);
	int zrel = VCrelC(z,settings.nvcz);
	int p = scVC(xrel,yrel,zrel);
	int q = scVC(VCrelC(x,settings.nvcx),VCrelC(y,settings.nvcy),VCrelC(z,settings.nvcz));
	int r = scVC(0,0,0);
	int s = 0;  //todo clean up this debug mess
	assert (chunkp);
	return &chunkp->voxeldata.at(p);  //FIXME breaks here
}

VData* World::editDataPAt(float x, float y, float z) {
	getCreateChunkPAtVC(x/settings.voxelsize,y/settings.voxelsize,z/settings.voxelsize)->update = true;
	return getVDataPAtVC(x/settings.voxelsize,y/settings.voxelsize,z/settings.voxelsize);
}

VData* World::editDataPAtVC(int vx, int vy, int vz) {
	getCreateChunkPAtVC(vx,vy,vz)->update = true;
	return getVDataPAtVC(vx,vy,vz);
}

VData World::retrieveDataAt(float x, float y, float z) {
	return getVDataDefAtVC(x/settings.voxelsize,y/settings.voxelsize,z/settings.voxelsize);
}

VData World::retrieveDataAtVC(int vx, int vy, int vz) {
	return getVDataDefAtVC(vx,vy,vz);
}

void World::applyFuncInRange(void (*func)(float x, float y, float z, VData* datap), float x1, float y1, float z1, float x2, float y2, float z2) {
	for (int z=z1/settings.voxelsize; z<z2/settings.voxelsize; z++) {
		for (int y=y1/settings.voxelsize; y<y2/settings.voxelsize; y++) {
			for (int x=x1/settings.voxelsize; x<x2/settings.voxelsize; x++) {
				func(x*settings.voxelsize,y*settings.voxelsize,z*settings.voxelsize,editDataPAt(x,y,z));
			}
		}
	}
}

void World::voxelGeomSphere(float xc, float yc, float zc, float r, bool solid) {
	xc/=settings.voxelsize;
	yc/=settings.voxelsize;
	zc/=settings.voxelsize;
	r/=settings.voxelsize;

	for (int z=zc-r; z<zc+r; z+=settings.voxelsize) {
		for (int y=yc-r; y<yc+r; y+=settings.voxelsize) {
			for (int x=xc-r; x<xc+r; x+=settings.voxelsize) {
				if (pythag(x-xc,y-yc,z-zc)<r) {
					editDataPAtVC(x,y,z)->solid=solid;
					//printf("#");
				}
				else {
					//printf(" ");
				}
			}
			//printf("\n");
		}
		//printf("\n");
	}
}

void World::generateNormalsInChunk(Chunk* chunkp) {
	std::vector<bool> soliddata;
	int ns=settings.normal_smoothing;
	int sdwx=settings.nvcx+settings.normal_smoothing*2;
	int sdwy=settings.nvcy+settings.normal_smoothing*2;
	int sdwz=settings.nvcz+settings.normal_smoothing*2;
	int sdlen=sdwx*sdwy*sdwz;
	soliddata.resize(sdlen,false);
	for (int czo=-1; czo<=1; czo++) {
		for (int cyo=-1; cyo<=1; cyo++) {
			for (int cxo=-1; cxo<=1; cxo++) {
				Chunk* cp=getChunkPAtCC(chunkp->cx+cxo,chunkp->cy+cyo,chunkp->cz+czo);
				if (cp) {
					for (int vcz=0; vcz<settings.nvcz; vcz++) {
						for (int vcy=0; vcy<settings.nvcy; vcy++) {
							for (int vcx=0; vcx<settings.nvcx; vcx++) {
								int abx=(cxo)*settings.nvcx+vcx+settings.normal_smoothing;
								int aby=(cyo)*settings.nvcy+vcy+settings.normal_smoothing;
								int abz=(czo)*settings.nvcz+vcz+settings.normal_smoothing;
								if (abx>=0 && abx<sdwx &&
									aby>=0 && aby<sdwy &&
									abz>=0 && abz<sdwz) {
									soliddata.at(sc3w(abx,aby,abz,sdwx,sdwy,sdwz))=cp->voxeldata.at(scVC(vcx,vcy,vcz)).solid;
								}
							}
						}
					}
				}
			}
		}
	}
	//chunkpdebug = chunkp;
	printf("Chunk is at CC (%d,%d,%d)\n",chunkp->cx,chunkp->cy,chunkp->cz);
	/*for (int vz=chunkp->cz*settings.nvcz; vz<(chunkp->cz+1)*settings.nvcz; vz++) {
		for (int vy=chunkp->cy*settings.nvcy; vy<(chunkp->cy+1)*settings.nvcy; vy++) {
			for (int vx=chunkp->cx*settings.nvcx; vx<(chunkp->cx+1)*settings.nvcx; vx++) {*/
	for (int vcz=0; vcz<settings.nvcz; vcz++) {
		for (int vcy=0; vcy<settings.nvcy; vcy++) {
			for (int vcx=0; vcx<settings.nvcx; vcx++) {
				//printf("\tProcessing voxel at (%d,%d,%d)\n",vx,vy,vz);
				VData* tvp = &chunkp->voxeldata.at(scVC(vcx,vcy,vcz));
				float normalx=0;
				float normaly=0;
				float normalz=0;
				/*bool neighbors[8];
				int n=0;
				for (int vzo=-1; vzo<=0; vzo++) {
					for (int vyo=-1; vyo<=0; vyo++) {
						for (int vxo=-1; vxo<=0; vxo++) {
							neighbors[n]=soliddata.at(sc3w(vcx+vxo+ns,vcy+vyo+ns,vcz+vzo+ns,sdwx,sdwy,sdwz));
							n++;
						}
					}
				}
				bool exposed=false;  //Test if this corner is exposed, meaning not all neighbors are empty and not all are solid
				for (int i=0; i<8 && !exposed; i++) {
					if (neighbors[i]!=neighbors[0]) {
						exposed=true;
					}
				}*/
				bool exposed=false;  //Test if this corner is exposed, meaning not all neighbors are empty and not all are solid
				bool first=true;
				bool compare;
				for (int vzo=-1; vzo<=0; vzo++) {
					for (int vyo=-1; vyo<=0; vyo++) {
						for (int vxo=-1; vxo<=0; vxo++) {
							if (first) {
								compare=soliddata.at(sc3w(vcx+vxo+ns,vcy+vyo+ns,vcz+vzo+ns,sdwx,sdwy,sdwz));
								first=false;
							}
							else {
								if (soliddata.at(sc3w(vcx+vxo+ns,vcy+vyo+ns,vcz+vzo+ns,sdwx,sdwy,sdwz))!=compare) {
									exposed=true;
								}
							}
						}
					}
				}
				if (exposed) {
					//printf("\t\t Testing:");
					for (int vzo=-ns; vzo<ns; vzo++) {
						for (int vyo=-ns; vyo<ns; vyo++) {
							for (int vxo=-ns; vxo<ns; vxo++) {    /// Crude normal approximation by calculating weighting of voxels in nrange cubic neighborhood
								if (!soliddata.at(sc3w(vcx+vxo+ns,vcy+vyo+ns,vcz+vzo+ns,sdwx,sdwy,sdwz))) {
									float dist=pythag(vxo+0.5,vyo+0.5,vzo+0.5);
									float direcx = vxo+0.5;
									float direcy = vyo+0.5;
									float direcz = vzo+0.5;
									normalize3p(&direcx,&direcy,&direcz);
									normalx+=direcx/dist;
									normaly+=direcy/dist;
									normalz+=direcz/dist;
								}
							}
						}
					}
				}
				//printf("\n");
				normalize3p(&normalx,&normaly,&normalz);
				/*normalx = (vx-chunkp->cx*settings.nvcx)/8.0;
				normaly = (vy-chunkp->cy*settings.nvcy)/8.0;
				normalz = (vz-chunkp->cz*settings.nvcz)/8.0;*/
				tvp->xnormalc = normalx;
				tvp->ynormalc = normaly;
				tvp->znormalc = normalz;
			}
		}
	}
}

void World::generateVertsInChunk(Chunk* chunkp) {
	printf("Updating chunk with CC (%d,%d,%d)\n",chunkp->cx,chunkp->cy,chunkp->cz);
	DrawData* drawp = &chunkp->drawdata;
	drawp->colors.clear();
	drawp->normals.clear();
	drawp->verts.clear();
	for (int z=chunkp->cz*settings.nvcz; z<(chunkp->cz+1)*settings.nvcz; z++) {
		for (int y=chunkp->cy*settings.nvcy; y<(chunkp->cy+1)*settings.nvcy; y++) {
			for (int x=chunkp->cx*settings.nvcx; x<(chunkp->cx+1)*settings.nvcx; x++) {
				VData* tvp = getVDataPAtVC(x,y,z);
				if (getVDataDefAtVC(x,y,z).solid) {
					//printf("Solid voxel detected at (%d,%d,%d)\n",x,y,z);
					GLfloat r = (x-chunkp->cx*settings.nvcx)/(float)settings.nvcx;
					GLfloat g = (y-chunkp->cy*settings.nvcy)/(float)settings.nvcy;
					GLfloat b = (z-chunkp->cz*settings.nvcz)/(float)settings.nvcz;
					//////// C++ Generated by voxelcuberule.py ////////
					if (getVDataDefAtVC(x+0,y+0,z-1).solid==0) {
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z-0.5);
					}
					if (getVDataDefAtVC(x+0,y+0,z+1).solid==0) {
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z+0.5);
					}
					if (getVDataDefAtVC(x+0,y-1,z+0).solid==0) {
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z+0.5);
					}
					if (getVDataDefAtVC(x+0,y+1,z+0).solid==0) {
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z-0.5);
					}
					if (getVDataDefAtVC(x-1,y+0,z+0).solid==0) {
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+1)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+0,z+0)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+0)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+0,y+1,z+1)->znormalc);
						drawp->verts.push_back(x-0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z+0.5);
					}
					if (getVDataDefAtVC(x+1,y+0,z+0).solid==0) {
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+1)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z+0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+1,z+0)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y+0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+0)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z-0.5);
						drawp->colors.push_back(r); drawp->colors.push_back(g); drawp->colors.push_back(b);
						drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->xnormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->ynormalc); drawp->normals.push_back(getCreateVDataPAtVC(x+1,y+0,z+1)->znormalc);
						drawp->verts.push_back(x+0.5); drawp->verts.push_back(y-0.5); drawp->verts.push_back(z+0.5);
					}
					////////       End of generated code       ////////
				}
			}
		}
	}
	printf("%d verts extracted\n",drawp->verts.size());
}

void World::drawChunk(Chunk* chunkp) {
	glColorMaterial(GL_FRONT, GL_DIFFUSE);  // Thanks Nathan!
	glEnable(GL_COLOR_MATERIAL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &chunkp->drawdata.verts[0]);
    glNormalPointer(GL_FLOAT, 3*sizeof(GLfloat), &chunkp->drawdata.normals[0]);
    glColorPointer(3, GL_FLOAT, 3*sizeof(GLfloat), &chunkp->drawdata.colors[0]);
    //glTexCoordPointer(2, GL_FLOAT, 0, &texcoords.front());
    glDrawArrays(GL_QUADS, 0*sizeof(GLfloat), chunkp->drawdata.verts.size()/12*sizeof(GLfloat));
    //printf("Drawing %d verts from voxel object\n",verts.size()/3);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void World::drawChunkDebug(Chunk* chunkp) {
	if (chunkp) {
		for (int z=chunkp->cz*settings.nvcz; z<(chunkp->cz+1)*settings.nvcz; z++) {
			for (int y=chunkp->cy*settings.nvcy; y<(chunkp->cy+1)*settings.nvcy; y++) {
				for (int x=chunkp->cx*settings.nvcx; x<(chunkp->cx+1)*settings.nvcx; x++) {
					VData v = *getCreateVDataPAtVC(x,y,z);
					//if (chunkp->voxeldata.at(x+y*8+z*8*8).solid) {
					if (getCreateVDataPAtVC(x,y,z)->solid) {
						//drawSphere(x,y,z,.5,3,5);
						float nx = v.xnormalc;
						float ny = v.ynormalc;
						float nz = v.znormalc;
						glBegin(GL_LINES);
						glVertex3f(x,y,z);
						glVertex3f(x+nx,y+ny,z+nz);
						glEnd();
					}
				}
			}
		}
	}
}

void World::drawDebugChunkAtCC(int cx, int cy, int cz) {
	Chunk* chunkp = getChunkPAtCC(cx,cy,cz);
	if (chunkp) {
		for (int z=chunkp->cz*settings.nvcz; z<(chunkp->cz+1)*settings.nvcz; z++) {
			for (int y=chunkp->cy*settings.nvcy; y<(chunkp->cy+1)*settings.nvcy; y++) {
				for (int x=chunkp->cx*settings.nvcx; x<(chunkp->cx+1)*settings.nvcx; x++) {

				}
			}
		}
	}
}

void World::update() {
	printf("%d chunks exist\n",chunkstorage.size());
	unsigned int i;
	for (i=0; i<chunkstorage.size(); i++) {
		Chunk* chunkp=&chunkstorage.at(i);
		//printf("Update chunk?  %d\n",chunkp->update);
		if (chunkp->update) {
			printf("Generating normals in chunk %d\n",i);
			generateNormalsInChunk(chunkp);
			//generateVertsInChunk(chunkp);
		}
	}
	for (i=0; i<chunkstorage.size(); i++) {
		Chunk* chunkp=&chunkstorage.at(i);
		//printf("Update chunk?  %d\n",chunkp->update);
		if (chunkp->update) {
			printf("Generating vertices in chunk %d\n",i);
			//generateNormalsInChunk(chunkp);
			generateVertsInChunk(chunkp);
			chunkp->update=false;
		}
	}
	printf("%d chunks processed in update\n",i);
}

void World::updateNextChunk() {
	unsigned int i=0;
	while (i<chunkstorage.size() && !chunkstorage.at(i).update) {
		i+=1;
	}
	if (i<chunkstorage.size()) {
		generateNormalsInChunk(&chunkstorage.at(i));
		generateVertsInChunk(&chunkstorage.at(i));
		chunkstorage.at(i).update=false;
	}
}

void World::draw(int mode) {
	for (unsigned int i=0; i<chunkstorage.size(); i++) {
		Chunk* chunkp=&chunkstorage.at(i);
		if (mode==0) {
			drawChunk(chunkp);
		}
		if (mode==1) {
			drawChunk(chunkp);
			drawChunkDebug(chunkp);
		}
	}
}

} /* namespace World */
