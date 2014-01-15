#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <Tree generation.h>
using namespace std;
int main()
{
	srand(time(NULL));
	int uselessThing=rand();
	seed seed1;
	seed seed2;
	seed1.branchDensity=randFloat(0,2);
	seed1.angleVariance=randFloat(10,30);
	seed1.featureChance=randFloat(0,1);
	seed1.primaryColor[0]=randInt(0,255);
	seed1.primaryColor[1]=randInt(0,255);
	seed1.primaryColor[2]=randInt(0,255);
	seed1.secondaryColor[0]=randInt(0,255);
	seed1.secondaryColor[1]=randInt(0,255);
	seed1.secondaryColor[2]=randInt(0,255);
	seed1.tertiaryColor[0]=randInt(0,255);
	seed1.tertiaryColor[1]=randInt(0,255);
	seed1.tertiaryColor[2]=randInt(0,255);
	
	seed2.branchDensity=randFloat(0,2);
	seed2.angleVariance=randFloat(10,30);
	seed2.featureChance=randFloat(0,1);
	seed2.primaryColor[0]=randInt(0,255);
	seed2.primaryColor[1]=randInt(0,255);
	seed2.primaryColor[2]=randInt(0,255);
	seed2.secondaryColor[0]=randInt(0,255);
	seed2.secondaryColor[1]=randInt(0,255);
	seed2.secondaryColor[2]=randInt(0,255);
	seed2.tertiaryColor[0]=randInt(0,255);
	seed2.tertiaryColor[1]=randInt(0,255);
	seed2.tertiaryColor[2]=randInt(0,255);
	
	cout << "Seed 1 Data:" << endl;
	cout << "Primary: " << seed1.primaryColor[0] << " " << seed1.primaryColor[1] << " " << seed1.primaryColor[2] << endl;
	cout << "Secondary: " << seed1.secondaryColor[0] << " " << seed1.secondaryColor[1] << " " << seed1.secondaryColor[2] << endl;
	cout << "Tertiary: " << seed1.tertiaryColor[0] << " " << seed1.tertiaryColor[1] << " " << seed1.tertiaryColor[2] << endl;
	cout << "Branch Density: " << seed1.branchDensity << " Angle Variance: " << seed1.angleVariance << " Feature Chance: "<< seed1.featureChance<<endl;
	
	cout << endl << "Seed 2 Data:" << endl;
	cout << "Primary: " << seed2.primaryColor[0] << " " << seed2.primaryColor[1] << " " << seed2.primaryColor[2] << endl;
	cout << "Secondary: " << seed2.secondaryColor[0] << " " << seed2.secondaryColor[1] << " " << seed2.secondaryColor[2] << endl;
	cout << "Tertiary: " << seed2.tertiaryColor[0] << " " << seed2.tertiaryColor[1] << " " << seed2.tertiaryColor[2] << endl;
	cout << "Branch Density: " << seed2.branchDensity << " Angle Variance: " << seed2.angleVariance << " Feature Chance: "<< seed2.featureChance<<endl;
	
	cout << endl << "Using Averages:"  << endl; //This averages values from the 2 seeds to create a child
	cout << "Primary: " << (seed1.primaryColor[0]+seed2.primaryColor[0])/2 << " " << (seed1.primaryColor[1]+seed2.primaryColor[1])/2  << " " << (seed1.primaryColor[2]+seed2.primaryColor[2])/2  << endl;
	cout << "Secondary: " << (seed1.secondaryColor[0]+seed2.secondaryColor[0])/2  << " " << (seed1.secondaryColor[1]+seed2.secondaryColor[1])/2 << " " << (seed1.secondaryColor[2]+seed2.secondaryColor[2])/2 << endl;
	cout << "Tertiary: " << (seed1.tertiaryColor[0]+seed2.tertiaryColor[0])/2 << " " << (seed1.tertiaryColor[1]+seed2.tertiaryColor[1])/2 << " " << (seed1.tertiaryColor[2]+seed2.tertiaryColor[2])/2 << endl;
	cout << "Branch Density: " << (seed1.branchDensity+seed2.branchDensity)/2 << " Angle Variance: " << (seed1.angleVariance+seed2.angleVariance)/2 << " Feature Chance: "<< (seed1.featureChance+seed2.featureChance)/2<<endl;
	
	
	cout << endl << "Choosing Parts:"  << endl; //This randomly selects values from one seed or the other to generate the child
	for (int i = 0; i<12 ; i++)
	{
		if (randInt(0,2)==1)
		{
			switch(i)
			{
			case 0: cout << "Primary: " << seed1.primaryColor[0] << " ";
					break;
			case 1: cout << seed1.primaryColor[1] << " ";
					break;
			case 2: cout << seed1.primaryColor[2] << endl;
					break;
			case 3: cout << "Secondary: " << seed1.secondaryColor[0] << " ";
					break;
			case 4: cout << seed1.secondaryColor[1] << " ";
					break;
			case 5: cout << seed1.secondaryColor[2] << endl;
					break;
			case 6: cout << "Tertiary: " << seed1.tertiaryColor[0] << " ";
					break;
			case 7: cout << seed1.tertiaryColor[1] << " ";
					break;
			case 8: cout << seed1.tertiaryColor[2] << endl;
					break;
			case 9: cout << "Branch Density: " << seed1.branchDensity;
					break;
			case 10: cout << " Angle Variance: " << seed1.angleVariance;
					break;
			case 11: cout << " Feature Chance: "<< seed1.featureChance << endl;
			}
		}
		else
		{
			switch(i)
			{
			case 0: cout << "Primary: " << seed2.primaryColor[0] << " ";
					break;
			case 1: cout << seed2.primaryColor[1] << " ";
					break;
			case 2: cout << seed2.primaryColor[2] << endl;
					break;
			case 3: cout << "Secondary: " << seed2.secondaryColor[0] << " ";
					break;
			case 4: cout << seed2.secondaryColor[1] << " ";
					break;
			case 5: cout << seed2.secondaryColor[2] << endl;
					break;
			case 6: cout << "Tertiary: " << seed2.tertiaryColor[0] << " ";
					break;
			case 7: cout << seed2.tertiaryColor[1] << " ";
					break;
			case 8: cout << seed2.tertiaryColor[2] << endl;
					break;
			case 9: cout << "Branch Density: " << seed2.branchDensity;
					break;
			case 10: cout << " Angle Variance: " << seed2.angleVariance;
					break;
			case 11: cout << " Feature Chance: "<< seed2.featureChance << endl;
			}
		}
	}
	cin >> uselessThing;
}