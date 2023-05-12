#include "HoffmanGrahamScheduler.h"

void HoffmanGrahamScheduler::CalculatePriorities()
{
    int priorityCount = 1;
    map<string, NodePriorityParameters> parametersList;
    for (auto element : Nodes)
    {
        if (element.second.Successors.size() == 0)
        {
            NodePriorityParameters parameters;
            parameters.name = element.first;
            parameters.priority = priorityCount;
            parametersList[element.first] = parameters;
            priorityCount++;
        }
    }
    SetPriorities(parametersList);
    parametersList = GetParents(parametersList);
    while (parametersList.size() > 0)
    {
        vector<NodePriorityParameters> parametersVector;
        for (auto element : parametersList)
        {
            parametersVector.push_back(element.second);
        }

        SortParameters(parametersVector);

        for (int i = 0; i < parametersVector.size(); i++)
        {
            string key = parametersVector[i].name;
            if (Nodes.count(key) == 0) continue;

            Nodes[key].Priority = priorityCount;
            priorityCount++;
        }

        parametersList = GetParents(parametersList);
    }
}



map<string, NodePriorityParameters> HoffmanGrahamScheduler::GetParents(map<string, NodePriorityParameters> children)
{
    map<string, NodePriorityParameters> parametersList;
    for (auto element : children)
    {
        string key = element.first;
        if (Nodes.count(key) == 0) continue;

        int predecessorsCount = Nodes[key].Predecessors.size();
        for (int i = 0; i < predecessorsCount; i++)
        {
            string parentKey = Nodes[key].Predecessors[i];

            if (Nodes.count(parentKey) > 0)
            {
                vector<int> successorsPriorities;
                for (int j = 0; j < Nodes[parentKey].Successors.size(); j++)
                {
                    string tmpKey = Nodes[parentKey].Successors[j];

                    if (Nodes.count(tmpKey) > 0 && Nodes[tmpKey].Priority > -1)
                    {
                        successorsPriorities.push_back(Nodes[tmpKey].Priority);
                    }
                }
                if (successorsPriorities.size() == Nodes[parentKey].Successors.size())
                {
                    parametersList[parentKey].name = parentKey;
                    parametersList[parentKey].sucessorsPriorities = successorsPriorities;
                }
            }
        }
    }
    return parametersList;
}

void HoffmanGrahamScheduler::SetPriorities(map<string, NodePriorityParameters> parametersList)
{
    for (auto element : parametersList)
    {
        if (Nodes.count(element.first) == 0) continue;

        Nodes[element.first].Priority = element.second.priority;
    }
}

int HoffmanGrahamScheduler::CompareVectors(vector<int> v1, vector<int> v2)
{
    if (v1.size() < v2.size())
    {
        return -1;
    }
    else if (v2.size() < v1.size())
    {
        return 1;
    }
    else 
    {
        SortIntegers(v1);
        SortIntegers(v2);
        for (int i = 0; i < v1.size(); i++)
        {
            if (v1[i] < v2[i])
            {
                return -1;
            }
            else if (v2[i] < v1[i])
            {
                return 1;
            }
         }
    }
    return 0;
}

void HoffmanGrahamScheduler::SortIntegers(vector<int>& arr)
{
    int i, key, j;
    for (i = 1; i < arr.size(); i++)
    {
        key = arr[i];
        j = i - 1;

        // Move elements of arr[0..i-1], 
        // that are greater than key, to one
        // position ahead of their
        // current position
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void HoffmanGrahamScheduler::SortParameters(vector<NodePriorityParameters>& arr)
{
    int i, key, j;
    NodePriorityParameters tmp;
    for (i = 1; i < arr.size(); i++)
    {
        tmp = arr[i];
        j = i - 1;

        // Move elements of arr[0..i-1], 
        // that are greater than key, to one
        // position ahead of their
        // current position
        while (j >= 0 && CompareVectors(arr[j].sucessorsPriorities, tmp.sucessorsPriorities) > 0)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = tmp;
    }

}


