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

bool PointInRect(Point point, Rect rect);
bool RectIntersectsRect(Rect a, Rect b);
bool RectInsideRect(Rect object, Rect container);
Rect ClampRectToRect(Rect object, Rect container);

#endif
