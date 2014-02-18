#include <cstdlib>
#include <ctime>
#include "VectorIntLib.h" //delete 'Resources/' if broken
#include "Tree generation.h"
//#include "TreeRender.h"

using namespace std;
using namespace veclib;

int main()
{
	string uselessThing;
	int depth = 10;
    int length = 100;
    int width = 100;
    int TopsoilDepth = 3;
    DimensionStruct DimInfo;
    DimInfo.length = length;
    DimInfo.width = width;
    DimInfo.depth = depth;
    DimInfo.TopsoilDepth = TopsoilDepth;
    vector<VectorStruct> ResourceVector=initializeResources(DimInfo);
    forest newForest; //all trees
	seed treeSeed=userDefinedSeed();
	while(true)
	{
		srand(12345); // constant seed so that people see how the ... tree egg affects tree growth
		tree newTree=spawnTree(0,0,0,treeSeed,DimInfo,ResourceVector);
		newForest.trees.push_back(newTree);
		newForest=generateTree(newForest, DimInfo, ResourceVector, treeSeed.youth);
		string choice;
		cout << "Edit seed? (y/n) " << endl;
		cin >> choice;
		if (choice=="y")
		{
			changeSeed(treeSeed);
		}
		newForest.trees.clear();		
	}
	cin >> uselessThing;
}