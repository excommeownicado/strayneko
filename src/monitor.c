#include "strayneko.h"
#include "monitor.h"

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
