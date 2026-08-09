#ifndef STRAYNEKO_MONITOR_H
#define STRAYNEKO_MONITOR_H

#include "geometry.h"

int RectOnMonitor(
    int x,
    int y,
    int width,
    int height
);

int FindMonitorFor(int x, int y);
MonitorRect GetMonitorRect(int monitor);
MonitorBounds GetMonitorBounds(int monitor);

#endif
