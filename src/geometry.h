#ifndef STRAYNEKO_GEOMETRY_H
#define STRAYNEKO_GEOMETRY_H

#include <stdint.h>

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

MonitorRect GetMonitorRect(int monitor);
MonitorBounds GetMonitorBounds(int monitor);

int RectOnMonitor(
    int x,
    int y,
    int width,
    int height
);

int FindMonitorFor(int x, int y);

#endif