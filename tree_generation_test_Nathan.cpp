
#include <stdafx.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "math.h"
#include <stdlib.h>
#include <GL/glut.h>


using namespace std;

//largely uncommented, older version of Ben's tree generator
//will not work without GLUT installed


struct branch
{
	int connection;
	float xAngle;
	float yAngle;
	float length;
	int feature;
	vector<int> children;
};
struct seed
{
	float branchDensity;
	float angleVariance;
	float featureChance;
	int primaryColor[3];
	int secondaryColor[3];
	int tertiaryColor[3];
};

vector<branch> tree;
branch trunk;

float randFloat(float min, float max)
{
	float r = min + (static_cast <float> (rand()) / static_cast <float> (RAND_MAX / (max - min)));
	return r;
}
int randInt(int min, int max)
{
	int r = min + (rand() / (RAND_MAX / (max - min)));
	return r;
}

float red = 1.0f;
float green = 1.0f;
float blue = 1.0f;

// angle of rotation for the camera direction
float ry = 0.0;
// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;

float deltaAngle = 0.0f;
float deltaMove = 0;

float dAngle = 0.0f;
int xOrigin = -1;

void mouseButton(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			ry += dAngle;
			xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
		}
	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		dAngle = (x - xOrigin) * 0.01f;

		// update camera's direction
		lx = sin(ry + dAngle);
		lz = -cos(ry + dAngle);
	}
}

void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void computeDir(float deltaAngle) {

	ry += deltaAngle;
	lx = sin(ry);
	lz = -cos(ry);
}

void pressKey(int key, int xx, int yy)
{
	switch (key) {
	case GLUT_KEY_LEFT: deltaAngle = -0.01f; break;
	case GLUT_KEY_RIGHT: deltaAngle = 0.01f; break;
	case GLUT_KEY_UP: deltaMove = 0.5f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
	}

}

void releaseKey(int key, int xx, int yy)
{

	switch (key) {
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT: deltaAngle = 0.0f; break;
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}

}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

float degtorad(float a)
{
	a = (a / 180) * 3.14159265;
	return a;
}

vector<float> calcTran (float length, float xAng, float yAng) //Calculates 3-d transforms for each 
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

