#include "geometry.h"

bool
PointInRect(Point point, Rect rect)
{
    int64_t right = (int64_t)rect.x + rect.width;
    int64_t bottom = (int64_t)rect.y + rect.height;

    return point.x >= rect.x &&
           point.y >= rect.y &&
           (int64_t)point.x < right &&
           (int64_t)point.y < bottom;
}

bool
RectIntersectsRect(Rect a, Rect b)
{
    int64_t a_right = (int64_t)a.x + a.width;
    int64_t a_bottom = (int64_t)a.y + a.height;
    int64_t b_right = (int64_t)b.x + b.width;
    int64_t b_bottom = (int64_t)b.y + b.height;

    return a.x < b_right &&
           a_right > b.x &&
           a.y < b_bottom &&
           a_bottom > b.y;
}

bool
RectInsideRect(Rect object, Rect container)
{
    int64_t object_right = (int64_t)object.x + object.width;
    int64_t object_bottom = (int64_t)object.y + object.height;
    int64_t container_right = (int64_t)container.x + container.width;
    int64_t container_bottom = (int64_t)container.y + container.height;

    return object.x >= container.x &&
           object.y >= container.y &&
           object_right <= container_right &&
           object_bottom <= container_bottom;
}

Rect
ClampRectToRect(Rect object, Rect container)
{
    Rect result = object;

    if (result.width >= container.width) {
        result.x = container.x;
    } else {
        if (result.x < container.x) {
            result.x = container.x;
        }

        if ((int64_t)result.x + result.width >
            (int64_t)container.x + container.width) {
            result.x = container.x + container.width - result.width;
        }
    }

    if (result.height >= container.height) {
        result.y = container.y;
    } else {
        if (result.y < container.y) {
            result.y = container.y;
        }

        if ((int64_t)result.y + result.height >
            (int64_t)container.y + container.height) {
            result.y = container.y + container.height - result.height;
        }
    }

    return result;
}
