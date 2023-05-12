#include "SchedulerFactory.h"
#include<iostream>

Algorithm algorithm = Algorithm::Hu;
int processesCount = 2;
Scheduler* scheduler;

void setTasks()
{
	if (algorithm == Algorithm::Hu)
	{
		vector<string> tasks{ "a","b", "c" , "d" , "e", "f" , "g" , "h" , "i" , "j" , "k" , "l" , "m" };
		scheduler->addTasks(tasks);
		scheduler->setPredecessors("d", vector<string>{"a", "b", "c"});
		scheduler->setPredecessors("i", vector<string>{"d", "e", "f", "g"});
		scheduler->setPredecessors("k", vector<string>{"h", "i", "j"});
		scheduler->setPredecessors("m", vector<string>{"k", "l"});
	}
	else if (algorithm == PapadimitriouYannakakis)
	{
		vector<string> tasks{ "a","b", "c" , "d" , "e", "f" , "g" , "h" , "i" , "j" };
		scheduler->addTasks(tasks);
		scheduler->setSucessors("a", vector<string>{"c", "d", "e"});
		scheduler->setSucessors("b", vector<string>{"e", "f"});
		scheduler->setSucessors("c", vector<string>{"f", "g"});
		scheduler->setSucessors("d", vector<string>{"f", "g"});
		scheduler->setSucessors("e", vector<string>{"g"});
		scheduler->setSucessors("f", vector<string>{"h"});
		scheduler->setSucessors("g", vector<string>{"h", "i", "j"});
	}
	else if (algorithm == HoffmanGraham)
	{
		vector<string> tasks{ "a","b", "c" , "d" , "e", "f" , "g" , "h" , "i" , "j" , "k" };
		scheduler->addTasks(tasks);
		scheduler->setSucessors("a", vector<string>{"c"});
		scheduler->setSucessors("b", vector<string>{"d"});
		scheduler->setSucessors("c", vector<string>{"e", "f"});
		scheduler->setSucessors("d", vector<string>{"f", "g"});
		scheduler->setSucessors("e", vector<string>{"h", "j"});
		scheduler->setSucessors("f", vector<string>{"h", "i"});
		scheduler->setSucessors("g", vector<string>{"i"});
		scheduler->setSucessors("i", vector<string>{"k"});
		scheduler->setSucessors("h", vector<string>{"j" , "k"});
	}

}
void test()
{
	scheduler = SchedulerFactory::CreateInstance(algorithm);
	scheduler->processesCount = algorithm == Algorithm::HoffmanGraham ? 2 : processesCount;
	setTasks();

	scheduler->Run();
	cout << "Priorities:\n";
	for (auto element : scheduler->Nodes)
	{
		cout << "task: " << element.first << " priority: " << element.second.Priority << "\n";
	}

	cout << "Run steps:\n";
	cout << "Process ";
	for (int p = 0; p < scheduler->processesCount; p++)
	{
		cout << p << " ";
	}
	int stepsCount = scheduler->processesMatrix.size();
	for (int s = 0; s < stepsCount; s++)
	{
		cout << "step " << s << " ";
		int stepSize = scheduler->processesMatrix[s].size();
		for (int t = 0; t < stepSize; t++)
		{
			cout << scheduler->processesMatrix[s][t] << " ";
		}
		cout << "\n";
	}

}

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; i++) {
		if ((strcmp(argv[i], "-algorithm") == 0 || strcmp(argv[i], "-a") == 0) && i < argc - 1)
		{
			if (strcmp(argv[i + 1], "1") == 0)
			{
				algorithm = Algorithm::Hu;
			}
			else if (strcmp(argv[i + 1], "2") == 0)
			{
				algorithm = Algorithm::PapadimitriouYannakakis;
			}
			else if (strcmp(argv[i + 1], "3") == 0)
			{
				algorithm = Algorithm::HoffmanGraham;
			}
		}
		else if ((strcmp(argv[i], "-processes") == 0 || strcmp(argv[i], "-p") == 0) && i < argc - 1)
		{
			processesCount = atoi(argv[i + 1]);
		}
	}
	test();


	return 0;
}