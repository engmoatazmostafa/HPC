#include "Scheduler.h"

void Scheduler::addTasks(vector<string> tasksNames)
{
	int count = tasksNames.size();
	for (int i = 0; i < count; i++)
	{
		Node node;
		node.name = tasksNames[i];
		Nodes[tasksNames[i]] = node;
	}
}

void Scheduler::setPredecessors(string taskName, vector<string> predecessors)
{
	if (Nodes.count(taskName) == 0) return;

	int count = predecessors.size();
	for (int i = 0; i < count; i++)
	{
		if (Nodes.count(predecessors[i]) == 0) continue;

		Nodes[taskName].Predecessors.push_back(predecessors[i]);
		Nodes[predecessors[i]].Successors.push_back(taskName);
	}

}

void Scheduler::setSucessors(string taskName, vector<string> sucessors)
{
	if (Nodes.count(taskName) == 0) return;

	int count = sucessors.size();
	for (int i = 0; i < count; i++)
	{
		if (Nodes.count(sucessors[i]) == 0) continue;
	
		Nodes[taskName].Successors.push_back(sucessors[i]);
		Nodes[sucessors[i]].Predecessors.push_back(taskName);

	}

}

void Scheduler::Run()
{
	//step 1
	CalculatePriorities();

	//step 2: sort tasks by priorities
	vector<NodeSchedulingParameters> parametersList;
	for (auto element : Nodes)
	{
		NodeSchedulingParameters parameters;
		parameters.name = element.first;
		parameters.priority = element.second.Priority;
		parametersList.push_back(parameters);
	}
	SortTasks(parametersList);

	//step 3: Run
	currentProcessesStatus.clear();
	for (int p = 0; p < processesCount; p++)
	{
		currentProcessesStatus.push_back("");
	}

	int tasksCount = parametersList.size();
	int tasksDone = 0;
	//	int runStep = 1;

	while (tasksDone < tasksCount)
	{
		//find idle process
		int processIndex = 0;
		int taskIndex = 0;
		string taskName;
		while (processIndex < processesCount)
		{
			//allocate ready task to process
			if (strcmp(currentProcessesStatus[processIndex].c_str(), "") == 0)
			{
				bool ready = false;
				while (!ready && taskIndex < tasksCount)
				{
					taskName = parametersList[taskIndex].name;
					ready = TaskReady(taskName);
					taskIndex++;
				}
				if (ready)
				{
					currentProcessesStatus[processIndex] = taskName;
				}
			}
			processIndex++;
		}

		//register status
		vector<string> stepStatus;
		for (int p = 0; p < processesCount; p++)
		{
			stepStatus.push_back(currentProcessesStatus[p]);
			if (strcmp(currentProcessesStatus[p].c_str(), "") != 0)
			{
				//mark task as done
				taskName = currentProcessesStatus[p];
				Nodes[taskName].done = true;
				
				//free process
				currentProcessesStatus[p] = "";
				
				//increase counter for done process
				tasksDone++;
			}
		}
		processesMatrix.push_back(stepStatus);
	}
}

bool Scheduler::TaskReady(string taskname)
{
	if (Nodes.count(taskname) == 0) return true;

	if (Nodes[taskname].done) return false;

	int predecessorsCount = Nodes[taskname].Predecessors.size();
	for (int i = 0; i < predecessorsCount; i++)
	{
		if (Nodes.count(Nodes[taskname].Predecessors[i]) == 0) continue;

		if (!Nodes[Nodes[taskname].Predecessors[i]].done) return false;
	}

	return true;
}

void Scheduler::SortTasks(vector<NodeSchedulingParameters>& tasks)
{
	int sizeOfList = tasks.size();
	int i, key, j;
	NodeSchedulingParameters tmp;
	for (i = 1; i < sizeOfList; i++)
	{
		tmp = tasks[i];
		key = tasks[i].priority;
		j = i - 1;

		// Move elements of arr[0..i-1], 
		// that are less than key, to one
		// position ahead of their
		// current position
		while (j >= 0 && tasks[j].priority < key)
		{
			tasks[j + 1] = tasks[j];
			j = j - 1;
		}
		tasks[j + 1] = tmp;
	}
}

