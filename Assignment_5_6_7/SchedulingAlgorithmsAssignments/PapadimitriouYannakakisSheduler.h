#pragma once
#include "Scheduler.h"
class PapadimitriouYannakakisSheduler : public Scheduler
{
public:
	void CalculatePriorities();
private:
	int CountSuccessors(string taskname);
};

