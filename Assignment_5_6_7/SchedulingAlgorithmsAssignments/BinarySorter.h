#pragma once
#include <vector>
using namespace std;
class ChildrenPriorities
{
public:
	int ParentNodeIndex = 0;
	vector<int> successors;
};

class BinarySorter
{
public:
	int binarySearch(vector<ChildrenPriorities> v, ChildrenPriorities item, int low, int high);
	void Sort(vector<ChildrenPriorities> v);
private:
	int compare(ChildrenPriorities a, ChildrenPriorities b);
};

