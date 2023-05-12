#include "HuScheduler.h"

void HuScheduler::CalculatePriorities()
{
	vector<string> keys;
	for (auto element : Nodes)
	{
		keys.push_back(element.first);
	}
	int totalCount = keys.size();
	for (int i = 0; i < totalCount; i++)
	{
		Nodes[keys[i]].Priority = GetDepth(keys[i]);
	}
}

int HuScheduler::GetDepth(string taskname)
{
	if (Nodes.count(taskname) == 0) return -1;
	
	Node node = Nodes[taskname];
	if (node.Priority > -1) return node.Priority;

	int successorsCount = node.Successors.size();
	int max = 0;
	for (int i = 0; i < successorsCount; i++)
	{
		int tmp = 0;
		tmp = GetDepth(node.Successors[i]);
		if (tmp > max)
		{
			max = tmp;
		}
	}
	max++;
	Nodes[taskname].Priority = max;
	return max;
}

//int HuScheduler::GetDepth(Node* node)
//{
//	if (node->Priority > -1)
//	{
//		return node->Priority;
//	}
//	int sucessorsCount = node->Successors.size();
//	if (sucessorsCount == 0)
//	{
//		node->Priority = 1;
//		return 1;
//	}
//
//	int count = 1;
//	int max = 0;
//	for (int i = 0; i < sucessorsCount; i++)
//	{
//		int tmp = GetDepth(node->Successors[i]);
//		node->Successors[i]->Priority = tmp;
//		if (tmp > max)
//		{
//			max = tmp;
//		}
//	}
//	count += max;
//	return count;
//}
