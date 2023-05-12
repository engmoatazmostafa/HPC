#pragma once
#include "Scheduler.h"
class HuScheduler : public Scheduler
{
public:
	void CalculatePriorities();
private:
	int GetDepth(string taskname);
};

