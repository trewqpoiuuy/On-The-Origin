#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
float randFloat(float min, float max)
{
	float r=min+(static_cast <float> (rand())/static_cast <float> (RAND_MAX/(max-min)));
	return r;
}
int randInt(int min, int max)
{
	int r=min+(rand()/(RAND_MAX/(max-min)));
	return r;
}
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