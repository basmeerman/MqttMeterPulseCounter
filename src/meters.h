#include <Meter.h>
#include <TaskSchedulerDeclarations.h>

#ifndef METERS_H
#define METERS_H

extern Meter meter1;
extern Meter meter2;
extern Meter meter3;

extern Task t_update_meter_average;

void setup_meters();
void update_meter_average();
#endif
