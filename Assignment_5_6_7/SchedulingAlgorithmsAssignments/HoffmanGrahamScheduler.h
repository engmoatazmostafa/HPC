#pragma once
#include "Scheduler.h"
#include "BinarySorter.h"

struct NodePriorityParameters {
public:
	string name;
	vector<int> sucessorsPriorities;
	int priority = -1;
};

class HoffmanGrahamScheduler : public Scheduler
{
public:
	void CalculatePriorities();
private:
	map<string, NodePriorityParameters> GetParents(map<string, NodePriorityParameters> children);
	void SetPriorities(map<string, NodePriorityParameters> parametersList);
	int CompareVectors(vector<int> v1, vector<int> v2);
	void SortIntegers(vector<int>& arr);
	void SortParameters(vector<NodePriorityParameters>& arr);
};

