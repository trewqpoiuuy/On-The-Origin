#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <string>
#include "math.h"
#include <stdlib.h>
#include <SDL2/SDL_opengl.h>

using namespace std;

float degtorad(float a) //Converts degrees to radians
{
	a = (a / 180) * 3.14159265;
	return a;
}

float radtodeg(float a) //Converts radians to degrees
{
	a = (a / 3.14159265) * 180;
	return a;
}

//calculates Cartesian coordinates from Polar coordinates
vector<float> calcTran(float length, float xAng, float yAng) //Calculates 3-d transforms for each 
{
	float xTran, yTran, zTran;
	xAng = degtorad(xAng);
	yAng = degtorad(yAng);
	xTran = length * sin(yAng) * cos(xAng);
	zTran = length * sin(yAng) * sin(xAng);
	yTran = length * cos(yAng);
	vector<float> transforms(3);
	transforms.at(0) = xTran;
	transforms.at(1) = yTran;
	transforms.at(2) = zTran;
	return transforms;
}

tree CalcXYZ(tree Wtree)
{
	for (int d = 0; d < Wtree.branches.size();)
	{
		while (Wtree.branches.at(d).xyzPos.size() < 3)
		{
			Wtree.branches.at(d).xyzPos.push_back(0);
		}
		Wtree.branches.at(d).xyzPos.at(0) = 0;
		Wtree.branches.at(d).xyzPos.at(1) = 0;
		Wtree.branches.at(d).xyzPos.at(2) = 0;

		while (Wtree.branches.at(d).pRot.size() < 2)
		{
			Wtree.branches.at(d).pRot.push_back(0);
		}
		Wtree.branches.at(d).pRot.at(0) = 0;
		Wtree.branches.at(d).pRot.at(1) = 0;

		d++;
	}
	Wtree.maxvals[0] = 0;
	Wtree.maxvals[1] = 0;
	Wtree.maxvals[2] = 0;
	for (int f = 0; f<Wtree.branches.size();)
	{

		vector<float> xyzPos(3);
		xyzPos.at(0) = 0;
		xyzPos.at(1) = 0;
		xyzPos.at(2) = 0;

		float pyRot = Wtree.branches.at(f).yAngle;
		float pxRot = Wtree.branches.at(f).xAngle;
		pxRot += Wtree.branches.at(f).pRot.at(0);
		pyRot += Wtree.branches.at(f).pRot.at(1);
		//cout << Wtree.at(f).pRot.at(1) << endl;
		float length = Wtree.branches.at(f).length;
		vector<int> children = Wtree.branches.at(f).children;

		xyzPos = calcTran(length, pxRot, pyRot);

		Wtree.branches.at(f).xyzPos.at(0) += xyzPos.at(0);
		Wtree.branches.at(f).xyzPos.at(1) += xyzPos.at(1);
		Wtree.branches.at(f).xyzPos.at(2) += xyzPos.at(2);

		for (int i = 0; i < children.size();)
		{
			int child = Wtree.branches.at(f).children.at(i) - 1;

			Wtree.branches.at(child).pRot.at(0) += pxRot;
			Wtree.branches.at(child).pRot.at(1) += pyRot;
			Wtree.branches.at(child).xyzPos.at(0) += Wtree.branches.at(f).xyzPos.at(0);
			Wtree.branches.at(child).xyzPos.at(1) += Wtree.branches.at(f).xyzPos.at(1);
			Wtree.branches.at(child).xyzPos.at(2) += Wtree.branches.at(f).xyzPos.at(2);
			i++;
		}
		if (Wtree.maxvals[0] < Wtree.branches.at(f).xyzPos.at(0))
		{
			Wtree.maxvals[0] = Wtree.branches.at(f).xyzPos.at(0);
		}
		if (Wtree.maxvals[1] < Wtree.branches.at(f).xyzPos.at(1))
		{
			Wtree.maxvals[1] = Wtree.branches.at(f).xyzPos.at(1);
		}
		if (Wtree.maxvals[2] < Wtree.branches.at(f).xyzPos.at(2))
		{
			Wtree.maxvals[2] = Wtree.branches.at(f).xyzPos.at(2);
		}
		f++;
	}
	return Wtree;
}

void drawBranch(float x1, float y1, float z1, float len1, float x2, float y2, float z2, float len2, float resize, bool cap)
{
	glBegin(GL_QUADS);
	glVertex3f(x2 / resize, y2 / resize, z2 / resize + len2);
	glVertex3f(x2 / resize - len2, y2 / resize, z2 / resize);
	glVertex3f(x1 / resize - len1, y1 / resize, z1 / resize);
	glVertex3f(x1 / resize, y1 / resize, z1 / resize + len1);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(x2 / resize + len2, y2 / resize, z2 / resize);
	glVertex3f(x2 / resize, y2 / resize, z2 / resize + len2);
	glVertex3f(x1 / resize, y1 / resize, z1 / resize + len1);
	glVertex3f(x1 / resize + len1, y1 / resize, z1 / resize);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(x2 / resize, y2 / resize, z2 / resize - len2);
	glVertex3f(x2 / resize + len2, y2 / resize, z2 / resize);
	glVertex3f(x1 / resize + len1, y1 / resize, z1 / resize);
	glVertex3f(x1 / resize, y1 / resize, z1 / resize - len1);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(x2 / resize - len2, y2 / resize, z2 / resize);
	glVertex3f(x2 / resize, y2 / resize, z2 / resize - len2);
	glVertex3f(x1 / resize, y1 / resize, z1 / resize - len1);
	glVertex3f(x1 / resize - len1, y1 / resize, z1 / resize);
	glEnd();
	if (cap == true)
	{
		glBegin(GL_QUADS);
		glVertex3f(x1 / resize + len1, y1 / resize, z1 / resize);

		glVertex3f(x1 / resize, y1 / resize, z1 / resize + len1);
		glVertex3f(x1 / resize - len1, y1 / resize, z1 / resize);
		glVertex3f(x1 / resize, y1 / resize, z1 / resize - len1);
		glEnd();
	}
}

