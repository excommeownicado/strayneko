#include "strayneko.h"

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

MonitorRect
GetMonitorRect(int monitor)
{
    MonitorRect empty = {0};

    if (!Monitors ||
        monitor < 0 ||
        monitor >= MonitorCount) {
        return empty;
    }

    return (MonitorRect) {
        .x = Monitors[monitor].x,
        .y = Monitors[monitor].y,
        .width = Monitors[monitor].width,
        .height = Monitors[monitor].height
    };
}

int
RectOnMonitor(int x, int y, int w, int h)
{
    if (w < 0 || h < 0) {
        return 0;
    }

    Rect object = {
        .x = x,
        .y = y,
        .width = w,
        .height = h
    };

    if (MonitorCount <= 0) {
        Rect root = {
            .x = 0,
            .y = 0,
            .width = (int)WindowWidth,
            .height = (int)WindowHeight
        };

        return RectInsideRect(object, root);
    }

    if (Config.restrict_monitor >= 0 &&
        Config.restrict_monitor < MonitorCount) {
        MonitorRect monitor = GetMonitorRect(Config.restrict_monitor);
        Rect container = {
            .x = monitor.x,
            .y = monitor.y,
            .width = monitor.width,
            .height = monitor.height
        };

        return RectInsideRect(object, container);
    }

    for (int i = 0; i < MonitorCount; i++) {
        MonitorRect monitor = GetMonitorRect(i);
        Rect container = {
            .x = monitor.x,
            .y = monitor.y,
            .width = monitor.width,
            .height = monitor.height
        };

        if (RectInsideRect(object, container)) {
            return 1;
        }
    }

    return 0;
}

int
FindMonitorFor(int x, int y)
{
    Point point = {
        .x = x,
        .y = y
    };

    for (int i = 0; i < MonitorCount; i++) {
        MonitorRect monitor = GetMonitorRect(i);
        Rect rect = {
            .x = monitor.x,
            .y = monitor.y,
            .width = monitor.width,
            .height = monitor.height
        };

        if (PointInRect(point, rect)) {
            return i;
        }
    }

    return -1;
}

MonitorBounds
GetMonitorBounds(int monitor)
{
    MonitorBounds bounds = {0};

    if (!Monitors ||
        monitor < 0 ||
        monitor >= MonitorCount) {
        return bounds;
    }

    MonitorRect rect = GetMonitorRect(monitor);

    bounds.min_x = rect.x;
    bounds.min_y = rect.y;

    int64_t max_x = (int64_t)rect.x +
                    rect.width -
                    BITMAP_WIDTH;

    int64_t max_y = (int64_t)rect.y +
                    rect.height -
                    BITMAP_HEIGHT;

    bounds.max_x = max_x > rect.x
        ? (int)max_x
        : rect.x;

    bounds.max_y = max_y > rect.y
        ? (int)max_y
        : rect.y;

    if (bounds.max_x < bounds.min_x) {
        bounds.max_x = bounds.min_x;
    }

    if (bounds.max_y < bounds.min_y) {
        bounds.max_y = bounds.min_y;
    }

    return bounds;
}
