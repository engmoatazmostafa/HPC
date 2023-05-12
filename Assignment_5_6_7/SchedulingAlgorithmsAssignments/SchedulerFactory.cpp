#include "SchedulerFactory.h"
#include "HuScheduler.h"
#include "PapadimitriouYannakakisSheduler.h"
#include "HoffmanGrahamScheduler.h"

Scheduler* SchedulerFactory::CreateInstance(Algorithm algorithm)
{
    if (algorithm == Algorithm::Hu)
    {
        return new HuScheduler();
    }
    else if (algorithm == Algorithm::PapadimitriouYannakakis)
    {
        return new PapadimitriouYannakakisSheduler();
    }
    else
    {
        return new HoffmanGrahamScheduler();
    }
}
