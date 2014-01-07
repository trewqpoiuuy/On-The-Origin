#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
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
float randFloat(float min, float max)
{
	float r=min+(static_cast <float> (rand())/static_cast <float> (RAND_MAX/(max-min)));
	return r;
}
int randInt(int min, int max)
{
	float r=min+(rand()/(RAND_MAX/(max-min)));
}
int main()
{
	srand (static_cast <unsigned> (time(0)));
	seed treeSeed;
	treeSeed.branchDensity=randFloat(0,1);
	treeSeed.angleVariance=randFloat(0,30);
	treeSeed.featureChance=randFloat(0,1);
	treeSeed.primaryColor[0]=randInt(0,255);
	treeSeed.primaryColor[1]=randInt(0,255);
	treeSeed.primaryColor[2]=randInt(0,255);
	treeSeed.secondaryColor[0]=randInt(0,255);
	treeSeed.secondaryColor[1]=randInt(0,255);
	treeSeed.secondaryColor[2]=randInt(0,255);
	treeSeed.tertiaryColor[0]=randInt(0,255);
	treeSeed.tertiaryColor[1]=randInt(0,255);
	treeSeed.tertiaryColor[2]=randInt(0,255);
	int sunlight=0;
	int nutrients=0; //Will expand this later
	bool isAlive=1;
	vector<branch> tree; 
	branch trunk;
	trunk.connection=0;
	trunk.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
	trunk.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
	trunk.length=randFloat(1,40);
	trunk.feature=0;
	tree.push_back(trunk);
	while(isAlive==1)
	{
		int sunlightAdded;
		int nutrientsAdded;
		cout << "sunlight this turn?" << endl;
		cin >> sunlightAdded;
		sunlight=sunlight+sunlightAdded;
		cout << "nutrients this turn?" << endl;
		cin >> nutrientsAdded;
		nutrients=nutrients+nutrientsAdded;
		while(sunlight>=10 && nutrients>=20)
		{
			branch newBranch;
			newBranch.connection=(tree.size()-(rand()%tree.size()));
			tree.at(newBranch.connection-1).children.push_back(tree.size());
			newBranch.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
			newBranch.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
			newBranch.length=randFloat(1,40);
			newBranch.feature=0;
			int featureChance=rand()%100;
			if(featureChance<treeSeed.featureChance)
			{
				newBranch.feature=rand()%3;
			}
			tree.push_back(newBranch);
			sunlight=sunlight-10;
			nutrients=nutrients-20;
		}
		for(int f=0;f<tree.size();f++)
		{
			cout << "Branch #"  << f+1 << ": " ;
			cout << "Connection Point: " << tree.at(f).connection << " " ;
			cout << "X Angle: " << tree.at(f).xAngle << " " ;
			cout << "Y Angle: " << tree.at(f).yAngle << " ";
			cout <<  "Length: " << tree.at(f).length << " ";
			cout << "Feature: " << tree.at(f).feature << " "<< endl;
		}
		
	}
	
}