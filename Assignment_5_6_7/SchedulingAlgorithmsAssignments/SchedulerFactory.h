#pragma once
#include "Scheduler.h"
enum Algorithm
{
	Hu, PapadimitriouYannakakis, HoffmanGraham
};
class SchedulerFactory
{
public:
	static Scheduler* CreateInstance(Algorithm algorithm);
};

