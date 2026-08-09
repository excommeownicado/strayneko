#ifndef STRAYNEKO_GEOMETRY_H
#define STRAYNEKO_GEOMETRY_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    int width;
    int height;
} Size;

typedef struct {
    int x;
    int y;
    int width;
    int height;
} Rect;

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

bool PointInRect(Point point, Rect rect);
bool RectIntersectsRect(Rect a, Rect b);
bool RectInsideRect(Rect object, Rect container);
Rect ClampRectToRect(Rect object, Rect container);

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