void renderScene(void) {

	if (deltaMove)
		computePos(deltaMove);
	if (deltaAngle)
		computeDir(deltaAngle);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(x, 1.0f, z,
		x + lx, 1.0f, z + lz,
		0.0f, 1.0f, 0.0f);

	// Draw ground
	/*glColor3f(0.5f, 0.5f, 0.5f);
	glBegin(GL_QUADS);
	glVertex3f(-100.0f, 0.0f, -100.0f);
	glVertex3f(-100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, 100.0f);
	glVertex3f(100.0f, 0.0f, -100.0f);
	glEnd();*/

	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidCube(0.3f);

	for (int f = 0; f<tree.size();)
	{
		vector<float> xyzPos(3);
		xyzPos.at(0) = 0;
		xyzPos.at(1) = 0;
		xyzPos.at(2) = 0;

		float yRot = tree.at(f).yAngle;
		float xRot = tree.at(f).xAngle;

		float brLen = 0;
		
		float length = tree.at(f).length;

		xyzPos.at(0) += calcTran(length, xRot, yRot).at(0);
		xyzPos.at(1) += calcTran(length, xRot, yRot).at(1);
		xyzPos.at(2) += calcTran(length, xRot, yRot).at(2);

		float treeconnection = 0;

		treeconnection = tree.at(f).connection;

		for (int c = f; c > 0; c--) //negating previous translations
		{
			if (treeconnection == c)
			{
				brLen = tree.at(c-1).length;
				treeconnection = tree.at(c-1).connection;
				yRot = tree.at(c - 1).yAngle;
				xRot = tree.at(c - 1).xAngle;
				xyzPos.at(0) += calcTran(brLen, xRot, yRot).at(0);
				xyzPos.at(1) += calcTran(brLen, xRot, yRot).at(1);
				xyzPos.at(2) += calcTran(brLen, xRot, yRot).at(2);
			}
		}
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(xyzPos.at(0)/20, (xyzPos.at(1) /*+ brLen*/)/20, xyzPos.at(2)/20);
		glutSolidSphere(0.25f, 20, 20);
		/*glRotatef(xRot, 0.0f, 1.0f, 0.0f);
		glColor3f(0.9f, 0.9f, 0.9f);
		glutSolidCone(0.25f, length/20, 10, 10);
		glColor3f(1.0f, 1.0f, 1.0f);
		glRotatef(-xRot, 0.0f, 1.0f, 0.0f);*/
		glTranslatef(-xyzPos.at(0)/20, -(xyzPos.at(1) /*+ brLen*/)/20, -xyzPos.at(2)/20);
		/*cout << "Branch #" << f + 1 << ": ";
		cout << "Connection Point: " << tree.at(f).connection << " ";
		cout << "X Angle: " << tree.at(f).xAngle << " ";
		cout << "Y Angle: " << tree.at(f).yAngle << " ";
		cout << "Length: " << tree.at(f).length << " ";
		cout << "Feature: " << tree.at(f).feature << " " << endl;*/
		f++;
	}

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	srand(static_cast <unsigned> (time(0)));
	seed treeSeed;
	treeSeed.branchDensity = randFloat(0, 2);
	treeSeed.angleVariance = randFloat(0, 30);
	treeSeed.featureChance = randFloat(0, 1);
	treeSeed.primaryColor[0] = randInt(0, 255);
	treeSeed.primaryColor[1] = randInt(0, 255);
	treeSeed.primaryColor[2] = randInt(0, 255);
	treeSeed.secondaryColor[0] = randInt(0, 255);
	treeSeed.secondaryColor[1] = randInt(0, 255);
	treeSeed.secondaryColor[2] = randInt(0, 255);
	treeSeed.tertiaryColor[0] = randInt(0, 255);
	treeSeed.tertiaryColor[1] = randInt(0, 255);
	treeSeed.tertiaryColor[2] = randInt(0, 255);
	cout << treeSeed.branchDensity << " " << treeSeed.angleVariance << endl;
	int sunlight = 0;
	int nutrients = 0; //Will expand this later
	bool isAlive = 1;
	
	trunk.connection = 0;
	trunk.xAngle = randFloat(-treeSeed.angleVariance, treeSeed.angleVariance);
	trunk.yAngle = randFloat(-treeSeed.angleVariance, treeSeed.angleVariance);
	trunk.length = randFloat(1, 40);
	trunk.feature = 0;
	tree.push_back(trunk);
	while (isAlive == 1)
	{
		int sunlightAdded;
		int nutrientsAdded;
		cout << "sunlight this turn?" << endl;
		cin >> sunlightAdded;
		sunlight = sunlight + sunlightAdded;
		cout << "nutrients this turn?" << endl;
		cin >> nutrientsAdded;
		nutrients = nutrients + nutrientsAdded;
		while (sunlight >= 10 && nutrients >= 20)
		{
			branch newBranch;
			int branchWeighting = randInt(0, tree.size())*treeSeed.branchDensity;
			if (branchWeighting >= tree.size())
			{
				branchWeighting = tree.size() - 1;
			}
			newBranch.connection = (tree.size() - branchWeighting);
			tree.at(newBranch.connection - 1).children.push_back(tree.size());
			newBranch.xAngle = randFloat(-treeSeed.angleVariance, treeSeed.angleVariance);
			newBranch.yAngle = randFloat(-treeSeed.angleVariance, treeSeed.angleVariance);
			newBranch.length = randFloat(1, 40);
			newBranch.feature = 0;
			int featureChance = rand() % 100;
			if (featureChance<treeSeed.featureChance)
			{
				newBranch.feature = rand() % 3;
			}
			tree.push_back(newBranch);
			sunlight = sunlight - 10;
			nutrients = nutrients - 20;
		}
		for (int f = 0; f<tree.size(); f++)
		{
			cout << "Branch #" << f + 1 << ": ";
			cout << "Connection Point: " << tree.at(f).connection << " ";
			cout << "X Angle: " << tree.at(f).xAngle << " ";
			cout << "Y Angle: " << tree.at(f).yAngle << " ";
			cout << "Length: " << tree.at(f).length << " ";
			cout << "Feature: " << tree.at(f).feature << " " << endl;
		}
		cout << "Kill? (1 for yes, 0 for no)" << endl;
		bool isKilled;
		cin >> isKilled;
		if (isKilled == 1)
		{
			isAlive = 0;
		}
	}
	cout << "Generation Ended" << endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Tree Generation Test");
	

	// register callbacks
	

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutSpecialFunc(pressKey);
	glutIgnoreKeyRepeat(1);
	glutSpecialUpFunc(releaseKey);

	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);
	

	//depth test
	glEnable(GL_DEPTH_TEST);

	// enter GLUT event processing loop
	glutMainLoop();

}