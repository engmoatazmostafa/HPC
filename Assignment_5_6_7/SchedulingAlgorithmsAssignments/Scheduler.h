#pragma once
#include <vector>;
#include <string>
#include <map>
using namespace std;

struct Node
{
public:
	string name = "";
	vector<string> Predecessors;
	vector<string> Successors;
	int Priority = -1;
	bool done = false;
};

struct NodeSchedulingParameters
{
public:
	string name;
	int priority = -1;
};

class Scheduler
{
public:
	virtual void CalculatePriorities() = 0;
	map<string, Node> Nodes;
	int processesCount = 2;
	void addTasks(vector<string> tasksNames);
	void setPredecessors(string taskName, vector<string> predecessors);
	void setSucessors(string taskName, vector<string> sucessors);
	void Run();
	vector<vector<string>> processesMatrix;

private:
	bool TaskReady(string taskname);
	void SortTasks(vector<NodeSchedulingParameters>& tasks);
	vector<string> currentProcessesStatus;

};


//struct vNode
//{
//public:
//	string name = "";
//	vector<vNode*> Predecessors;
//	vector<vNode*> Successors;
//	int Priority = -1;
//	void AddPredecessor(vNode* predecessor);
//	void AddSucessor(vNode* successor);
//};