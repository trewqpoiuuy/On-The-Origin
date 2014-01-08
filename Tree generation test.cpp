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
	int r=min+(rand()/(RAND_MAX/(max-min)));
	return r;
}
int main()
{
	srand(time(NULL));
	int uselessThing=rand(); //first value of rand is the seed, so this gets rid of it
	seed treeSeed;
	treeSeed.branchDensity=randFloat(0,2);
	treeSeed.angleVariance=randFloat(10,30);
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
	cout << "Seed Data:" << endl;
	cout << "Primary: " << treeSeed.primaryColor[0] << " " << treeSeed.primaryColor[1] << " " << treeSeed.primaryColor[2] << endl;
	cout << "Secondary: " << treeSeed.secondaryColor[0] << " " << treeSeed.secondaryColor[1] << " " << treeSeed.secondaryColor[2] << endl;
	cout << "Tertiary: " << treeSeed.tertiaryColor[0] << " " << treeSeed.tertiaryColor[1] << " " << treeSeed.tertiaryColor[2] << endl;
	cout << "Branch Density: " << treeSeed.branchDensity << " Angle Variance: " << treeSeed.angleVariance << " Feature Chance: "<< treeSeed.featureChance<<endl;
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
			int branchWeighting=randInt(0,tree.size())*treeSeed.branchDensity;
			if(branchWeighting>=tree.size())
			{
				branchWeighting=tree.size()-1;
			}
			newBranch.connection=(tree.size()-branchWeighting);
			tree.at(newBranch.connection-1).children.push_back(tree.size()+1);
			newBranch.xAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
			newBranch.yAngle=randFloat(-treeSeed.angleVariance,treeSeed.angleVariance);
			newBranch.length=randFloat(1,20);
			newBranch.feature=0;
			int featureChance=rand()%100;
			if(featureChance/100<treeSeed.featureChance)
			{
				newBranch.feature=randInt(1,3);
			}
			tree.push_back(newBranch);
			sunlight=sunlight-newBranch.length/2;
			nutrients=nutrients-newBranch.length;
		}
		for(int f=0;f<tree.size();f++)
		{
			cout << "Branch #"  << f+1 << ": " ;
			cout << "Connection Point: " << tree.at(f).connection << " ";
			cout << "X Angle: " << tree.at(f).xAngle << " ";
			cout << "Y Angle: " << tree.at(f).yAngle << " ";
			cout <<  "Length: " << tree.at(f).length << " ";
			cout << "Feature: " << tree.at(f).feature << " " << endl;
			cout << "Children: ";
			for(int g=0;g<tree.at(f).children.size();g++)
			{
				cout << tree.at(f).children.at(g) <<" ";
			}
			cout << endl;
		}
	cout << "Sunlight: " << sunlight << " Nutrients: " << nutrients << endl;
	}
}