forest renderforest;

void drawForest()
{
	int correction = 0;
	float zpos = 0;

	for (int i = 0; i < renderforest.trees.size();)
	{
		int forestdim;
		
		forestdim = sqrt(renderforest.trees.size()) + 0.5;
		if ((i - correction) > forestdim)
		{
			correction = i;
			zpos += 1;
		}

		glTranslatef(renderforest.trees.at(i).x * 2,renderforest.trees.at(i).z * 2,renderforest.trees.at(i).y * 2);

		for (int f = 0; f < renderforest.trees.at(i).branches.size();)
		{

			float colorwheel = (renderforest.trees.at(i).branches.at(f).xyzPos.at(1) / renderforest.trees.at(i).maxvals[1]);
			//cout << colorwheel << endl;
			float primColor = 2 * (-colorwheel + 0.5f);
			float tertColor = 2 * (colorwheel - 0.5f);
			if (primColor < 0)
			{
				primColor = 0;
			}
			if (tertColor < 0)
			{
				tertColor = 0;
			}
			float secoColor = 1 - (primColor + tertColor);

			int ch = renderforest.trees.at(i).branches.at(f).connection - 1;
			bool cap = false;
			if (renderforest.trees.at(i).branches.at(f).children.size() < 1)
			{
				cap = true;
			}

			//cout << treeSeed.primaryColor[0] * primColor << endl;

			glColorMaterial(GL_FRONT, GL_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);

			glColor3f(((renderforest.trees.at(i).treeSeed.primaryColor[0] * primColor) + (renderforest.trees.at(i).treeSeed.secondaryColor[0] * secoColor) + (renderforest.trees.at(i).treeSeed.tertiaryColor[0] * tertColor)) / 255,
				((renderforest.trees.at(i).treeSeed.primaryColor[1] * primColor) + (renderforest.trees.at(i).treeSeed.secondaryColor[1] * secoColor) + (renderforest.trees.at(i).treeSeed.tertiaryColor[1] * tertColor)) / 255,
				((renderforest.trees.at(i).treeSeed.primaryColor[2] * primColor) + (renderforest.trees.at(i).treeSeed.secondaryColor[2] * secoColor) + (renderforest.trees.at(i).treeSeed.tertiaryColor[2] * tertColor)) / 255);
			//cout << "diam: " << renderforest.trees.at(i).branches.at(f).diameter << " thick: " << renderforest.trees.at(i).thickness << endl;
			if (ch < 0)
			{
				drawBranch(renderforest.trees.at(i).branches.at(f).xyzPos.at(0), renderforest.trees.at(i).branches.at(f).xyzPos.at(1), renderforest.trees.at(i).branches.at(f).xyzPos.at(2),
					renderforest.trees.at(i).branches.at(f).diameter * renderforest.trees.at(i).thickness * 0.5,
					0.0f, 0.0f, 0.0f,
					renderforest.trees.at(i).thickness,
					20.0f, cap);
			}
			else
			{
				drawBranch(renderforest.trees.at(i).branches.at(f).xyzPos.at(0), renderforest.trees.at(i).branches.at(f).xyzPos.at(1), renderforest.trees.at(i).branches.at(f).xyzPos.at(2),
					renderforest.trees.at(i).branches.at(f).diameter  * renderforest.trees.at(i).thickness * 0.5,
					renderforest.trees.at(i).branches.at(ch).xyzPos.at(0), renderforest.trees.at(i).branches.at(ch).xyzPos.at(1), renderforest.trees.at(i).branches.at(ch).xyzPos.at(2),
					renderforest.trees.at(i).branches.at(ch).diameter  * renderforest.trees.at(i).thickness * 0.5,
					20.0f, cap);
			}

			f++;
		}
		
		glTranslatef(-renderforest.trees.at(i).x * 2,-renderforest.trees.at(i).z * 2,-renderforest.trees.at(i).y * 2);
		
		i++;
	}
}

void renderScene(void)
{
	float light0pos[4] = {1,1,0,1};  //70-128
	float light0color[4] = {.6,.45,.4,10};
	float light0amb[4] = {.2,.2,.2,10};
	float light1pos[4] = {5,20,7,0};
	float light1color[4] = {0.2,0.2,0.5};
	glDisable(GL_LIGHTING);

	/*
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0color);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, light1spec);
*/
	// Draw ground
	glColor3f(0.5f, 0.5f, 0.5f);
	/*glBegin(GL_QUADS);
	glVertex3f(-10.0f, 0.0f, 10.0f);
	glVertex3f(-10.0f, 0.0f, -10.0f);
	glVertex3f(10.0f, 0.0f, -10.0f);
	glVertex3f(10.0f, 0.0f, 10.0f);
	glEnd();*/

	drawForest();
}

void passForest(forest &RendForest)
{
	renderforest = RendForest;
}
