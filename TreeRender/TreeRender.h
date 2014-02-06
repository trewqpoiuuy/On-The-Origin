#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "math.h"
#include <stdlib.h>
#include "../Engine/Engine.h"
#include "../Engine/primitives.h"
#include <GL/gl.h>



using namespace std;

float degtorad(float a)
{
	a = (a / 180) * 3.14159265;
	return a;
}

float radtodeg(float a)
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

	//yAng += 1.5708;

	xTran = length * sin(yAng) * cos(xAng);
	zTran = length * sin(yAng) * sin(xAng);
	yTran = length * cos(yAng);
	vector<float> transforms(3);
	transforms.at(0) = xTran;
	transforms.at(1) = yTran;
	transforms.at(2) = zTran;
	return transforms;
}

float angle = 0.0f;
float tRot = 0.0f;

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

void drawBranch(float x1, float y1, float z1, float len1, float x2, float y2, float z2, float len2)
{
	glBegin(GL_QUADS);
	glVertex3f(x2 / 20 - len2, y2 / 20, z2 / 20);
	glVertex3f(x2 / 20, y2 / 20, z2 / 20 + len2);
	glVertex3f(x1 / 20, y1 / 20, z1 / 20 + len1);
	glVertex3f(x1 / 20 - len1, y1 / 20, z1 / 20);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(x2 / 20, y2 / 20, z2 / 20 + len2);
	glVertex3f(x2 / 20 + len2, y2 / 20, z2 / 20);
	glVertex3f(x1 / 20 + len1, y1 / 20, z1 / 20);
	glVertex3f(x1 / 20, y1 / 20, z1 / 20 + len1);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(x2 / 20 + len2, y2 / 20, z2 / 20);
	glVertex3f(x2 / 20, y2 / 20, z2 / 20 - len2);
	glVertex3f(x1 / 20, y1 / 20, z1 / 20 - len1);
	glVertex3f(x1 / 20 + len1, y1 / 20, z1 / 20);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(x2 / 20, y2 / 20, z2 / 20 - len2);
	glVertex3f(x2 / 20 - len2, y2 / 20, z2 / 20);
	glVertex3f(x1 / 20 - len1, y1 / 20, z1 / 20);
	glVertex3f(x1 / 20, y1 / 20, z1 / 20 - len1);
	glEnd();
}

tree rendertree;

void drawTree() {


	glColor3f(1.0f, 0.0f, 0.0f);
	//glutSolidCube(0.2f);

	for (int f = 0; f < rendertree.branches.size();)
	{



		float colorwheel = (rendertree.branches.at(f).xyzPos.at(1) / rendertree.maxvals[1]);
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

		int ch = rendertree.branches.at(f).connection - 1;

		//cout << treeSeed.primaryColor[0] * primColor << endl;

		glColor3f(((rendertree.treeSeed.primaryColor[0] * primColor) + (rendertree.treeSeed.secondaryColor[0] * secoColor) + (rendertree.treeSeed.tertiaryColor[0] * tertColor)) / 255,
			((rendertree.treeSeed.primaryColor[1] * primColor) + (rendertree.treeSeed.secondaryColor[1] * secoColor) + (rendertree.treeSeed.tertiaryColor[1] * tertColor)) / 255,
			((rendertree.treeSeed.primaryColor[2] * primColor) + (rendertree.treeSeed.secondaryColor[2] * secoColor) + (rendertree.treeSeed.tertiaryColor[2] * tertColor)) / 255);
		if (ch < 0)
		{
			drawBranch(rendertree.branches.at(f).xyzPos.at(0), rendertree.branches.at(f).xyzPos.at(1), rendertree.branches.at(f).xyzPos.at(2), 0.1f,
				0.0f, 0.0f, 0.0f, 0.1f);
		}
		else
		{
			drawBranch(rendertree.branches.at(f).xyzPos.at(0), rendertree.branches.at(f).xyzPos.at(1), rendertree.branches.at(f).xyzPos.at(2), 0.1f,
				rendertree.branches.at(ch).xyzPos.at(0), rendertree.branches.at(ch).xyzPos.at(1), rendertree.branches.at(ch).xyzPos.at(2), 0.1f);
		}

		f++;

	}


}
