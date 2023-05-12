#include "BinarySorter.h"

int BinarySorter::binarySearch(vector<ChildrenPriorities> v, ChildrenPriorities item, int low, int high)
{
	while (low <= high) {
		int mid = low + (high - low) / 2;
		int comparision = compare(item, v[mid]);
		if (comparision == 0)
			return mid + 1;
		else if (comparision == 1)
			low = mid + 1;
		else
			high = mid - 1;
	}

	return low;
}

void BinarySorter::Sort(vector<ChildrenPriorities> v)
{
	int i, loc, j, k;
	ChildrenPriorities selected;

	for (i = 1; i < v.size(); ++i) {
		j = i - 1;
		selected = v[i];

		// find location where selected should be inserted
		loc = binarySearch(v, selected, 0, j);

		// Move all elements after location to create space
		while (j >= loc) {
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = selected;
	}

}

int BinarySorter::compare(ChildrenPriorities a, ChildrenPriorities b)
{
	if (a.successors.size() < b.successors.size())
	{
		return -1;
	}
	else if (a.successors.size() > b.successors.size())
	{
		return 1;
	}
	else 
	{
		for (int i = 0; i < a.successors.size(); i++)
		{
			if (a.successors[i] < b.successors[i])
			{
				return -1;
			}
			else if(a.successors[i] > b.successors[i])
			{
				return 1;
			}
		}
		return 0;
	}
}
