#include "PapadimitriouYannakakisSheduler.h"

void PapadimitriouYannakakisSheduler::CalculatePriorities()
{
	vector<string> keys;
	for (auto element : Nodes)
	{
		keys.push_back(element.first);
	}
	int totalCount = keys.size();
	for (int i = 0; i < totalCount; i++)
	{
		Nodes[keys[i]].Priority = CountSuccessors(keys[i]);
	}

}

int PapadimitriouYannakakisSheduler::CountSuccessors(string taskname)
{
	if (Nodes.count(taskname) == 0) return -1;

	Node node = Nodes[taskname];
	if (node.Priority > -1) return node.Priority;

	int successorsCount = node.Successors.size();
	int max = 0;
	for (int i = 0; i < successorsCount; i++)
	{
		max += CountSuccessors(node.Successors[i]) + 1;
	}
	Nodes[taskname].Priority = max;
	return max;

}


