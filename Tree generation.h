#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
float randFloat(float min, float max)
{
	//Returns a random floating point number between min and max
	float r=min+(static_cast <float> (rand())/static_cast <float> (RAND_MAX/(max-min)));
	return r;
}
int randInt(int min, int max)
{
	//Returns a random integer between min and max
	int r=min+(rand()/(RAND_MAX/(max-min)));
	return r;
}
struct branch
{
	int connection; //What the branch connects to
	float xAngle; //
	float yAngle; //Polar coordinates relative to connection
	float length; //
	int feature; //0=nothing, 1=flower, 2=fruit. More to come
	vector<int> children; //Branches connected to this one
};
struct seed
{
	float branchDensity; //The higher the value, the lower branches attach 
	float angleVariance; //Variation in angle
	float featureChance; //likelihood of generating features
	float lengthVariance; //lower value-> shorter branches
	int primaryColor[3]; //Bark color
	int secondaryColor[3]; //Leaf color
	int tertiaryColor[3]; //Flower/fruit color
};
struct tree
{
	int sunlight;   //
	int water;      //
	int phosphorus; //Resources
	int nitrogen;   //
	int potassium;  //
	int x;
	int y;
	int z;
	bool isAlive;
	vector<branch> branches;
	seed treeSeed;
};
