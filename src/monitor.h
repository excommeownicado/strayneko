#ifndef STRAYNEKO_MONITOR_H
#define STRAYNEKO_MONITOR_H

#include "geometry.h"

typedef struct {
    int x;
    int y;
    int width;
    int height;
} MonitorRect;

typedef struct {
    int min_x;
    int min_y;
    int max_x;
    int max_y;
} MonitorBounds;

void InitMonitors(void);

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
